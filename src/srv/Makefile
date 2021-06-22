# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix services makefile                                                   -
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

DSTDIR		= $(BLDDST)/src/srv

# ----------------------------------------------------------------------------
# - project rules                                                            -
# ----------------------------------------------------------------------------

# rule: all
# this rule is the default rule which call the build rule

all: build
.PHONY: all

# include: rule.mak
# this rule includes the platform dependant rules

include $(RULEFILE)

# rule: build
# this rule build all source directories

build:
	@${MAKE} -C bce all
	@${MAKE} -C cda all
	@${MAKE} -C csm all
	@${MAKE} -C dip all
	@${MAKE} -C phy all
	@${MAKE} -C svg all
	@${MAKE} -C tls all
	@${MAKE} -C xpe all
	@${MAKE} -C wax all
.PHONY: build

# rule: test
# this rule build and test all libraries

test:
	@${MAKE} -C bce test
	@${MAKE} -C cda test
	@${MAKE} -C csm test
	@${MAKE} -C dip test
	@${MAKE} -C phy test
	@${MAKE} -C svg test
	@${MAKE} -C tls test
	@${MAKE} -C xpe test
	@${MAKE} -C wax test
.PHONY: test

# rule: doc
# this rule builds the documentation

doc:
	@${MAKE} -C bce doc
	@${MAKE} -C csm doc
	@${MAKE} -C xpe doc
	@${MAKE} -C wax doc
.PHONY: doc

# rule: distri
# this rule install the mod distribution files

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
	@${MAKE} -C bce distri
	@${MAKE} -C cda distri
	@${MAKE} -C csm distri
	@${MAKE} -C dip distri
	@${MAKE} -C phy distri
	@${MAKE} -C svg distri
	@${MAKE} -C tls distri
	@${MAKE} -C xpe distri
	@${MAKE} -C wax distri
.PHONY: distri

# rule: install
# this rule install the distribution

install:
	@${MAKE} -C bce install
	@${MAKE} -C cda install
	@${MAKE} -C csm install
	@${MAKE} -C dip install
	@${MAKE} -C phy install
	@${MAKE} -C svg install
	@${MAKE} -C tls install
	@${MAKE} -C xpe install
	@${MAKE} -C wax install
.PHONY: install

# rule: publish
# this rule install the documentation

publish:
	@${MAKE} -C bce publish
	@${MAKE} -C csm publish
	@${MAKE} -C xpe publish
	@${MAKE} -C wax publish
.PHONY: publish

# rule: clean
# this rule clean all source directories

clean::
	@${MAKE} -C bce clean
	@${MAKE} -C cda clean
	@${MAKE} -C csm clean
	@${MAKE} -C dip clean
	@${MAKE} -C phy clean
	@${MAKE} -C svg clean
	@${MAKE} -C tls clean
	@${MAKE} -C xpe clean
	@${MAKE} -C wax clean
.PHONY: clean
