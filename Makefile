#
#==============================================================================

TARGETS = $(PRODUCT).phk
SOURCE_DIR = src
BUILD_DIR = build
EXTRA_CLEAN = $(PRODUCT).psf $(PRODUCT)

#-----------------------------

include ./make.vars
include ./make.common

#-----------------------------

.PHONY: all clean_distrib istrib test mem_test clean_test \
	clean_examples

all: base

clean: clean_base clean_distrib clean_test clean_examples

#--- How to build the package

$(PRODUCT).phk: $(PRODUCT).psf
	$(PHK_BUILD) $@

#--- Tests

test mem_test: base
	$(MAKE) -C test $@

clean_test:
	$(MAKE) -C test clean

#--- Examples

clean_examples:
	$(MAKE) -C examples clean

#--- How to build distrib
# As we copy the whole examples and test subdirs into the distrib, we must
# clean them first.

distrib:  $(DISTRIB)

$(DISTRIB): base clean_test
	BASE=$(PWD) TMP_DIR=$(TMP_DIR) PRODUCT=$(PRODUCT) \
	SOFTWARE_VERSION=$(SOFTWARE_VERSION) \
	SOFTWARE_RELEASE=$(SOFTWARE_RELEASE) $(MK_DISTRIB)

clean_distrib:
	/bin/rm -f $(DISTRIB)

#--- Sync subtrees - Dev private

SYNC = rsync -av --del --exclude external --delete-excluded

sync: sync_phool

sync_phool:
	$(SYNC) ../../phool/public/ external/phool

#-----------------------------------------------------------------------------
