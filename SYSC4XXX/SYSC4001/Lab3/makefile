CC=gcc

.PHONY: cleanobj clean fresh

lab3: random_threshold.c
	$(CC) -o $@ $^

fresh: clean all cleanobj

all: alarm sensor shm1 shm2 random_threshold lab3

alarm: alarm.c
	$(CC) -o $@ $^

sensor: sensor.c
	$(CC) -o $@ $^

shm1.o: shm1.c shm_com.h
	$(CC) -c $< -o $@

shm1: shm1.o
	$(CC) -o $@ $^

shm2.o: shm2.c shm_com.h
	$(CC) -c $< -o $@

shm2: shm2.o
	$(CC) -o $@ $^

random_threshold: random_threshold.c
	$(CC) -o $@ $^

cleanobj:
	rm -f *.o

clean:
	rm -f *.o sensor shm1 shm2 random_threshold *.exe

