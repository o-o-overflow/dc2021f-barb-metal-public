#!/bin/bash
set -e

source gensig.sh

echo "[*] Testing"
echo "----------------------------------------------------------"

function check_cond {
	local ACTUAL=$1
	local EXPECTED=$2

	if [[ $ACTUAL == $EXPECTED ]]; then
		echo "Test OK"
	else
		echo "Test FAIL"
		exit 1
	fi

	echo "----------------------------------------------------------"
}

set +e

./verify ${PUBLIC_KEY_DER} ${SIGNATURE} ${MESSAGE}
check_cond $? 0

./verify ${PUBLIC_KEY_DER} ${SIGNATURE} ${BADMESSAGE}
check_cond $? 1
