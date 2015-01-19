#==============================================================================

# In a real extension, not bundled with the code generation software, this
# should be modified to contain the location of the php-ext-gen.phk file

PHP_EXT_GEN_PKG = ../../php-ext-gen.phk

#-----------------

EXTENSION = newt
TARGETS = gen/extgen_php_$(EXTENSION).c
COMPILE_TARGET = gen/modules/$(EXTENSION).la

include ../../make.vars
include ../../make.common

#-----------------------------------------------------------------------------

.PHONY: all clean test compile base install

all: base

clean: clean_base
	/bin/rm -rf gen

base: $(TARGETS)

$(TARGETS):
	mkdir gen && $(PHPCMD) $(PHP_EXT_GEN_PKG) build

compile: $(COMPILE_TARGET)

$(COMPILE_TARGET): $(TARGETS)
	cd gen && phpize && ./configure && make

install: $(COMPILE_TARGET)
	$(MAKE) -C gen $@

test: $(COMPILE_TARGET)
	$(MAKE) -C gen $@

#-----------------------------------------------------------------------------
