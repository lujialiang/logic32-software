
all:
	make -C l32-asm
	make -C l32-sim
	make -C l32-link
	make -C l32-ar
	make -C lcc32

clean:
	make -C l32-asm distclean
	make -C l32-sim distclean
	make -C l32-link distclean
	make -C l32-ar distclean
	make -C lcc32 clean
	make -C program clean
	$(RM) bin/*

.PHONY: all clean
