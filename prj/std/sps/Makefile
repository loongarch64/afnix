# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix:std:sps project makefile                                           -
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

TOPDIR		= ../../..
MAKDIR		= $(TOPDIR)/cnf/mak
CONFFILE	= $(MAKDIR)/afnix-conf.mak
RULEFILE	= $(MAKDIR)/afnix-rule.mak
include		  $(CONFFILE)

# ----------------------------------------------------------------------------
# - project configuration                                                    -
# ----------------------------------------------------------------------------

DSTDIR		= $(BLDDST)/prj/std/sps

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
	@${MAKE} -C src all
.PHONY: build

# rule: test
# this rule build and test all libraries

test:
	@${MAKE} -C tst test
.PHONY: test

# rule: doc
# this rule build the documentation

doc:
	@${MAKE} -C doc doc
.PHONY: doc

# rule: distri
# this rule install the distribution files

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
	@${MAKE}  -C src distri
.PHONY: distri

# rule: install
# this rule install the distribution

install:
	@${MAKE} -C src install
.PHONY: install

# rule: publish
# this rule install the documentation

publish:
	@${MAKE} -C doc publish
.PHONY: publish

# rule: clean
# this rule clean all source directories

clean::
	@${MAKE} -C src clean
.PHONY: clean
