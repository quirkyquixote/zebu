
.PHONY: all
all:
	@make -C src all

.PHONY: clean
clean:
	@make -C src clean

.PHONY: install
install:
	@make -C src install

.PHONY: test
test: all
	@make -C tests all

.PHONY: clean-test
clean-test:
	@make -C tests clean

.PHONY: apidoc
apidoc:
	doxygen Doxyfile

.PHONY: clean-apidoc
clean-apidoc:
	rm -rf apidoc

.PHONY: realclean
realclean: 
	git clean -fdx

