# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix cnf makefile                                                       -
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

DSTDIR		= $(BLDDST)/cnf

# ----------------------------------------------------------------------------
# - project rules                                                            -
# ----------------------------------------------------------------------------

# rule: all
# this rule is the default rule which does nothing

all:
	@exit 0
.PHONY: all

# include: rule.mak
# this rule includes the platform dependant rules

include $(RULEFILE)

# rule: devel
# this rule install the development files

devel:
	@${MAKE} -C bin devel
	@${MAKE} -C mak devel
.PHONY: devel

# rule: distri
# this rule install the mak files in the distribution

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
	@${MAKE}  -C bin distri
	@${MAKE}  -C def distri
	@${MAKE}  -C mak distri
	@${MAKE}  -C pkg distri
.PHONY: distri

# rule: clean
# This rule cleans the configuration.

clean::
	@${MAKE} -C bin clean
	@${MAKE} -C def clean
	@${MAKE} -C mak clean
	@${MAKE} -C pkg clean
