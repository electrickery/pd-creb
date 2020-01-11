#!/usr/bin/make -f
# Makefile for pure data externals in lib creb.
# Needs Makefile.pdlibbuilder to work (https://github.com/pure-data/pd-lib-builder)

lib.name = creb

# special file that does not provide a class
lib.setup.sources = modules/setup.c

# all other C and C++ files in subdirs are source files per class
# (alternatively, enumerate them by hand)
class.sources = $(filter-out $(lib.setup.sources),$(wildcard modules/*.c modules++/*.cc))

datafiles = \
$(wildcard doc/*-help.pd abs/*.pd) \
creb-meta.pd \
CHANGES.LOG \
COPYING \
README \
TODO

datadirs = doc/examples/

# per default, build a multi-object library
make-lib-executable=yes

# pass current version (from creb-meta.pd) to the compiler
cflags = -DCREB_VERSION=\"$(shell $(SHELL) bin/version)\"

# default target 'all'
all:
pre: bootstrap

################################################################################
### pdlibbuilder ###############################################################
################################################################################


# Include Makefile.pdlibbuilder from this directory, or else from externals
# root directory in pd-extended configuration.

include $(firstword $(wildcard Makefile.pdlibbuilder ../Makefile.pdlibbuilder))


################################################################################
### creb extra targets #########################################################
################################################################################


.PHONY: bootstrap
bootstrap:
	-chmod +x bin/*


# install files with idiosyncratic source/destination paths

install: install-creb-extras

install-creb-extras: all
	$(INSTALL_DIR) $(installpath)/manual && \
        $(INSTALL_DATA) doc/reference.txt $(installpath)/manual
	$(INSTALL_DIR) $(installpath)/examples && \
        $(INSTALL_DATA) $(wildcard doc/examples/*.pd) $(installpath)/examples

