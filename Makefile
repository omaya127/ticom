all:
	gcc -o ticom ticom.c keymap.c term.c

clean:
	rm -rf test ticom
