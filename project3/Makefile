CC = gcc
CFLAGS = -g -w
DFLAGS = 
# DFLAGS = -D DEBUG

# TEST_FILE = samples/test-null.txt
# TEST_FILE = samples/test-a.txt
# TEST_FILE = samples/test-abc.txt
# TEST_FILE = samples/test-15.txt
# TEST_FILE = samples/test-16.txt
# TEST_FILE = samples/test-41.txt
# TEST_FILE = samples/poem.txt
# TEST_FILE = samples/alice.txt
# TEST_FILE = samples/clown.jpg
# TEST_FILE = test.txt

KEY_FILE = key.txt

PLATFORM = linux

all:: clean vencrypt vdecrypt scrypt sbencrypt sbdecrypt

vencrypt: vencrypt.c
	$(CC) $(CFLAGS) $(DFLAGS) -o vencrypt vencrypt.c

vdecrypt: vdecrypt.c
	$(CC) $(CFLAGS) $(DFLAGS) -o vdecrypt vdecrypt.c

scrypt: scrypt.c
	$(CC) $(CFLAGS) $(DFLAGS) -o scrypt scrypt.c

sbencrypt: sbencrypt.c
	$(CC) $(CFLAGS) $(DFLAGS) -o sbencrypt sbencrypt.c

sbdecrypt: sbdecrypt.c
	$(CC) $(CFLAGS) $(DFLAGS) -o sbdecrypt sbdecrypt.c

vencrypt_test:
	./vencrypt ${KEY_FILE} ${TEST_FILE} _encrypt
	${PLATFORM}/vencrypt ${KEY_FILE} ${TEST_FILE} _encrypt2
	./check _encrypt _encrypt2 vencrypt

vbdecrypt_test:
	./vdecrypt ${KEY_FILE} ${TEST_FILE} _decrypt
	${PLATFORM}/vdecrypt ${KEY_FILE} ${TEST_FILE} _decrypt2
	./check _decrypt _decrypt2 vdecrypt

scrypt_test:
	#macos/prand-test -p 123 -n 3
	./scrypt abc123

sbencrypt_test:

sbdecrypt_test:

# test: scrypt_test
# test: sbencrypt_test sbdecrypt_test
test: vencrypt_test vbdecrypt_test scrypt_test sbencrypt_test sbdecrypt_test

clean:
	rm -rf vencrypt vdecrypt scrypt sbencrypt sbdecrypt _encrypt _encrypt2 _decrypt _decrypt2