#!/usr/bin/env python3
import socket

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
    clientsocket.send(bytes("Welcome to the server!", "utf-8"))

