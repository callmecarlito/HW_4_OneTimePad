#compiler: 
CC = gcc 
#compiler flags:
#-g -ansi -Wall - pedantic -std=gnu99 -I -Wno-missing-braces
CFLAGS = -g -Wall -I -std=gnu99 -Wno-missing-braces

all: keygen otp_enc_d otp_enc

keygen: keygen.c
	$(CC) -o keygen keygen.c $(CFLAGS)

otp_enc_d: otp_enc_d.c network_helpers.c
	$(CC) -o otp_enc_d otp_enc_d.c network_helpers.c $(CFLAGS)

otp_enc: otp_enc.c network_helpers.c
	$(CC) -o otp_enc otp_enc.c network_helpers.c $(CFLAGS)

otp_dec_d: otp_dec_d.c network_helpers.c
	$(CC) -o otp_dec_d otp_dec_d.c network_helpers.c $(CFLAGS)

otp_dec: otp_edec.c network_helpers.c
	$(CC) -o otp_dec otp_dec.c network_helpers.c $(CFLAGS)

clean:
	$(RM) keygen otp_enc_d otp_enc otp_dec_d otp_dec *.o *~