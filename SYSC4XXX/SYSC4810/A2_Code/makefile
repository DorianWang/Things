CFLAGS = -lcrypto -O2
CPPFLAGS =

.PHONY: all all_dec clean

all_dec:
	$(MAKE) all CPPFLAGS=-DDEC_PRINT

all: problem3 problem4 problem5 problem6 problem7


problem3: problem3.c 
	$(CC) -o $@ $^ $(CFLAGS) $(CPPFLAGS)
problem4: problem4.c 
	$(CC) -o $@ $^ $(CFLAGS) $(CPPFLAGS)
problem5: problem5.c 
	$(CC) -o $@ $^ $(CFLAGS) $(CPPFLAGS)
problem6: problem6.c 
	$(CC) -o $@ $^ $(CFLAGS) $(CPPFLAGS)
problem7: problem7.c 
	$(CC) -o $@ $^ $(CFLAGS) $(CPPFLAGS)

clean:
	rm problem3 problem4 problem5 problem6 problem7

