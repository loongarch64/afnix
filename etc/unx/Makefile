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

TOPDIR		= ../..
MAKDIR		= $(TOPDIR)/cnf/mak
CONFFILE	= $(MAKDIR)/afnix-conf.mak
RULEFILE	= $(MAKDIR)/afnix-rule.mak
include		  $(CONFFILE)

# ----------------------------------------------------------------------------
# - project configuration                                                    -
# ----------------------------------------------------------------------------

DSTDIR		= $(BLDDST)/etc/unx
EELDIR		= $(SHRDIR)/emacs/site-list

# ----------------------------------------------------------------------------
# - project rules                                                            -
# ----------------------------------------------------------------------------

# rule: all
# this rule is the default rule which call the build rule

all:
	@exit 0
.PHONY: all

# include: rule.mak
# this rule includes the platform dependant rules

include $(RULEFILE)

# rule: distri
# this rule install the etc distribution files

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile       $(DSTDIR)
	@$(CP)    afnix-gud.el   $(DSTDIR)
	@$(CP)    afnix-mode.el  $(DSTDIR)
.PHONY: distri

# rule: install
# this rule install the distribution

install:
	@exit 0
.PHONY: install

# rule: emacs
# this rule install the emacs distribution

emacs:
	@$(MKDIR) $(EELDIR)
	@$(CP)    afnix-gud.el   $(EELDIR)
	@$(CP)    afnix-mode.el  $(EELDIR)
.PHONY: emacs
