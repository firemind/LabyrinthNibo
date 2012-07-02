
TARGETS = libnibo.a \
          libnibo2.a \
          libnds3.a \
          libuart-0.a \
          libuart-1.a \
          libxbee-0.a \
          libxbee-1.a \
 

all: $(TARGETS)

libnibo.a:
	make -f Makefile.nibo1

libnibo2.a:
	make -f Makefile.nibo2


libnds3.a:
	make -f Makefile.nds3

libuart-0.a:
	make -f Makefile.uart-0

libuart-1.a:
	make -f Makefile.uart-1

libxbee-0.a:
	make -f Makefile.xbee-0

libxbee-1.a:
	make -f Makefile.xbee-1


clean:
	rm -rf obj *~ $(TARGETS)


.PHONY: clean all

