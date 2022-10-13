# Makefile
srg: srg.c WavUtil.c
	gcc srg.c WavUtil.c -o srg -lm
