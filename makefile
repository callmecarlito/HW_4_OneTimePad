#compiler: 
CC = gcc 
#compiler flags:
#-g -ansi -Wall - pedantic -std=gnu99 -I -Wno-missing-braces
CFLAGS = -g -Wall -I -std=gnu99 

all: keygen otp_enc_d

keygen: keygen.c
	$(CC) -o keygen keygen.c $(CFLAGS)

otp_end_d: otp_enc_d.c network_helpers.c
	$(CC) -o otp_end_d otp_end_d.c network_helpers.c $(CFLAGS)

clean:
	$(RM) keygen otp_enc_d *.o *~