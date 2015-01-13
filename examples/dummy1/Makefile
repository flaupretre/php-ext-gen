#==============================================================================

EXTENSION = dummy1
TARGETS = extgen_$(EXTENSION).c
PHP_EXT_GEN_PKG = ../../php-ext-gen.phk

include ../../make.vars
include ../../make.common

#-----------------------------------------------------------------------------

.PHONY: all clean

all: base

clean: clean_base

extgen_$(EXTENSION).c :
	$(PHPCMD) $(PHP_EXT_GEN_PKG) build

#-----------------------------------------------------------------------------
