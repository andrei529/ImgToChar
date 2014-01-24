all: imgtochar

imgtochar: imgtochar.c
	gcc imgtochar.c -o imgtochar -lIL -lILU -lILUT

clean:
	rm imgtochar
