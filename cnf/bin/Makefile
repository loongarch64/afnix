# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix bin makefile                                                       -
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

DSTDIR		= $(BLDDST)/cnf/bin
DVLDIR		= $(BLDDVL)/bin

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
# this rule install the development file

devel:
	@$(MKDIR) $(DVLDIR)
	@$(CP)    afnix-fxdir $(DVLDIR)
	@$(CP)    afnix-aexec $(DVLDIR)
	@$(CP)    afnix-bexec $(DVLDIR)
.PHONY: devel

# rule: distri
# this rule install the bin files in the distribution

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile    $(DSTDIR)
	@$(CP)    afnix-aexec $(DSTDIR)
	@$(CP)    afnix-bexec $(DSTDIR)
	@$(CP)    afnix-guess $(DSTDIR)
	@$(CP)    afnix-fxdir $(DSTDIR)
	@$(CP)    afnix-query $(DSTDIR)
	@$(CP)    afnix-setup $(DSTDIR)
	@$(CP)    afnix-vcomp $(DSTDIR)
.PHONY: distri
