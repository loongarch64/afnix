# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix deb makefile                                                       -
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

TOPDIR		= ../../..
MAKDIR		= $(TOPDIR)/cnf/mak
CONFFILE	= $(MAKDIR)/afnix-conf.mak
RULEFILE	= $(MAKDIR)/afnix-rule.mak
include		  $(CONFFILE)

# ----------------------------------------------------------------------------
# - project configuration                                                    -
# ----------------------------------------------------------------------------

DSTDIR		= $(BLDDST)/cnf/pkg/deb

SRCDIR		= pkg/deb/afnix-$(VERSION)
TRGDIR		= $(BLDDIR)/$(SRCDIR)
DEBDIR		= $(TRGDIR)/debian

# ----------------------------------------------------------------------------
# - project rules                                                            -
# ----------------------------------------------------------------------------

# rule: all
# this rule is the default rule which does nothing
all: build

# include: rule.mak
# this rule includes the platform dependant rules
include $(RULEFILE)

# rule: build
# build the debian packages
build:
	@$(RM)  -rf $(TRGDIR)
	${MAKE} -C  $(TOPDIR) SRCDST=$(SRCDIR) distri
	@$(MKDIR)   $(DEBDIR)
	@$(CP)      rules             $(DEBDIR)
	@$(CP)      compat            $(DEBDIR)
	@$(CP)      control           $(DEBDIR)
	@$(CP)      changelog         $(DEBDIR)
	@$(CP)      copyright         $(DEBDIR)
	@$(CP)      afnix.install     $(DEBDIR)
	@$(CP)      afnix-doc.install $(DEBDIR)
	@$(CP)      afnix-dev.install $(DEBDIR)
	@(cd        $(TRGDIR);        ./debian/rules build)
	@(cd        $(TRGDIR);        fakeroot ./debian/rules binary)
	@(cd        $(BLDDIR);        $(LN) $(SRCDIR)/bld deb)
.PHONY:build

# rule: distri
# this rules install the deb files in the distribution

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile          $(DSTDIR)
	@$(CP)    rules             $(DSTDIR)
	@$(CP)    compat            $(DSTDIR)
	@$(CP)    control           $(DSTDIR)
	@$(CP)    changelog         $(DSTDIR)
	@$(CP)    copyright         $(DSTDIR)
	@$(CP)    afnix.install     $(DSTDIR)
	@$(CP)    afnix-doc.install $(DSTDIR)
	@$(CP)    afnix-dev.install $(DSTDIR)
.PHONY: distri

