# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix interpreter test makefile                                          -
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

DSTDIR		= $(BLDDST)/src/clt/axi/tst

# ----------------------------------------------------------------------------
# test definition                                                            -
# ----------------------------------------------------------------------------

TSTALS		= $(wildcard $(TSTREF)/*.als)
TSTAXI		= $(notdir $(TSTALS:.als=.axi))

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
# map als file to axd files
vpath %.als : $(TSTREF)
%.axi       : %.als
	@$(CP) $< $@
	@$(AEXEC) -v --prefix=$(BLDDIR) \
                     --binexe=$(AXIEXE) --binopt=$(AXIOPT) $@
	@$(RM)    $@

# rule: test
# test the als files

test: $(TSTAXI)
	@$(RM) *.axi
.PHONY: test

# rule: distri
# this rule install the exe distribution files

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
.PHONY: distri

# rule: clean
# this cleans file and some local extras

clean::
	@$(RM) *.axi
	@$(RM) *.xl
