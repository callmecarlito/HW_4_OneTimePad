import socket

HOST = socket.gethostname()
PORT = 5757

#create socket object using internet address family IPv4, SOCK_STREAM = TCP socket type
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
s.connect((HOST, PORT))

msg = s.recv(1024) #receives 1024 bytes of data at a time
print(msg.decode("utf-8"))