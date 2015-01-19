# A minimal Makefile to build a php-ext-gen extension
#==============================================================================

# The location of the php-ext-gen.phk file

PHP_EXT_GEN_PKG = ../../php-ext-gen.phk

#-----------------------------------------------------------------------------

all: gen

clean:
	/bin/rm -rf gen

gen:
	mkdir gen && php $(PHP_EXT_GEN_PKG) build

#-----------------------------------------------------------------------------
