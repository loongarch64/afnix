# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix etc makefile                                                       -
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

TOPDIR		= ..
MAKDIR		= $(TOPDIR)/cnf/mak
CONFFILE	= $(MAKDIR)/afnix-conf.mak
RULEFILE	= $(MAKDIR)/afnix-rule.mak
include		  $(CONFFILE)

# ----------------------------------------------------------------------------
# - project configuration                                                    -
# ----------------------------------------------------------------------------

DSTDIR		= $(BLDDST)/etc

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
	@${MAKE} -C std all
	@${MAKE} -C unx all
.PHONY: build

# rule: distri
# this rule installs the src distribution files

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile  $(DSTDIR)
	@${MAKE} -C std distri
	@${MAKE} -C unx distri
.PHONY: distri

# rule: install
# this rule installs the distribution

install:
	@${MAKE} -C std install
	@${MAKE} -C unx install
.PHONY: install

# rule: clean
# this rule clean all source directories

clean::
	@${MAKE} -C std clean
	@${MAKE} -C unx clean
.PHONY: clean
