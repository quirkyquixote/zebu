
include config.mk

ZEBU_OBJECTS += zebu.o

.PHONY: all
all: libzebu.so

.PHONY: clean
clean:
	@$(RM) *.o
	@$(RM) libzebu.so

.PHONY: install
install: all
	$(call colorecho,Installing $(LIBDIR)/libzebu.so.$(VERSION))
	@$(INSTALL) libzebu.so $(LIBDIR)/libzebu.so.$(VERSION)
	$(call colorecho,Installing $(LIBDIR)/libzebu.so)
	@cd $(LIBDIR) && ln -s -f libzebu.so.$(VERSION) libzebu.so

.PHONY: build-test
build-test: all
	@make -C tests $@

.PHONY: test
test: build-test
	@make -C tests $@

.PHONY: clean-test
clean-test:
	@make -C tests $@

.PHONY: realclean
realclean: clean clean-test

libzebu.so: $(ZEBU_OBJECTS)
	$(call colorecho,Linking C shared library $@)
	@$(CC) -shared -Wl,-soname,libzebu.so.$(VERSION) -o libzebu.so $^

