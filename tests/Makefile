
include ../config.mk

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
	@$(RM) *.o
	@$(RM) $(PROGRAMS)

print: print.o ../zebu.o
	$(call colorecho,Linking C executable $@)
	@$(CC) $(ALL_CFLAGS) $(ALL_LDFLAGS) -o $@ $^

tree: tree.o ../zebu.o
	$(call colorecho,Linking C executable $@)
	@$(CC) $(ALL_CFLAGS) $(ALL_LDFLAGS) -o $@ $^

walk: walk.o ../zebu.o
	$(call colorecho,Linking C executable $@)
	@$(CC) $(ALL_CFLAGS) $(ALL_LDFLAGS) -o $@ $^

test_%: %
	$(call colorecho,Test $<)
	@./$<

