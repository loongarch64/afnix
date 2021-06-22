# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix clients makefile                                                   -
# ----------------------------------------------------------------------------
# - This program is  free software;  you can  redistribute it and/or  modify -
# - it provided that this copyright notice is kept intact.                   -
# -                                                                          -
# - This  program  is  distributed in the hope  that it  will be useful, but -
# - without  any   warranty;  without  even   the   implied    warranty   of -
# - merchantability  or fitness for a particular purpose. In not event shall -
# - the copyright holder be  liable for  any direct, indirect, incidental or -
# - special damages arising in any way out of the use of this software.      -
# ----------------------------------------------------------------------------
# - copyright (c) 1999-2021 amaury darsch                                    -
# ----------------------------------------------------------------------------

TOPDIR		= ../..
MAKDIR		= $(TOPDIR)/cnf/mak
CONFFILE	= $(MAKDIR)/afnix-conf.mak
RULEFILE	= $(MAKDIR)/afnix-rule.mak
include		  $(CONFFILE)

# ----------------------------------------------------------------------------
# - project configuration                                                    -
# ----------------------------------------------------------------------------

DSTDIR		= $(BLDDST)/src/clt

# ----------------------------------------------------------------------------
# - project rules                                                            -
# ----------------------------------------------------------------------------

# rule: all
# this rule is the default rule which call the build rule

all: build
.PHONY: all

# include: rule.mak
# this rule includes the platform dependant rules

include $(RULEFILE)

# rule: build
# this rule build all source directories

build:
	@${MAKE} -C axi all
	@${MAKE} -C axc all
	@${MAKE} -C axl all
	@${MAKE} -C axd all
.PHONY: build

# rule: test
# this rule build and test all clients

test:
	@${MAKE} -C axi test
	@${MAKE} -C axc test
	@${MAKE} -C axl test
	@${MAKE} -C axd test
.PHONY: test

# rule: doc
# this rule build the documentation

doc:
	@${MAKE} -C axi doc
	@${MAKE} -C axc doc
	@${MAKE} -C axl doc
	@${MAKE} -C axd doc
.PHONY: doc

# rule: distri
# this rule install the clt distribution files

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
	@${MAKE}  -C axi distri
	@${MAKE}  -C axc distri
	@${MAKE}  -C axl distri
	@${MAKE}  -C axd distri
.PHONY: distri

# rule: install
# this rule install the distribution

install:
	@${MAKE} -C axi install
	@${MAKE} -C axc install
	@${MAKE} -C axl install
	@${MAKE} -C axd install
.PHONY: install

# rule: publish
# this rule install the documentation

publish:
	@${MAKE} -C axi publish
	@${MAKE} -C axc publish
	@${MAKE} -C axl publish
	@${MAKE} -C axd publish
.PHONY: publish

# rule: clean
# this rule clean all source directories

clean::
	@${MAKE} -C axi clean
	@${MAKE} -C axc clean
	@${MAKE} -C axl clean
	@${MAKE} -C axd clean
.PHONY: clean
