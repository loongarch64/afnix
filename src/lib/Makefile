# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix libraries makefile                                                 -
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

DSTDIR		= $(BLDDST)/src/lib

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
	@${MAKE} -C plt all
	@${MAKE} -C bit all
	@${MAKE} -C std all
	@${MAKE} -C eng all
.PHONY: build

# rule: test
# this rule build and test all libraries

test:
	@${MAKE} -C plt test
	@${MAKE} -C bit test
	@${MAKE} -C std test
	@${MAKE} -C eng test
.PHONY: test

# rule: distri
# this rule install the mod distribution files

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
	@${MAKE} -C plt distri
	@${MAKE} -C bit distri
	@${MAKE} -C std distri
	@${MAKE} -C eng distri
.PHONY: distri

# rule: install
# this rule install the distribution

install:
	@${MAKE} -C plt install
	@${MAKE} -C bit install
	@${MAKE} -C std install
	@${MAKE} -C eng install
.PHONY: install

# rule: clean
# this rule clean all source directories

clean::
	@${MAKE} -C plt clean
	@${MAKE} -C bit clean
	@${MAKE} -C std clean
	@${MAKE} -C eng clean
.PHONY: clean
