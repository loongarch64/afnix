# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix cross librarian test makefile                                      -
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

DSTDIR		= $(BLDDST)/src/clt/axl/tst

# ----------------------------------------------------------------------------
# test definition overwrite                                                  -
# ----------------------------------------------------------------------------

AXLEXE          = "axl"
AXLCRL          = "-c -f test.xl"
AXLXRL          = "-x -f test.xl"
TSTALS		= $(wildcard $(TSTREF)/*.als)
TSTAXL		= $(notdir $(TSTALS:.als=.axl))

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

# rule: 
# map als file to axl files
vpath %.als : $(TSTREF)
%.axl       : %.als
	@$(CP) $< $@

# rule: test
# build a test.xl file/ extract the files / test the files

test: $(TSTAXL)
	@$(AEXEC) -n --prefix=$(BLDDIR) \
                     --binexe=$(AXLEXE) --binopt=$(AXLCRL) $(TSTAXL)
	@$(RM) *.axl
	@$(AEXEC) -n --prefix=$(BLDDIR) \
                     --binexe=$(AXLEXE) --binopt=$(AXLXRL)
	@$(RM) test.xl
	@$(AEXEC) -v --prefix=$(BLDDIR) \
                     --binexe=$(AXIEXE) --binopt=$(AXIOPT) $(TSTAXL)
	@$(RM) *.axl
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
	@$(RM) *.xl
	@$(RM) *.axl
.PHONY: clean
