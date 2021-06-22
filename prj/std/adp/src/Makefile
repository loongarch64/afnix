# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix:adp project makefile                                               -
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

DSTDIR		= $(BLDDST)/prj/std/adp/src

APPLIB		= afnix-std-adp.axl
APPALS          = $(wildcard std-adp-*.als) $(wildcard cmd-adp-*.als)
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

# rule: install
# this rule install the distribution

install: $(APPLIB)
	@$(MKDIR)        $(PRJDIR)/axl
	@$(CP) $(APPLIB) $(PRJDIR)/axl
.PHONY: install

# rule: distri
# this rule install the app distribution files

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
	@$(CP)    *.als    $(DSTDIR)
.PHONY: distri

# rule: clean
# this rule local files
clean::
	@$(RM) $(APPLIB)
	@$(RM) *.axc
