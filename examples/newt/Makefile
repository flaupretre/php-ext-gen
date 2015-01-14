#==============================================================================

# In a real extension, not bundled with the code generation software, this
# should be modified to contain the location of the php-ext-gen.phk file

PHP_EXT_GEN_PKG = ../../php-ext-gen.phk

#-----------------

EXTENSION = newt
TARGETS = gen
COMPILE_TARGET = gen/modules/$(EXTENSION).la

include ../../make.vars
include ../../make.common

#-----------------------------------------------------------------------------

.PHONY: all clean

all: base

clean: clean_base

base: gen

gen:
	mkdir gen && $(PHPCMD) $(PHP_EXT_GEN_PKG) build

compile: $(COMPILE_TARGET)

$(COMPILE_TARGET): gen
	cd gen && phpize && ./configure && make

install: compile
	$(MAKE) -C gen $@

#-----------------------------------------------------------------------------
