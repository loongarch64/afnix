# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix cross compiler test makefile                                       -
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

DSTDIR		= $(BLDDST)/src/clt/axc/tst

# ----------------------------------------------------------------------------
# test definition                                                            -
# ----------------------------------------------------------------------------

AXCEXE          = "axc"
AXCOPT          = "-e UTF-8"
TSTALS		= $(wildcard $(TSTREF)/*.als)
TSTAXC		= $(notdir $(TSTALS:.als=.axc))

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

# this rule compile and test an als file

vpath %.als : $(TSTREF)
%.axc       : %.als
	@$(AEXEC) --prefix=$(BLDDIR) \
                  --binexe=$(AXCEXE) --binopt=$(AXCOPT) $<
	@$(AEXEC) -v --prefix=$(BLDDIR) \
                     --binexe=$(AXIEXE) --binopt=$(AXIOPT) $@
	@$(RM) $@

# rule: test
# test the cross compiler

test: $(TSTAXC)
	@$(RM) *.xl
	@$(RM) *.axc
.PHONY: test

# rule: distri
# this rule install the exe distribution files

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
.PHONY: distri

# clean
# this rule some local files
clean::
	@$(RM) test.xl
	@$(RM) *.axc
.PHONY: clean
