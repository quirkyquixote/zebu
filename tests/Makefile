
include ../config.mk

OBJECTS += print.o
OBJECTS += tree.o
OBJECTS += walk.o

PROGRAMS += print
PROGRAMS += tree
PROGRAMS += walk

TESTS += test_print
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

print: print.o ../zebu.o
tree: tree.o ../zebu.o
walk: walk.o ../zebu.o

test_%: %
	@$(call colorecho,TEST $<); ./$<

