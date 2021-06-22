# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix rpm makefile                                                       -
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

DSTDIR		= $(BLDDST)/cnf/pkg/rpm

TRGDIR		= $(BLDDIR)/pkg/rpm
SRCPKG		= afnix-src-$(VERSION)
SRCTGZ		= $(SRCPKG).tgz
SRCDIR		= pkg/rpm/$(SRCPKG)
FEDVRS		= 31

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
# create the rpm build directory

build:
	@$(RM)  -rf        $(TRGDIR)
	${MAKE} -C         $(TOPDIR) SRCDST=$(SRCDIR) distri
	@$(CP)  afnix.spec $(TRGDIR)
	@(cd $(TRGDIR); tar cvzf $(SRCTGZ) $(SRCPKG); rm -rf $(SRCPKG))
	@(cd $(TRGDIR); fedpkg --release f$(FEDVRS) local)
	@(cd $(BLDDIR); $(LN) pkg/rpm/$(SRCPKG)/bld rpm)
	@$(RM) -rf ~/rpmbuild

# rule: distri
# this rules install the rpm files in the distribution

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile    $(DSTDIR)
	@$(CP)    afnix.spec  $(DSTDIR)
.PHONY: distri

