#!/usr/bin/env python3
import argparse

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('infile')
    ap.add_argument('signature')
    ap.add_argument('outfile')
    args = ap.parse_args()

    d = open(args.infile, 'rb').read()
    sig = open(args.signature, 'rb').read()
    print(len(sig))
    assert(len(sig) == 256)
    d = sig + d

    with open(args.outfile, 'wb') as f:
        payload_len = len(d)
        f.write(payload_len.to_bytes(4, 'little'))
        # FIXME: Add signature, etc
        f.write(d)

if __name__ == '__main__':
    main()
