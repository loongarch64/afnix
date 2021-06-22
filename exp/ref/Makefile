# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix exp makefile                                                       -
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

DSTDIR		= $(BLDDST)/exp/ref

# ----------------------------------------------------------------------------
# test definition                                                            -
# ----------------------------------------------------------------------------

AXCEXE          = "axc"
TSTALS          = $(wildcard *.als)
TSTAXC          = $(TSTALS:.als=.axc)

# ----------------------------------------------------------------------------
# - project rules                                                            -
# ----------------------------------------------------------------------------

# rule: all
# this rule is the default rule that compile the files

all: compile
.PHONY: all

# include: rule.mak
# this rule includes the platform dependant rules

include $(RULEFILE)

# this rule compile an als file

%.axc : %.als
	@$(AEXEC) --prefix=$(BLDDIR) --binexe=$(AXCEXE) $<

# compile
# this rule compile all example file

compile: $(TSTAXC)
.PHONY: compile

# rule: distri
# this rule install the exp distribution files

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
	@$(CP)    *.als    $(DSTDIR)
.PHONY: distri

# clean
# this rule clean for some local file 

clean::
	@$(RM) *.axc
