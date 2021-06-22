# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix nuts and bolts test makefile                                       -
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

DSTDIR		= $(BLDDST)/src/lib/bit/tst
INCLUDE		= -I.             \
                  -I$(BLDHDR)/bit \
                  -I$(BLDHDR)/plt
EXELIBS		= -L$(BLDLIB)     \
                  -lafnix-plt     \
                  $(STDLIBS)
TESTERS         = $(basename $(CPPSRCS))

# ----------------------------------------------------------------------------
# - project rules                                                            -
# ----------------------------------------------------------------------------

# rule: all
# this rule is the default rule which call the test rule
all: test
.PHONY: all

# include: rule.mak
# this rule includes the platform dependant rules
include $(RULEFILE)

# rule: distri
# this rule install the tst distribution files
distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
	@$(CP)    *.cpp    $(DSTDIR)
.PHONY: distri

# rule: clean
# local clean for test programs
clean::
	@$(RM) $(TESTERS)
