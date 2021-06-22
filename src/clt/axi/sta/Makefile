# ----------------------------------------------------------------------------
# - Makefile                                                                 -
# - afnix interpreter (static version) client makefile                       -
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
RULEFILE	= $(MAKDIR)/afnix-rule.mak
include		  $(CONFFILE)

# ----------------------------------------------------------------------------
# project configurationn                                                     -
# ----------------------------------------------------------------------------

DSTDIR		= $(BLDDST)/src/clt/axi/sta
INCLUDE         = -I.             \
                  -I$(BLDHDR)/bce \
                  -I$(BLDHDR)/cda \
                  -I$(BLDHDR)/csm \
                  -I$(BLDHDR)/dip \
                  -I$(BLDHDR)/phy \
                  -I$(BLDHDR)/svg \
                  -I$(BLDHDR)/tls \
                  -I$(BLDHDR)/wax \
                  -I$(BLDHDR)/xpe \
                  -I$(BLDHDR)/gfx \
                  -I$(BLDHDR)/itu \
                  -I$(BLDHDR)/mth \
                  -I$(BLDHDR)/net \
                  -I$(BLDHDR)/nwg \
                  -I$(BLDHDR)/sec \
                  -I$(BLDHDR)/sio \
                  -I$(BLDHDR)/sps \
                  -I$(BLDHDR)/sys \
                  -I$(BLDHDR)/txt \
                  -I$(BLDHDR)/wgt \
                  -I$(BLDHDR)/xml \
                  -I$(BLDHDR)/eng \
                  -I$(BLDHDR)/std \
                  -I$(BLDHDR)/plt
EXELIBS         = $(BLDLIB)/libafnix-bce.a \
		  $(BLDLIB)/libafnix-cda.a \
		  $(BLDLIB)/libafnix-csm.a \
		  $(BLDLIB)/libafnix-dip.a \
		  $(BLDLIB)/libafnix-phy.a \
		  $(BLDLIB)/libafnix-svg.a \
		  $(BLDLIB)/libafnix-tls.a \
		  $(BLDLIB)/libafnix-wax.a \
		  $(BLDLIB)/libafnix-xpe.a \
		  $(BLDLIB)/libafnix-gfx.a \
		  $(BLDLIB)/libafnix-itu.a \
		  $(BLDLIB)/libafnix-mth.a \
		  $(BLDLIB)/libafnix-net.a \
                  $(BLDLIB)/libafnix-nwg.a \
                  $(BLDLIB)/libafnix-sec.a \
                  $(BLDLIB)/libafnix-sio.a \
                  $(BLDLIB)/libafnix-sps.a \
		  $(BLDLIB)/libafnix-sys.a \
	 	  $(BLDLIB)/libafnix-txt.a \
	 	  $(BLDLIB)/libafnix-wgt.a \
	 	  $(BLDLIB)/libafnix-xml.a \
		  $(BLDLIB)/libafnix-eng.a \
	          $(BLDLIB)/libafnix-std.a \
		  $(BLDLIB)/libafnix-plt.a \
	          $(STDLIBS)
STALIBS		= $(EXELIBS)
TARGETS		= axi

# ----------------------------------------------------------------------------
# - project rules                                                            -
# ----------------------------------------------------------------------------

# rule: all
# this rule is the default rule which call the target rule

all: targets
.PHONY: all

# include: rule.mak
# this rule includes the platform dependant rules

include $(RULEFILE)

# rule: distri
# this rule install the exe distribution files

distri:
	@$(MKDIR) $(DSTDIR)
	@$(CP)    Makefile $(DSTDIR)
	@$(CP)    *.cpp    $(DSTDIR)
.PHONY: distri

# rule: install
# this rule install the distribution

install:
	@$(MKDIR) $(BINDIR)
	@$(CP)    $(TARGETS) $(BINDIR)
.PHONY: install

# rule: clean
# local clean rule

clean::
	@$(RM) $(TARGETS)
