import Crypto.Util.number
from Crypto.PublicKey import RSA
import gensafeprime
import gmpy

def generate_keys():
    #recipient_key = RSA.import_key(open("private.pem").read())
    #e = recipient_key.e
    #p = recipient_key.p
    #q = recipient_key.q
    e = 3
    p = gensafeprime.generate(1024)
    q = gensafeprime.generate(1024)
    n = p*q
    et = (p-1)*(q-1)
    d = Crypto.Util.number.inverse(e,et)
    return(d,e,n)

def rsa(msg):
    d,e,n = generate_keys()
    m = Crypto.Util.number.bytes_to_long(msg)

    c = pow(m,e,n)
    decrypt_msg = pow(c,d,n)
    dec = Crypto.Util.number.long_to_bytes(decrypt_msg)
    print(dec)
    return(c,n)


