# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix src makefile                                                       -
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

TOPDIR		= ..
MAKDIR		= $(TOPDIR)/cnf/mak
CONFFILE	= $(MAKDIR)/afnix-conf.mak
RULEFILE	= $(MAKDIR)/afnix-rule.mak
include		  $(CONFFILE)

# ----------------------------------------------------------------------------
# - project configuration                                                    -
# ----------------------------------------------------------------------------

DSTDIR		= $(BLDDST)/src

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
	@${MAKE} -C lib all
	@${MAKE} -C mod all
	@${MAKE} -C srv all
	@${MAKE} -C clt all
.PHONY: build

# rule: interp
# this rule build the base interpreter

interp:
	@${MAKE} -C lib all
	@${MAKE} -C clt interp
.PHONY: build

# rule: test
# this rule build and test all libraries

test:
	@${MAKE} -C lib test
	@${MAKE} -C clt test
	@${MAKE} -C mod test
	@${MAKE} -C srv test
.PHONY: test

# rule: doc
# this rule build the documentation

doc:
	@${MAKE} -C mod doc
	@${MAKE} -C srv doc
	@${MAKE} -C clt doc
.PHONY: doc

# rule: distri
# this rule install the src distribution files

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
	@${MAKE}  -C lib distri
	@${MAKE}  -C mod distri
	@${MAKE}  -C srv distri
	@${MAKE}  -C clt distri
.PHONY: distri

# rule: install
# this rule install the distribution

install:
	@${MAKE}  -C lib install
	@${MAKE}  -C mod install
	@${MAKE}  -C srv install
	@${MAKE}  -C clt install
.PHONY: install

# rule: publish
# this rule install the documentation

publish:
	@${MAKE}  -C mod publish
	@${MAKE}  -C srv publish
	@${MAKE}  -C clt publish
.PHONY: publish

# rule: clean
# this rule clean all source directories

clean::
	@${MAKE} -C lib clean
	@${MAKE} -C mod clean
	@${MAKE} -C srv clean
	@${MAKE} -C clt clean
.PHONY: clean
