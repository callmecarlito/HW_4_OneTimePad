#!/usr/bin/env python3
import socket
import sys

HEADERSIZE = 10

HOST = socket.gethostname()
PORT = 5757

s = None
#set up address for server
for res in socket.getaddrinfo(HOST, PORT, socket.AF_INET, socket.SOCK_STREAM):
    af, socketype, proto, canonname, servaddr = res
    try:
        #create socket object using internet address family IPv4, SOCK_STREAM = TCP socket type
        s = socket.socket(af, socketype, proto)
    except OSError as msg:
        s = None
        continue
    try: 
        #connect to server
        s.connect(servaddr)
    except OSError as msg:
        s.close()
        s = None
        continue
    break

if s is None:
    print("could not open socket")
    sys.exit(1)

with s:
    while True:
            
        full_msg = ''
        new_msg = True

        while True:
            msg = s.recv(16) #receives 16 bytes of data at a time
            
            if new_msg:
                print(f"new message length: {msg[:HEADERSIZE]}")
                msg_len = int(msg[:HEADERSIZE])
                new_msg = False

            full_msg += msg.decode("utf-8")

            if (len(full_msg) - HEADERSIZE) == msg_len:
                print("Full message received.")
                print(full_msg[HEADERSIZE:])
                new_msg = True
                full_msg = ''