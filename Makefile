
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

.PHONY: test
test: all
	@make -C tests all

.PHONY: clean-test
clean-test:
	@make -C tests clean

.PHONY: realclean
realclean: 
	git clean -fdx

libzebu.so: $(OBJECTS)

