
all:
	cc chipmunk_demo.c `simple2d --libs` -lchipmunk -o chipmunk_demo
	./chipmunk_demo
