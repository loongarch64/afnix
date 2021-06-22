# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix:amd project makefile                                               -
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

TOPDIR		= ../../../../..
MAKDIR		= $(TOPDIR)/cnf/mak
CONFFILE	= $(MAKDIR)/afnix-conf.mak
RULEFILE	= $(MAKDIR)/afnix-rule.mak
include		  $(CONFFILE)

# ----------------------------------------------------------------------------
# project configurationn                                                     -
# ----------------------------------------------------------------------------

DSTDIR		= $(BLDDST)/exp/prj/amd/src/app

APPLIB		= afnix-exp-amd.axl
APPALS          = $(wildcard exp-amd-*.als)
APPAXC          = $(APPALS:.als=.axc)
AXCEXE          = "axc"
AXLEXE          = "axl"

# ----------------------------------------------------------------------------
# - project rules                                                            -
# ----------------------------------------------------------------------------

# rule: all
# this rule is the default rule which call the build rule

all: $(APPLIB)
.PHONY: all

# include: rule.mak
# this rule includes the platform dependant rules

include $(RULEFILE)
# rule: build
# this rule build all source directories

%.axc : %.als
	@echo "compiling " $<
	@$(AEXEC) --prefix=$(BLDDIR) --binexe=$(AXCEXE) $<

$(APPLIB) : $(APPAXC)
	@echo "linking   " $@
	@$(AEXEC) --noloop --prefix=$(BLDDIR) --binexe=$(AXLEXE) \
                  --binopt="-c -f $@" $(APPAXC)
	@$(MKDIR) $(BLDAXL)
	@$(CP) $@ $(BLDAXL)

# rule: distri
# this rule create the ditribution

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
	@$(CP)    $(wildcard *.als) $(DSTDIR)
.PHONY: distri

# rule: install
# this rule install the distribution

install: $(APPLIB)
	@$(MKDIR)        $(PRJDIR)/axl
	@$(CP) $(APPLIB) $(PRJDIR)/axl
.PHONY: install

# rule: clean
# this rule local files
clean::
	@$(RM) $(APPLIB)
	@$(RM) *.axc
