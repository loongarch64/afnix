# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix doc makefile                                                       -
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
RULEFILE	= $(MAKDIR)/afnix-docs.mak
include		  $(CONFFILE)

# ----------------------------------------------------------------------------
# - project configuration                                                    -
# ----------------------------------------------------------------------------

DSTDIR		= $(BLDDST)/doc

# ----------------------------------------------------------------------------
# - project rules                                                            -
# ----------------------------------------------------------------------------

# rule: all
# this rule is the default rule which builds the documentation

all: doc
.PHONY: all

# include: rule.mak
# this rule includes the platform dependant rules

include $(RULEFILE)

# rule: doc
# this rule prepare the documentation

doc:
	@${MAKE} -C xml doc
.PHONY: doc

# rule: distri
# this rule creates the distribution

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
	@${MAKE}  -C dtd distri
	@${MAKE}  -C etc distri
	@${MAKE}  -C xml distri
.PHONY: distri

# rule: publish
# this rule install the documentation

publish:
	@${MAKE} -C etc publish
	@${MAKE} -C xml publish
.PHONY: publish

# rule: clean
# This rule cleans the directories

clean::
	@${MAKE} -C dtd clean
	@${MAKE} -C etc clean
	@${MAKE} -C xml clean
