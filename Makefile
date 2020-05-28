all:
	gcc -o ticom ticom.c parse.c term.c

clean:
	rm -rf test ticom
