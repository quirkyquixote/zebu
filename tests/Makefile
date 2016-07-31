
include ../config.mk

OBJECTS += alloc.o
OBJECTS += print.o
OBJECTS += string.o
OBJECTS += tree.o
OBJECTS += walk.o

PROGRAMS += alloc
PROGRAMS += print
PROGRAMS += string
PROGRAMS += tree
PROGRAMS += walk

TESTS += test_alloc
TESTS += test_print
TESTS += test_string
TESTS += test_tree
TESTS += test_walk

.PHONY: build-test
build-test: $(PROGRAMS)

.PHONY: test
test: $(TESTS)

.PHONY: clean-test
clean-test:
	@$(RM) $(OBJECTS)
	@$(RM) $(PROGRAMS)

alloc: alloc.o ../zebu.o
print: print.o ../zebu.o
string: string.o ../zebu.o
tree: tree.o ../zebu.o
walk: walk.o ../zebu.o

test_%: %
	@$(call colorecho,TEST $<); ./$<

