all:
	mkoctfile --link-stand-alone -v embedded.cc

run:
	./a.out
