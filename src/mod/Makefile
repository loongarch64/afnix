# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix modules makefile                                                   -
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

DSTDIR		= $(BLDDST)/src/mod

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
	@${MAKE} -C gfx all
	@${MAKE} -C itu all
	@${MAKE} -C mth all
	@${MAKE} -C net all
	@${MAKE} -C nwg all
	@${MAKE} -C sec all
	@${MAKE} -C sio all
	@${MAKE} -C sps all
	@${MAKE} -C sys all
	@${MAKE} -C txt all
	@${MAKE} -C xml all
	@${MAKE} -C wgt all
.PHONY: build

# rule: test
# this rule build and test all libraries

test:
	@${MAKE} -C gfx test
	@${MAKE} -C itu test
	@${MAKE} -C mth test
	@${MAKE} -C net test
	@${MAKE} -C nwg test
	@${MAKE} -C sec test
	@${MAKE} -C sio test
	@${MAKE} -C sps test
	@${MAKE} -C sys test
	@${MAKE} -C txt test
	@${MAKE} -C xml test
	@${MAKE} -C wgt test
.PHONY: test

# rule: doc
# this rule builds the documentation

doc:
	@${MAKE} -C gfx doc
	@${MAKE} -C itu doc
	@${MAKE} -C mth doc
	@${MAKE} -C net doc
	@${MAKE} -C nwg doc
	@${MAKE} -C sec doc
	@${MAKE} -C sio doc
	@${MAKE} -C sps doc
	@${MAKE} -C sys doc
	@${MAKE} -C txt doc
	@${MAKE} -C xml doc
	@${MAKE} -C wgt doc
.PHONY: doc

# rule: distri
# this rule install the mod distribution files

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
	@${MAKE} -C gfx distri
	@${MAKE} -C itu distri
	@${MAKE} -C mth distri
	@${MAKE} -C net distri
	@${MAKE} -C nwg distri
	@${MAKE} -C sec distri
	@${MAKE} -C sio distri
	@${MAKE} -C sps distri
	@${MAKE} -C sys distri
	@${MAKE} -C txt distri
	@${MAKE} -C xml distri
	@${MAKE} -C wgt distri
.PHONY: distri

# rule: install
# this rule install the distribution

install:
	@${MAKE} -C gfx install
	@${MAKE} -C itu install
	@${MAKE} -C mth install
	@${MAKE} -C net install
	@${MAKE} -C nwg install
	@${MAKE} -C sec install
	@${MAKE} -C sio install
	@${MAKE} -C sps install
	@${MAKE} -C sys install
	@${MAKE} -C txt install
	@${MAKE} -C xml install
	@${MAKE} -C wgt install
.PHONY: install

# rule: publish
# this rule install the distribution

publish:
	@${MAKE} -C gfx publish
	@${MAKE} -C itu publish
	@${MAKE} -C mth publish
	@${MAKE} -C net publish
	@${MAKE} -C nwg publish
	@${MAKE} -C sec publish
	@${MAKE} -C sio publish
	@${MAKE} -C sps publish
	@${MAKE} -C sys publish
	@${MAKE} -C txt publish
	@${MAKE} -C xml publish
	@${MAKE} -C wgt publish
.PHONY: publish

# rule: clean
# this rule clean all source directories

clean::
	@${MAKE} -C gfx clean
	@${MAKE} -C itu clean
	@${MAKE} -C mth clean
	@${MAKE} -C net clean
	@${MAKE} -C nwg clean
	@${MAKE} -C sec clean
	@${MAKE} -C sio clean
	@${MAKE} -C sps clean
	@${MAKE} -C sys clean
	@${MAKE} -C txt clean
	@${MAKE} -C xml clean
	@${MAKE} -C wgt clean
.PHONY: clean
