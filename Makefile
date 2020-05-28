all:
	gcc -o test test.c parse.c

clean:
	rm -rf test ticom
