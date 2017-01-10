#!/bin/bash -l

set -exo pipefail

CWDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source "${CWDIR}/common.bash"

function gen_env(){
	cat > ~/run_unit_tests.sh <<-EOF
	set -exo pipefail

	cd "\${1}/gpdb_src/gpAux/extensions/gps3ext"
	make test
	EOF

	chmod a+x ~/run_unit_tests.sh
}

function run_unit_tests() {
	bash -e ~/run_unit_tests.sh $(pwd)
}

function _main() {
	time gen_env

	time run_unit_tests
}

_main "$@"
