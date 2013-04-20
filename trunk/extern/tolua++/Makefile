# makefile for tolua hierarchy

OBJS=src/lib/tolua_event.o src/lib/tolua_is.o src/lib/tolua_map.o src/lib/tolua_push.o src/lib/tolua_to.o

all:
	scons
	gcc -shared -Wl,-E -fPIC -Wl,-rpath,'$${ORIGIN}' $(OBJS) -o ../bin/libtolua++.so
	cp bin/tolua++5.1 ../bin/tolua++

clean:
	scons --clean
	rm -f ../bin/libtolua++.so
	rm -f ../bin/tolua++
