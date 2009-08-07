

all:
	cd libs/basekit; make
	cd libs/jfile; make
	cd libs/jfile/tests; make
	cd libs/udb; make
	cd libs/skipdb; make
	#cd libs/tagdb; make

clean:
	cd libs/basekit; make clean
	cd libs/jfile; make clean
	cd libs/jfile/tests; make clean
	cd libs/udb; make clean
	cd libs/skipdb; make clean
	#cd libs/tagdb; make clean

test:
	cd libs/jfile/tests; ./test
