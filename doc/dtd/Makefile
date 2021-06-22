# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix dtd makefile                                                       -
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
RULEFILE	= $(MAKDIR)/afnix-docs.mak
include		  $(CONFFILE)

# ----------------------------------------------------------------------------
# - project configuration                                                    -
# ----------------------------------------------------------------------------

DSTDIR		= $(BLDDST)/doc/dtd

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

# rule: distri
# this rule install the distribution

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile  $(DSTDIR)
	@$(CP)    std-us-afnix.dtd $(DSTDIR)
.PHONY: distri
