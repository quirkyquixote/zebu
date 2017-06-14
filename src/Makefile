
include ../config.mk

objs += data.o
objs += dict.o
objs += tree.o
objs += print.o


deps = $(objs:.o=.d)

libs = libzebu.so libzebu.a
install_libs = $(addprefix $(libdir)/,$(libs))

headers += data.h
headers += dict.h
headers += list.h
headers += node.h
headers += print.h
headers += tree.h
headers += zebu.h

install_headers = $(addprefix $(includedir)/,$(headers))

.PHONY: all
all: $(libs) $(bins)

.PHONY: clean
clean:
	$(RM) $(objs)
	$(RM) $(libs)
	$(RM) $(deps)

.PHONY: install
install: all $(install_libs) $(install_headers)

libzebu.so: $(objs)
libzebu.a: $(objs)

-include $(deps)
