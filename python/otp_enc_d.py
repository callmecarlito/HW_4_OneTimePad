#!/usr/bin/env python3
import socket
import sys
import time

HEADERSIZE = 10

#HOST = '127.0.0.1'  #standard loopback interface address (localhost)
HOST = socket.gethostname()
PORT = 5757        #Port to listen on 

s = None
#set up address for server
for res in socket.getaddrinfo(HOST, PORT, socket.AF_INET, socket.SOCK_STREAM, 
                              0, socket.AI_PASSIVE):
    af, socktype, proto, canonname, servaddr = res
    try:
        #create socket object using internet address family IPv4, SOCK_STREAM = TCP socket type
        s = socket.socket(af, socktype, proto)
    except OSError as msg:
        s = None
        continue
    try: 
        s.bind(servaddr) #binds socket to address
        s.listen(5) #establishes listening socket - limited to 5 connect requests
    except OSError as msg:
        s.close()
        s = None
        continue
    break

if s is None:
    print("unable to open socket")
    sys.exit(1)

while True:
    clientsocket, clientaddr = s.accept() #accept incoming connections
    with clientsocket:
        print(f"connection from {clientaddr} has been established")
        
        msg = "Welcome to the server!"
        # :<10 is the header length, left aligned(<)
        print(f'{len(msg):<{HEADERSIZE}}' + msg)
        # curly brackets allow us to embed a varialbe within another variable
        msg = f'{len(msg):<{HEADERSIZE}}' + msg
        
        clientsocket.send(bytes(msg, "utf-8"))
        
        while True:
            time.sleep(3)
            msg = f'The time is: {time.time()}'
            msg = f'{len(msg):<{HEADERSIZE}}' + msg
            clientsocket.send(bytes(msg, "utf-8"))