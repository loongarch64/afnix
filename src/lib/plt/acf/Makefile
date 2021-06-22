# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - standard platform configuration makefile                                 -
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
include		  $(CONFFILE)

# ----------------------------------------------------------------------------
# - project configuration                                                    -
# ----------------------------------------------------------------------------

TRGDIR		= plt
DSTDIR		= $(BLDDST)/src/lib/plt/acf

# ----------------------------------------------------------------------------
# - project rules                                                            -
# ----------------------------------------------------------------------------

# rule: all
# this rule is the default rule which call the build rule

all: ccnf.hpp
.PHONY: all

# rule: ccnf.hpp
# generate the ccnf.hpp rule file

ccnf.hpp: ccnf
	@./ccnf > ccnf.hpp
	@$(MKDIR) $(BLDHDR)/$(TRGDIR)
	@$(CP) $@ $(BLDHDR)/$(TRGDIR)

# rule: ccnf
# build the ccnf generator

ccnf: ccnf.cpp
	@$(CN) $(CNFDEFINES) -o ccnf ccnf.cpp

# rule: distri
# this rule install the acf directory in the distribution

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
	@$(CP)    ccnf.cpp $(DSTDIR)
.PHONY: distri

# rule: clean
# clean this directory

clean:
	@$(RM) *~ ccnf ccnf.hpp core
