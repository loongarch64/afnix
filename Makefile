# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix top level makefile                                                 -
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

TOPDIR          = .
CONFFILE	= cnf/mak/afnix-conf.mak
RULEFILE	= cnf/mak/afnix-rule.mak
include		$(CONFFILE)

# ----------------------------------------------------------------------------
# - project rules                                                            -
# ----------------------------------------------------------------------------

# rule: all
# this rule is the default rule which call the build rule
all   : build
.PHONY: all

# include: afnix-rule.mak
# this rule includes the platform dependant rules

include $(RULEFILE)

# rule: debug
# configure the world in debug mode
debug:
	@$(TEST) -f bld/cnf/afnix-comp.mak && \
         $(TEST) -f bld/cnf/afnix-plat.mak || ./cnf/bin/afnix-setup -g
.PHONY: debug

# rule: optimized
# configure the world in optimized mode
optimized:
	@$(TEST) -f bld/cnf/afnix-comp.mak && \
         $(TEST) -f bld/cnf/afnix-plat.mak || ./cnf/bin/afnix-setup -o
.PHONY: optimized

# rule: build
# this rule build the complete source tree
build: debug
	@${MAKE} -C src all
	@${MAKE} -C prj all
	@${MAKE} devel
.PHONY: build

# rule: world
# this rule build the optimized world
world: optimized
	@${MAKE} -C src all
	@${MAKE} -C prj all
	@${MAKE} devel
.PHONY: world

# rule: test
# this rule build and test all libraries
test:
	@${MAKE} -C src test
	@${MAKE} -C prj test
.PHONY: test

# rule: devel
# this rule install the development file
devel:
	@${MAKE} -C cnf devel
	@${MAKE} -C tst devel
.PHONY: devel

# rule: doc
# this rule build the documentation

doc:
	@${MAKE} -C doc doc
	@${MAKE} -C src doc
	@${MAKE} -C prj doc
.PHONY: doc

# rule: distri
# this rule creates the distribution
distri:
	@$(MKDIR) $(BLDDST)
	@$(CP)    Makefile $(BLDDST)
	@${MAKE}  -C cnf distri
	@${MAKE}  -C src distri
	@${MAKE}  -C prj distri
	@${MAKE}  -C tst distri
	@${MAKE}  -C etc distri
	@${MAKE}  -C exp distri
	@${MAKE}  -C doc distri
.PHONY: distri

# rule: install
# this rule install the distribution
install:
	@${MAKE}  -C src install
	@${MAKE}  -C prj install
	@${MAKE}  -C etc install
.PHONY: install

# rule: publish
# this rule install the documentation
publish:
	@${MAKE}  -C doc publish
	@${MAKE}  -C src publish
	@${MAKE}  -C prj publish
.PHONY: publish

# rule: clean
# This rule cleans the distribution.
clean::
	@${MAKE} -C cnf clean
	@${MAKE} -C src clean
	@${MAKE} -C prj clean
	@${MAKE} -C tst clean
	@${MAKE} -C exp clean
	@${MAKE} -C doc clean
	@${MAKE} -C etc clean
.PHONY: clean

# rule: reset
# This rule reset the whole distribution and configuration
reset: clean
	@$(RMDIR) $(BLDDIR)
.PHONY: reset
