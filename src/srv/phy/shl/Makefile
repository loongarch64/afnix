# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix:phy service makefile                                               -
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

TOPDIR		= ../../../..
MAKDIR		= $(TOPDIR)/cnf/mak
CONFFILE	= $(MAKDIR)/afnix-conf.mak
RULEFILE	= $(MAKDIR)/afnix-rule.mak
include		  $(CONFFILE)

# ----------------------------------------------------------------------------
# project configurationn                                                     -
# ----------------------------------------------------------------------------

TRGDIR		= phy
TRGLIB		= afnix-phy
DSTDIR		= $(BLDDST)/src/srv/phy/shl
EXTLIB		= -L$(BLDLIB)     \
                  -lafnix-eng     \
                  -lafnix-std     \
                  -lafnix-plt
INCLUDE		= -I.             \
                  -I$(BLDHDR)/eng \
                  -I$(BLDHDR)/std \
                  -I$(BLDHDR)/plt

# ----------------------------------------------------------------------------
# - project rules                                                            -
# ----------------------------------------------------------------------------

# rule: all
# this rule is the default rule which call the build rule

all: $(TRGLIB)
.PHONY: all

# include: rule.mak
# this rule includes the platform dependant rules

include $(RULEFILE)

# rule: distri
# this rule install the phy distribution files

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
	@$(CP)    *.hpp    $(DSTDIR)
	@$(CP)    *.cpp    $(DSTDIR)
.PHONY: distri

# rule: install
# this rule install the distribution

install: install-lib
	@$(MKDIR)    $(HDRDIR)/phy
	@$(CP) *.hpp $(HDRDIR)/phy
.PHONY: install
