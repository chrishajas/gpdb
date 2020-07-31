#!/usr/bin/python2

import optparse
import os
import shutil
import stat
import subprocess
import sys

from builds.GpBuild import GpBuild

INSTALL_DIR = "/usr/local/gpdb"

def print_compiler_version():
    status = subprocess.call(["g++", "--version"])
    if status:
        return status
    return subprocess.call(["gcc", "--version"])


def create_gpadmin_user():
    status = subprocess.call("gpdb_src/concourse/scripts/setup_gpadmin_user.bash")
    os.chmod('/bin/ping', os.stat('/bin/ping').st_mode | stat.S_ISUID)
    if status:
        return status

def tar_explain_output():
    status = subprocess.call(["tar", "czvf", "icg_output/explain_ouput.tar.gz", "out/"])
    return status

def copy_output():
    for dirpath, dirs, diff_files in os.walk('gpdb_src/'):
        if 'regression.diffs' in diff_files:
            diff_file = dirpath + '/' + 'regression.diffs'
            print("======================================================================\n" +
                  "DIFF FILE: " + diff_file + "\n" +
                  "----------------------------------------------------------------------")
            with open(diff_file, 'r') as fin:
                print fin.read()
    shutil.copyfile("gpdb_src/src/test/regress/regression.diffs", "icg_output/regression.diffs")
    shutil.copyfile("gpdb_src/src/test/regress/regression.out", "icg_output/regression.out")


def fail_on_error(status):
    if status:
        sys.exit(status)


def main():
    parser = optparse.OptionParser()
    parser.add_option("--mode", choices=['orca', 'planner'])
    parser.add_option("--output_dir", dest="output_dir", default=INSTALL_DIR)
    parser.add_option("--configure-option", dest="configure_option", action="append",
                      help="Configure flags, ex --configure_option=--disable-orca --configure_option=--disable-gpcloud")
    parser.add_option("--action", choices=['build', 'test', 'test_explain_suite'], dest="action", default='build',
                      help="Build GPDB or Run Install Check")
    parser.add_option("--dbexists", dest="dbexists", action="store_true", default=False, help="create new demo cluster")
    (options, args) = parser.parse_args()

    (options, args) = parser.parse_args()

    gpBuild = GpBuild(options.mode)
    status = print_compiler_version()
    fail_on_error(status)

    status = gpBuild.install_dependency("bin_gpdb", INSTALL_DIR)
    fail_on_error(status)

    status = create_gpadmin_user()
    fail_on_error(status)
    status = gpBuild.run_explain_test_suite(options.dbexists)
    fail_on_error(status)
    status = tar_explain_output()
    fail_on_error(status)
    return 0


if __name__ == "__main__":
    sys.exit(main())
