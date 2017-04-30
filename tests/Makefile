
include ../config.mk

DIFF = diff -q --ignore-blank-lines --ignore-space-change
MEMCHECK = valgrind -q --tool=memcheck

objs += list.o
objs += dict.o
objs += alloc.o
objs += build.o
objs += data.o
objs += error.o
objs += location.o
objs += print.o
objs += tree.o

bins = $(objs:.o=)
deps = $(objs:.o=.d)
logs = $(objs:.o=.log)

.PHONY: all
all: $(logs)
	@for i in $(bins); do echo DIFF $$i.log; $(DIFF) $$i.log $$i.gold; done
	@for i in $(bins); do echo MEMCHECK $$i; $(MEMCHECK) ./$$i > /dev/null 2>&1; done

.PHONY: clean
clean:
	$(RM) $(bins)
	$(RM) $(objs)
	$(RM) $(deps)
	$(RM) $(logs)

alloc: alloc.o ../src/libzebu.a
build: build.o ../src/libzebu.a
data: data.o ../src/libzebu.a
dict: dict.o ../src/libzebu.a
error: error.o ../src/libzebu.a
list: list.o ../src/libzebu.a
location: location.o ../src/libzebu.a
print: print.o ../src/libzebu.a
string: string.o ../src/libzebu.a
tree: tree.o ../src/libzebu.a

../src/libzebu.a:
	make -C ../src libzebu.a

%.log: % make-always
	$(QUIET_GEN)./$< > $@ 2>&1

.PHONY: make-always
make-always:

-include $(deps)
