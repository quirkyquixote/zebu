
include ../config.mk

OBJECTS += alloc.o
OBJECTS += build.o
OBJECTS += error.o
OBJECTS += list.o
OBJECTS += location.o
OBJECTS += print.o
OBJECTS += string.o
OBJECTS += tree.o
OBJECTS += walk.o

PROGRAMS += alloc
PROGRAMS += build
PROGRAMS += error
PROGRAMS += list
PROGRAMS += location
PROGRAMS += print
PROGRAMS += string
PROGRAMS += tree
PROGRAMS += walk

TESTS += test_alloc
TESTS += test_build
TESTS += test_error
TESTS += test_list
TESTS += test_location
TESTS += test_print
TESTS += test_string
TESTS += test_tree
TESTS += test_walk

# Required for the allocation test to compile
ALL_CFLAGS += -Wno-overlength-strings

.PHONY: build-test
build-test: $(PROGRAMS)

.PHONY: test
test: $(TESTS)

.PHONY: clean-test
clean-test:
	@$(RM) $(OBJECTS)
	@$(RM) $(PROGRAMS)

alloc: alloc.o ../zebu.o
build: build.o ../zebu.o
error: error.o ../zebu.o
list: list.o ../zebu.o
location: location.o ../zebu.o
print: print.o ../zebu.o
string: string.o ../zebu.o
tree: tree.o ../zebu.o
walk: walk.o ../zebu.o

test_%: %
	@$(call colorecho,TEST $<); ./$<

