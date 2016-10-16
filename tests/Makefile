
include ../config.mk

OBJS += alloc.o
OBJS += build.o
OBJS += error.o
OBJS += list.o
OBJS += location.o
OBJS += print.o
OBJS += string.o
OBJS += tree.o

PROGRAMS = $(OBJS:.o:)
DEPS = $(OBJS:.o=.d)
LOGS = $(OBJS:.o=.log)

.PHONY: all
all: $(LOGS)
	@for i in $(LOGS); do echo TEST $$i; diff -q $$i $$i.gold; done

.PHONY: clean
clean:
	$(RM) $(PROGRAMS)
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(LOGS)

alloc: alloc.o ../zebu.o
build: build.o ../zebu.o
error: error.o ../zebu.o
list: list.o ../zebu.o
location: location.o ../zebu.o
print: print.o ../zebu.o
string: string.o ../zebu.o
tree: tree.o ../zebu.o

%.log: %
	$(QUIET_GEN)./$< > $@ 2>&1

