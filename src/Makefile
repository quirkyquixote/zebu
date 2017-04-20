
include ../config.mk

objs += dict.o
objs += tree.o
objs += print.o

deps = $(objs:.o=.d)

libs = libzebu.so libzebu.a
install_libs = $(addprefix $(libdir)/,$(libs))

.PHONY: all
all: $(libs) $(bins)

.PHONY: clean
clean:
	$(RM) $(objs)
	$(RM) $(libs)
	$(RM) $(deps)

.PHONY: install
install: all $(install_libs)

libzebu.so: $(objs)
libzebu.a: $(objs)

-include $(deps)
