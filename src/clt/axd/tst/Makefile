# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix cross debugger test makefile                                       -
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

DSTDIR		= $(BLDDST)/src/clt/axd/tst

# ----------------------------------------------------------------------------
# test definition overwrite                                                  -
# ----------------------------------------------------------------------------

AXDEXE          = "axd"
AXDOPT          = "-e UTF-8 -f assert -f runini"
TSTALS		= $(wildcard $(TSTREF)/*.als)
TSTAXD		= $(notdir $(TSTALS:.als=.axd))

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
%.axd       : %.als
	@$(CP) $< $@
	@$(AEXEC) -v --prefix=$(BLDDIR) \
                     --binexe=$(AXDEXE) --binopt=$(AXDOPT) $@
	@$(RM)    $@

# rule: test
# test the als files

test: $(TSTAXD)
	@$(RM) *.axd
.PHONY: test

# rule: distri
# this rule install the tst distribution files

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
.PHONY: distri

# rule: clean
# this cleans file and some local extras

clean::
	@$(RM) *.xl
	@$(RM) *.axd
