
include ../config.mk

DIFF = diff -q --ignore-blank-lines --ignore-space-change
MEMCHECK = valgrind -q --tool=memcheck

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
	@for i in $(LOGS); do echo TEST $$i; $(DIFF) $$i $$i.gold; done

.PHONY: clean
clean:
	$(RM) $(PROGRAMS)
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(LOGS)

alloc: alloc.o ../src/libzebu.a
build: build.o ../src/libzebu.a
error: error.o ../src/libzebu.a
list: list.o ../src/libzebu.a
location: location.o ../src/libzebu.a
print: print.o ../src/libzebu.a
string: string.o ../src/libzebu.a
tree: tree.o ../src/libzebu.a

../src/libzebu.a:
	make -C ../src libzebu.a

%.log: % make-always
	$(QUIET_GEN)$(MEMCHECK) ./$< > $@ 2>&1

.PHONY: make-always
make-always:

