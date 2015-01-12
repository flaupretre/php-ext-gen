#==============================================================================

TARGETS = newt.done
PHP_EXT_GEN_PKG = ../../php-ext-gen.phk

include ../../make.vars
include ../../make.common

#-----------------------------------------------------------------------------

.PHONY: all clean

all: base

clean: clean_base
	/bin/rm -rf ext

newt.done :
	$(PHPCMD) $(PHP_EXT_GEN_PKG) build -f php5 . ext

#-----------------------------------------------------------------------------
