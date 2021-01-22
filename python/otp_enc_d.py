#!/usr/bin/env python3
import socket

HEADERSIZE = 10
#HOST = '127.0.0.1'  #standard loopback interface address (localhost)
HOST = socket.gethostname()
PORT = 5757        #Port to listen on 

#create socket object using internet address family IPv4, SOCK_STREAM = TCP socket type
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
s.bind((HOST, PORT)) #binds socket to port
s.listen(5) #establishes listening socket - limited to 5 connect requests

while True:
    clientsocket, address = s.accept() #accept incoming connections
    print(f"Connection from {address} has been established!")
    
    msg = "Welcome to the server!"
    # :<10 is the header length, left aligned(<)
    print(f'{len(msg):<{HEADERSIZE}}' + msg)
    # curly brackets allow us to embed a varialbe within another variable
    msg = f'{len(msg):<{HEADERSIZE}}' + msg
     
    clientsocket.send(bytes(msg, "utf-8"))
    

