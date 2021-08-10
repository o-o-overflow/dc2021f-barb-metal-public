from rsa import *
from hashlib import sha256
from Crypto.Util.number import long_to_bytes, bytes_to_long
import gmpy2
import sys

sha256_asn1 = b'\x30\x31\x30\x0d\x06\x09\x60\x86\x48\x01\x65\x03\x04\x02\x01\x05\x00\x04\x20'

# matches the c impl
def verify_sig(signature,message,pk):
    #generate clearsig
    e,n = pk
    #generate clearsignature
    clearsig = pow(signature,e,n)
    clearsig = b'\x00' + long_to_bytes(clearsig)


    # verify if the signature start with
    # 00 01 FF
    if clearsig[0:3] != b'\x00\x01\xff':
        raise ValueError("No sig marker")

    sep_index = clearsig.index(b'\x00',2)

    if not clearsig[sep_index+1:].startswith(sha256_asn1):
        raise ValueError("no asn1")

    signature_hash = clearsig[sep_index+len(sha256_asn1)+1:sep_index+len(sha256_asn1)+1+32]

    message_hash = sha256(message).digest()

    if message_hash != signature_hash:
        raise ValueError("hashes don't match")

    return True

def forgeSignature(msg,pk):
    e,n = pk

    #creating the hash of the message
    msg_hash = sha256(msg).digest()
    garbage = b'\x00'*201

    #appending garbage value to forge message we like
    forged_signature = b'\x00\x01\xff\x00' + sha256_asn1 + msg_hash + garbage

    forged_signature_num = bytes_to_long(forged_signature)
    (cube_root,exact) = gmpy2.iroot(forged_signature_num,3)
    cube_root += 1

    recovered = long_to_bytes(pow(int(cube_root),e,n))
    assert b"\x01\xff" in recovered
    assert sha256_asn1 in recovered
    assert msg_hash in recovered

    return int(cube_root)




def main():
    print("STARTING")
    d,e,n = generate_keys()
    print("generated")
    msg = open(sys.argv[1], "rb").read()
    private_keys = e,n

    forged_signature = forgeSignature(msg,private_keys)

    f = open('sig', 'w+b')
    binary_format = long_to_bytes(forged_signature)
    print(len(binary_format))
    f.write(binary_format)
    f.close()

if __name__ == '__main__':
    main()
