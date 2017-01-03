"""
Copyright (C) 2004-2015 Pivotal Software, Inc. All rights reserved.

This program and the accompanying materials are made available under
the terms of the under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

from gppylib.commands.base import Command
import tinctest
from mpp.lib.PSQL import PSQL
from tinctest.lib import Gpdiff
from tinctest.lib.system import TINCSystem
import os
import sys
import unittest2 as unittest
from time import sleep
from mpp.gpdb.tests.utilities.upgrade.gpdb_upgrade import UpgradeTestCase
from tinctest.models.scenario import ScenarioTestCase
from mpp.gpdb.tests.storage.lib.gp_filedump import GpfileTestCase


class DeltaUpgradeTestCase(ScenarioTestCase, UpgradeTestCase):
    ''' 
    Additonal upgrade and backup/restore
    tests for delta compression feature.
    @product_version gpdb: [4.3.3.0-]
    '''
    
    def setUp(self):
        self.upgrade_from = os.environ.get('UPGRADE_FROM', '4.2.6.0 2 sp')
        self.binary_swap_from = os.environ.get('BINARY_SWAP_FROM', '4.3.2.1 2 rc')
        if 'binary_swap' in self.test_method:
            (self.old_gpdb, self.new_gpdb) = self.get_gpdbpath_info(self.binary_swap_from)
        else:
            (self.old_gpdb, self.new_gpdb) = self.get_gpdbpath_info(self.upgrade_from)
        self.db_port = self.get_master_port()

        
    def tearDown(self):
        self.cleanup_upgrade(self.old_gpdb, self.new_gpdb, self.db_port, self.test_method)
        
    def test_upgrade_42_to_43(self):

        test_case_list0 = []
        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.install_GPDB', [self.upgrade_from]))
        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.setup_upgrade', {'old_version':self.upgrade_from, 'master_port': self.db_port, 'mirror_enabled' : True}))

        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.run_workload', ['test_delta'], {'db_port': self.db_port}))

        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.run_migrator', [self.old_gpdb, self.new_gpdb, self.db_port], {'mirror_enabled' : True}))
        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.validate_upgrade_correctness', [self.new_gpdb, self.db_port, self.test_method]))

        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.validate_workload', ['test_delta'], {'db_port': self.db_port}))

        test_case_list0.append(('mpp.gpdb.tests.utilities.upgrade.UpgradeHelperClass.check_gpfiledump', [self.old_gpdb, self.new_gpdb, self.db_port]))
        self.test_case_scenario.append(test_case_list0, serial=True)


    def test_binary_swap_432_to_43(self):
        
        self.binary_swap_from = '4.3.2.0 1 sp'
        (self.old_gpdb, self.new_gpdb) = self.get_gpdbpath_info(self.binary_swap_from)

        test_case_list0 = []
        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.install_GPDB', [self.binary_swap_from]))

        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.setup_upgrade', {'old_version':self.binary_swap_from, 'master_port': self.db_port, 'mirror_enabled' : True}))
        
        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.run_workload', ['test_delta'], {'db_port': self.db_port}))
        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.create_filespaces', [self.db_port]))
        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.run_workload', ['test_objects'], {'db_port': self.db_port}))
        
        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.run_swap',[self.old_gpdb, self.new_gpdb, self.db_port]))
        
        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.validate_workload', ['test_delta'], {'db_port': self.db_port}))

        test_case_list0.append(('mpp.gpdb.tests.utilities.upgrade.UpgradeHelperClass.check_gpfiledump', [self.old_gpdb, self.new_gpdb, self.db_port]))
        self.test_case_scenario.append(test_case_list0, serial=True)

    def test_binary_swap_43_to_432(self):


        self.binary_swap_to = '4.3.2.0 1 sp'
        (self.old_gpdb, self.new_gpdb) = self.get_gpdbpath_info(self.binary_swap_to)

        test_case_list0 = []
        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.install_GPDB', [self.binary_swap_to]))

        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.setup_upgrade', { 'old_version':self.binary_swap_to, 'master_port': self.db_port, 'mirror_enabled' : True, 'fresh_db': True}))

        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.run_workload', ['test_delta'], {'db_port': self.db_port}))
        
        mdd = self.upgrade_home + '/gpdb_%s/' % self.get_product_version()[1] + 'master/gpseg-1'
        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.run_swap',[ self.new_gpdb, self.old_gpdb, self.db_port], {'mdd':mdd, 'swap_back' : True}))

        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.validate_workload', ['test_delta_back'], {'db_port': self.db_port}))
        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.cleanup_upgrade', [self.new_gpdb, self.old_gpdb, self.db_port, self.test_method], {'fresh_db' : True}))
        self.test_case_scenario.append(test_case_list0, serial=True)


    def backup_432_restore_43(self, checksum=False):

        self.binary_swap_from = '4.3.2.0 1 sp'
        (self.old_gpdb, self.new_gpdb) = self.get_gpdbpath_info(self.binary_swap_from)

        test_case_list0 = []
        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.install_GPDB', [self.binary_swap_from]))

        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.setup_upgrade', {'old_version':self.binary_swap_from, 'master_port': self.db_port, 'mirror_enabled' : True}))

        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.run_workload', ['test_delta'], {'db_port': self.db_port}))

        test_case_list0.append(('mpp.gpdb.tests.utilities.upgrade.UpgradeHelperClass.backup_db',[self.old_gpdb,self.db_port, 'dldb']))

        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.run_swap',[self.old_gpdb, self.new_gpdb, self.db_port]))

        test_case_list0.append(('mpp.gpdb.tests.utilities.upgrade.UpgradeHelperClass.restore_db',[self.old_gpdb, self.new_gpdb, self.db_port, 'dldb']))

        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.validate_workload', ['test_delta'], {'db_port': self.db_port}))

        test_case_list0.append(('mpp.gpdb.tests.utilities.upgrade.UpgradeHelperClass.check_gpfiledump', [self.old_gpdb, self.new_gpdb, self.db_port,checksum]))
        self.test_case_scenario.append(test_case_list0, serial=True)
    
    def test_backup_432_restore_43(self):
        '''
        @product_version gpdb: [4.3.3.0-4.3.3.99]
        '''
        self.backup_432_restore_43()

    def test_backup_432_restore_43(self):
        '''
        @product_version gpdb: [4.3.4.0-]
        '''
        self.backup_432_restore_43(checksum=True)

    def test_backup_43_restore_432(self):

        self.binary_swap_to = '4.3.2.0 1 sp'
        (self.old_gpdb, self.new_gpdb) = self.get_gpdbpath_info(self.binary_swap_to)

        test_case_list0 = []
        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.install_GPDB', [self.binary_swap_to]))

        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.setup_upgrade', {'old_version':self.binary_swap_to, 'master_port': self.db_port, 'mirror_enabled' : True, 'fresh_db': True}))

        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.run_workload', ['test_delta'], {'db_port': self.db_port}))
        
        mdd = self.upgrade_home + '/gpdb_%s/' % self.get_product_version()[1] + 'master/gpseg-1'
        test_case_list0.append(('mpp.gpdb.tests.utilities.upgrade.UpgradeHelperClass.backup_db',[self.new_gpdb,self.db_port, 'dldb'], {'new_gpdb': self.new_gpdb,'mdd': mdd}))

        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.run_swap',[self.new_gpdb, self.old_gpdb, self.db_port], {'mdd':mdd, 'swap_back' : True}))

        test_case_list0.append(('mpp.gpdb.tests.utilities.upgrade.UpgradeHelperClass.restore_db',[self.new_gpdb, self.old_gpdb + '/greenplum-db', self.db_port, 'dldb'], {'mdd': mdd}))

        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.validate_workload', ['test_delta'], {'db_port': self.db_port}))
        test_case_list0.append(('.gpdb_upgrade.UpgradeTestCase.cleanup_upgrade', [self.new_gpdb, self.old_gpdb, self.db_port, self.test_method], {'fresh_db' : True}))
        self.test_case_scenario.append(test_case_list0, serial=True)

