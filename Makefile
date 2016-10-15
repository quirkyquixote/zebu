
include config.mk

OBJECTS = zebu.o
LIBS = libzebu.so
INSTALL_LIBS = $(addprefix $(libdir)/,$(LIBS))

.PHONY: all
all: $(LIBS)

.PHONY: clean
clean:
	@$(RM) $(OBJECTS)
	@$(RM) $(LIBS)

.PHONY: install
install: all $(INSTALL_LIBS)

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
realclean: 
	git clean -fdx

libzebu.so: $(OBJECTS)

