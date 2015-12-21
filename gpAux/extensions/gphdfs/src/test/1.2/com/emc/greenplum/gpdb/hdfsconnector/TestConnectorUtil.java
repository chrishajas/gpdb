/*-------------------------------------------------------------------------
 *
 * TestConnectorUtil
 *
 * Copyright (c) 2011 EMC Corporation All Rights Reserved
 *
 * This software is protected, without limitation, by copyright law
 * and international treaties. Use of this software and the intellectual
 * property contained therein is expressly limited to the terms and
 * conditions of the License Agreement under which it is provided by
 * or on behalf of EMC.
 *
 *-------------------------------------------------------------------------
 */

package com.emc.greenplum.gpdb.hdfsconnector;

import org.apache.hadoop.fs.LocatedFileStatus;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.fs.RemoteIterator;
import org.apache.hadoop.hdfs.MiniDFSCluster;
import org.apache.hadoop.hdfs.server.datanode.DataNode;
import org.apache.hadoop.conf.Configuration;

import static org.junit.Assert.*;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import com.emc.greenplum.gpdb.hadoop.formathandler.AvroFileReader;
import com.emc.greenplum.gpdb.hadoop.formathandler.GpdbParquetFileReader;
import com.emc.greenplum.gpdb.hadoop.io.GPDBWritable;

import java.net.URI;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;

public class TestConnectorUtil  {
    private static MiniDFSCluster cluster;

    /*
     * setup the cluster and upload test files
     */
    @BeforeClass
    public static void setupBeforeClass() throws IllegalArgumentException, IOException {
		final Configuration conf = new Configuration();
		cluster =  new MiniDFSCluster.Builder(conf).numDataNodes(1).format(true).build();
		cluster.getFileSystem().mkdirs(new Path("/tmp"));

		int hadoopPort = cluster.getNameNodePort();
		String tmpDir = "hdfs://127.0.0.1:" + hadoopPort + "/tmp";

		//decimal.pq : file generated by hive using parquet format, contains two
		//columns of decimal
		Path decimal = new Path(tmpDir + "/decimal.pq");
		cluster.getFileSystem().copyFromLocalFile(new Path((new File("")).getAbsolutePath() + "/src/test/data/decimal.pq"), decimal);
		//alertlog.avro : file contains unicode text
		Path alertlog = new Path(tmpDir + "/alertlog.avro");
		cluster.getFileSystem().copyFromLocalFile(new Path((new File("")).getAbsolutePath() + "/src/test/data/alertlog.avro"), alertlog);
    }

    /*
     * shutdown the cluster
     */
    @AfterClass
    public static void teardownAfterClass() throws Exception {
         cluster.shutdown();
    }

    /*
     * test set for fs.defaultFS
     */ 
    @Test
    public void test_should_able_to_connect_to_hdfs() throws URISyntaxException, IOException {
        Configuration conf = new Configuration();
        URI inputURI = new URI("gphdfs://localhost:9000/test.txt");

        ConnectorUtil.setHadoopFSURI(conf, inputURI, "cdh4.1");

        assertEquals("hdfs://localhost:9000", conf.get("fs.defaultFS"));
    }

    /*
     * make sure all the test files are already in hadoop
     */ 
    @Test
    public void test_list_file() throws FileNotFoundException, IllegalArgumentException, IOException {
 		ArrayList<DataNode> dns = cluster.getDataNodes();

		assertEquals(dns.size(), 1);

		int fileNum = 0;
		RemoteIterator<LocatedFileStatus> fsIterator= cluster.getFileSystem().listFiles(new Path("/"), true);
 		while(fsIterator.hasNext()){
			fileNum ++;
			System.out.println(fsIterator.next().getPath());
		}

		assertEquals(fileNum, 2);
    }

    /*
     * test unicode support
     */ 
    @Test
    public void test_unicode() {
		int hadoopPort = cluster.getNameNodePort();

		Configuration conf = new Configuration();
		conf.addResource("hdfs-site.xml");

		try {
			URI uri = new URI("gphdfs://localhost:" + hadoopPort + "/tmp/alertlog.avro");
			ConnectorUtil.setHadoopFSURI(conf, uri, "gphdfs");

			ByteArrayOutputStream bout = new ByteArrayOutputStream();
			AvroFileReader aReader = new AvroFileReader(conf, 0, 1, uri.getPath(), null, null, false, false, bout);

			aReader.readAvroFormat();

			byte[] barray = bout.toByteArray();

			int line = 0;
			DataInputStream din = new DataInputStream(new ByteArrayInputStream(barray));
			while(din.available() != 0) {
				GPDBWritable writable = new GPDBWritable();
				writable.readFields(din);
				line++;
			}

			assertEquals(line, 1943);
		} catch (IOException e) {
			fail(e.getMessage());
		} catch (URISyntaxException e) {
			fail(e.getMessage());
		}
    }

    /*
     * test support for decimal in parquet file generated by hive
     */ 
    @Test
    public void test_hive_parquet_decimal() {
		Configuration conf = new Configuration();

		try {
			int hadoopPort = cluster.getNameNodePort();
			URI uri = new URI("gphdfs://localhost:" + hadoopPort + "/tmp/decimal.pq");
			ConnectorUtil.setHadoopFSURI(conf, uri, "gphdfs");
			String inputPath = uri.getPath();

			ByteArrayOutputStream bout = new ByteArrayOutputStream();
			GpdbParquetFileReader pReader = new GpdbParquetFileReader(conf, 0, 1, inputPath, null, false, false, bout);

			pReader.readParquetFormat();
			byte[] barray = bout.toByteArray();

			//this is the standard result which gphdfs writes to gpdb
			byte[] expect = {0, 0, 0, 32, 0, 1, 0, 2, 7, 7, 0, 0, 0, 0, 0, 7, 49, 50, 51, 46, 50, 49, 0, 0, 0, 0, 0, 4, 51, 46, 49, 0};
			for (int i = 0; i < barray.length; i++) {
				assertEquals(barray[i], expect[i]);
			}

			DataInputStream din = new DataInputStream(new ByteArrayInputStream(barray));
			GPDBWritable writable = new GPDBWritable();
			writable.readFields(din);
			String c1 = writable.getString(0);
			String c2 = writable.getString(1);
			assertEquals(c1, "123.21");
			assertEquals(c2, "3.1");

			assertEquals(din.available(), 0);
		} catch (IOException e) {
			fail(e.getMessage());
		} catch (URISyntaxException e) {
			fail(e.getMessage());
		}
    }

//    @Test
    public void test_should_able_to_connect_to_hdfs_with_ha() throws URISyntaxException {
        Configuration conf = new Configuration();
        URI inputURI = new URI("gphdfs://nameservice1/test.txt");

        ConnectorUtil.setHadoopFSURI(conf, inputURI, "cdh4.1");

        assertEquals("hdfs://nameservice1", conf.get("fs.defaultFS"));
    }

}
