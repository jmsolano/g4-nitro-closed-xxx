# Add the following 'help' target to your Makefile
# # And add help text after each target name starting with '\#\#'
#  
help:           ## Type 'make help'
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##//'

SCRIPTS = $(shell ls *.sh)


INSTALL_PATH ?= /usr/local/bin

.PHONY: install
install:	## Copies all scripts to /usr/local/bin. You can override the installation path by setting INSTALL_PATH="/your/install/path"
	@for i in $(SCRIPTS);do cp $$i $(INSTALL_PATH)/$${i%.sh} && chmod +x $(INSTALL_PATH)/$${i%.sh};done

.PHONY: uninstall
uninstall: ## Removes all scripts from /usr/local/bin. You can override the installation path by setting INSTALL_PATH="/your/install/path"
	@for i in $(SCRIPTS);do rm $(INSTALL_PATH)/$${i%.sh};done

.PHONY: all
all: help

