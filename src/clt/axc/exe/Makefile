# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix cross compiler client makefile                                     -
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

DSTDIR		= $(BLDDST)/src/clt/axc/exe
INCLUDE		= -I.             \
	          -I$(BLDHDR)/eng \
                  -I$(BLDHDR)/std \
                  -I$(BLDHDR)/plt
EXELIBS		= -L$(BLDLIB)     \
                  -lafnix-eng     \
                  -lafnix-std     \
                  -lafnix-plt     \
                  $(STDLIBS)
STALIBS		= $(BLDLIB)/libafnix-eng.a \
                  $(BLDLIB)/libafnix-std.a \
                  $(BLDLIB)/libafnix-plt.a \
                  $(STDLIBS)
TARGETS		= axc

# ----------------------------------------------------------------------------
# - project rules                                                            -
# ----------------------------------------------------------------------------

# rule: all
# this rule is the default rule which call the target rule

all: targets
.PHONY: all

# include: rule.mak
# this rule includes the platform dependant rules

include $(RULEFILE)

# rule: distri
# this rule install the exe distribution files

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
	@$(CP)    *.cpp    $(DSTDIR)
.PHONY: distri

# rule: install
# this rule install the distribution

install:
	@$(MKDIR) $(BINDIR)
	@$(CP)    $(TARGETS) $(BINDIR)
.PHONY: install

# rule: clean
# local clean rule

clean::
	@$(RM) $(TARGETS)
