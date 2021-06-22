# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix doc vol makefile                                                   -
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
RULEFILE	= $(MAKDIR)/afnix-docs.mak
include		  $(CONFFILE)

# ----------------------------------------------------------------------------
# - project configuration                                                    -
# ----------------------------------------------------------------------------

DSTDIR		= $(BLDDST)/doc/xml/vol

# ----------------------------------------------------------------------------
# - project rules                                                            -
# ----------------------------------------------------------------------------

# rule: all
# this rule is the default rule which does nothing

all: doc
.PHONY: all

# include: rule.mak
# this rule includes the platform dependant rules

include $(RULEFILE)

# rule: doc
# this rule prepare the documentation

doc:
	@${MAKE} -C 000 doc
	@${MAKE} -C 001 doc
	@${MAKE} -C 002 doc
.PHONY: doc

# rule: distri
# this rule install the files in the distribution

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
	@${MAKE}  -C 000 distri
	@${MAKE}  -C 001 distri
	@${MAKE}  -C 002 distri
.PHONY: distri

# rule: publish
# this rule install the documentation

publish:
	@${MAKE} -C 000 publish
	@${MAKE} -C 001 publish
	@${MAKE} -C 002 publish
.PHONY: publish

# rule: clean
# This rule cleans the directories

clean::
	@${MAKE} -C 000 clean
	@${MAKE} -C 001 clean
	@${MAKE} -C 002 clean
