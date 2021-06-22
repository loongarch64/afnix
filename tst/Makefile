# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix tst makefile                                                       -
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

DSTDIR		= $(BLDDST)/tst

# ----------------------------------------------------------------------------
# - project rules                                                            -
# ----------------------------------------------------------------------------

# rule: all
# this rule is the default rule which run the test rule

all: test
.PHONY: test

# include: rule.mak
# this rule includes the platform dependant rules

include $(RULEFILE)

# rule: devel
# this rule install the development files

devel:
	@${MAKE} -C ref devel
	@${MAKE} -C mod devel
.PHONY: devel

# rule: test
# this rule runs all test suite

test:
	@${MAKE}  -C ref
	@${MAKE}  -C mod
.PHONY: test

# rule: distri
# this rule install the mak files in the distribution

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
	@${MAKE}  -C ref distri
	@${MAKE}  -C mod distri
.PHONY: distri

# rule: clean
# This rule cleans the configuration.

clean::
	@${MAKE} -C ref clean
	@${MAKE} -C mod clean
