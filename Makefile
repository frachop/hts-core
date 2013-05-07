#master makefile

all:
	make all -C src
	
clean:
	make clean -C src

distclean:
	make distclean -C src
	