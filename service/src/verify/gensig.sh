#!/bin/bash
set -e

PRIVATE_KEY=private.pem
PUBLIC_KEY=public.pem
PUBLIC_KEY_DER=public.der
MESSAGE=message.txt
BADMESSAGE=message.txt.bad
SIGNATURE=${MESSAGE}.sig

echo "[*] Generating private key"
openssl genrsa -out ${PRIVATE_KEY}

echo "[*] Generating public key"
openssl rsa -in ${PRIVATE_KEY} -pubout -out ${PUBLIC_KEY}
openssl rsa -pubin -in ${PUBLIC_KEY} -outform der -out ${PUBLIC_KEY_DER}

echo "[*] Generating message"
echo "the battle starts at dawn" > ${MESSAGE}
echo "THE BATTLE STARTS AT DAWN" > ${BADMESSAGE}

echo "[*] Generating signature"
openssl dgst -sha256 -sign ${PRIVATE_KEY} -out ${SIGNATURE} ${MESSAGE}

xxd -i ${PUBLIC_KEY_DER} > ${PUBLIC_KEY_DER}.h
xxd -i ${SIGNATURE} > ${SIGNATURE}.h
xxd -i ${MESSAGE} > ${MESSAGE}.h
xxd -i ${BADMESSAGE} > ${BADMESSAGE}.h
