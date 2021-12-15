all:read.c write.c cs.c
	gcc -c -s -oo cs.c -o cs.o
	gcc -s -oo read.c cs.o -o consumer.exe
	gcc -s -oo write.c cs.o -o producer.exe

clean:
	rm *.o *.exe