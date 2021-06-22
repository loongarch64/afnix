# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix:wgt module doc makefile                                            -
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
RULEFILE	= $(MAKDIR)/afnix-docs.mak
include		  $(CONFFILE)

# ----------------------------------------------------------------------------
# - project configuration                                                    -
# ----------------------------------------------------------------------------

DSTDIR		= $(BLDDST)/src/mod/wgt/doc
MANSEC		= $(MANDIR)/man3
MANUAL		= afnix-us-wgt.xml
MANMAN		= afnix-wgt.3

# ----------------------------------------------------------------------------
# - project rules                                                            -
# ----------------------------------------------------------------------------

# rule: all
# this rule is the default rule which build the documentation

all: doc
.PHONY: doc

# include: rule.mak
# this rule includes the platform dependant rules

include $(RULEFILE)

# rule: distri
# this rule install the distribution

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
	@$(CP)    *.xml    $(DSTDIR)
.PHONY: distri

# rule: publish
# this rule install the documentation

publish: doc
	@$(MKDIR) $(DOCDIR)
	@$(CP)    $(XHTTRG) $(DOCDIR)/$(XHTMAN)
	@$(MKDIR) $(MANSEC)
	@$(CP)    $(MANTRG) $(MANSEC)/$(MANMAN)
.PHONY: publish

# rule: clean
# this rule clean the directory

clean::
	@$(RM) $(XHTTRG) $(MANTRG) $(TEXTRG)
