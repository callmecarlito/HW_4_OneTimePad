import socket

HEADERSIZE = 10

HOST = socket.gethostname()
PORT = 5757

#create socket object using internet address family IPv4, SOCK_STREAM = TCP socket type
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
s.connect((HOST, PORT))

while True:
        
    full_msg =''
    new_msg = True
    while True:
        msg = s.recv(16) #receives 16 bytes of data at a time
        if new_msg:
            print(f"new message length: {msg[:HEADERSIZE]}")
            msg_len = int(msg[:HEADERSIZE])
            new_msg = False
        full_msg += msg.decode("utf-8")
        if len(full_msg)-HEADERSIZE == msg_len:
            print("Full message received.")
            print(full_msg[HEADERSIZE:])
            new_msg = True
            full_msg = ''
    print(full_msg)