#!/usr/bin/env python3

from pwn import *
import argparse
from itertools import product
from random import randrange

# context.log_level = 'debug'

PROMPT = b'barbOS> '

def cmd(s, b:bytes):
    s.send(b)
    r = s.recvuntil(PROMPT, timeout=2)
    if r.endswith(PROMPT):
        return r[0:-len(PROMPT)]

def check_info(s):
    m = cmd(s, b'INFO\n')
    assert((b'DEBUG: barbOS v0.0 : num_devices 3') in m)



def check_therm(s):
    days = ("sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday")
    times = ("day", "night")
    temps = []

    for d,t in product(days,times):
        temps.append(randrange(60,90))
        r = cmd(s, f'THERM set {t} {d} {temps[-1]}\n'.encode('utf-8'))
        assert(f'DEBUG: set thermostat to {temps[-1]}\n'.encode('utf-8') in r)

    for i,(d,t) in enumerate(product(days,times)):
        r = cmd(s, f'THERM read {t} {d}\n'.encode('utf-8'))
        assert(f'{t} {temps[i]}\n'.encode('utf-8') in r)

def check_alarm(s):
    cmd(s, b'ALARM arm\n')
    assert(b'armed' in cmd(s, b'ALARM armed?\n'))
    assert(b'alarm disarmed' in cmd(s, b'ALARM disarm 1 3 3 7\n'))

def check_speaker(s):
    song_title = 'ooo.mp3'
    r = cmd(s, f'SPEAKER queue {song_title}\n'.encode('utf-8'))
    assert(f'playing {song_title} next, there are'.encode('utf-8') in r)
    #assert(r.startswith(f'playing {song_title} next, there are'.encode('utf-8')))

    r = cmd(s, f'SPEAKER review 5 {song_title}\n'.encode('utf-8'))
    assert(f'gave review of 5'.encode('utf-8') in r)
    #assert(r.startswith(f'gave review of 5'.encode('utf-8')))

    r = cmd(s, f'SPEAKER play\n'.encode('utf-8'))
    assert(f'now playing {song_title}'.encode('utf-8') in r)
    #assert(r.startswith(f'now playing {song_title}'.encode('utf-8')))

def check1(host, port):
    os.system("python3 /wsprox.py " + host + " " + str(port) + " >/dev/null &")
    sleep(1)
    s = remote("localhost", 1338)
    s.recvuntil(PROMPT)
    check_info(s)
    check_therm(s)
    check_alarm(s)
    check_speaker(s)
    s.close()

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('host')
    ap.add_argument('port', type=int)
    args = ap.parse_args()
    check1(args.host, args.port)

if __name__ == '__main__':
    main()
