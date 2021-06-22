# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix standard project makefile                                          -
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

DSTDIR		= $(BLDDST)/prj/std

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
# this rule build all project directories

build:
	@${MAKE} -C adp all
	@${MAKE} -C axp all
	@${MAKE} -C sps all
	@${MAKE} -C tls all
.PHONY: build

# rule: test
# this rule build and test all projects

test:
	@${MAKE} -C adp test
	@${MAKE} -C axp test
.PHONY: test

# rule: doc
# this rule builds the documentation

doc:
	@${MAKE} -C adp doc
	@${MAKE} -C axp doc
.PHONY: doc

# rule: distri
# this rule creates the distribution files

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
	@${MAKE} -C adp distri
	@${MAKE} -C axp distri
	@${MAKE} -C sps distri
	@${MAKE} -C tls distri
.PHONY: distri

# rule: install
# this rule install the distribution

install:
	@${MAKE} -C adp install
	@${MAKE} -C axp install
	@${MAKE} -C sps install
	@${MAKE} -C tls install
.PHONY: install

# rule: publish
# this rule publis the distribution

publish:
	@${MAKE} -C adp publish
	@${MAKE} -C axp publish
.PHONY: publish

# rule: clean
# this rule clean all source directories

clean::
	@${MAKE} -C adp clean
	@${MAKE} -C axp clean
	@${MAKE} -C sps clean
	@${MAKE} -C tls clean
.PHONY: clean
