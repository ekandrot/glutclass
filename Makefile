.PHONY: all clean
all:
	cd test0;make
	cd test1;make
	cd test2;make
	cd test3;make
	cd test4;make

clean:
	cd test0;make clean
	cd test1;make clean
	cd test2;make clean
	cd test3;make clean
	cd test4;make clean

