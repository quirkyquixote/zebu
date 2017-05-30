
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

.PHONY: html
html:
	@make -C doc html

.PHONY: clean-html
clean-html:
	@make -C doc clean-html

.PHONY: realclean
realclean: 
	git clean -fdx

