all:
	gcc -o ticom ticom.c
test:
	gcc -o test test.c parse.c
clean:
	rm -rf test ticom
