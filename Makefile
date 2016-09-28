
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
realclean: clean clean-test

.PHONY: html
html: index.html

.PHONY: pdf
pdf: index.pdf

index.html: index.asc zebu.asc
	$(QUIET_GEN)asciidoctor index.asc

index.pdf: index.asc zebu.asc
	$(QUIET_GEN)asciidoctor-pdf index.asc

zebu.asc: zebu.h generate-apidoc
	$(QUIET_GEN)./generate-apidoc zebu.h > zebu.asc

libzebu.so: $(OBJECTS)

