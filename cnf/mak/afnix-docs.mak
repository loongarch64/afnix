# ----------------------------------------------------------------------------
# - afnix-docs                                                               -
# - afnix documentation rule makefile configuration                          -
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

# ----------------------------------------------------------------------------
# - documentation definitions                                                -
# ----------------------------------------------------------------------------

DOCETC		= $(DIRDOC)/etc
DOCDTD		= $(DIRDOC)/dtd
DOCXML		= $(DIRDOC)/xml

ifeq ($(REFCSS),)
REFCSS		= $(DOCETC)/std-us-style.css
endif
ifeq ($(REFDTD),)
REFDTD		= $(DOCDTD)/std-us-afnix.dtd
endif
ifeq ($(REFIMG),)
REFIMG		= $(DIRETC)/std/std-us-afnix.png
endif
ifeq ($(ADPAXL),)
ADPAXL		= $(BLDPRJ)/axl/afnix-std-adp.axl
endif
ifeq ($(ADPPFX),)
ADPPFX		= $(BLDDIR)
endif
ifeq ($(ADPLIB),)
ADPLIB		= $(BLDLIB)
endif
ifeq ($(ADPEXE),)
ADPEXE		= "axi"
endif

# ----------------------------------------------------------------------------
# - documentation definitions                                                -
# ----------------------------------------------------------------------------

XMLMAN		= $(wildcard *.xml)
ifneq ($(MANUAL),)
ifneq ($(XHTTRG)),)
XHTTRG          = $(MANUAL:.xml=.xht)
endif
ifneq ($(MANTRG)),)
MANTRG          = $(MANUAL:.xml=.man)
endif
ifneq ($(TEXTRG)),)
TEXTRG          = $(MANUAL:.xml=.tex)
endif
endif

# ----------------------------------------------------------------------------
# - documentation control                                                    -
# ----------------------------------------------------------------------------

ifeq ($(XHTMAN),)
XHTMAN		= $(XHTTRG)
endif
ifeq ($(MANMAN),)
MANMAN		= $(MANTRG)
endif
ifeq ($(TEXMAN),)
TEXMAN		= $(TEXTRG)
endif

DOCTRG		= xht man
XHTOPT		= "-i $(ADPAXL) cmd-adp-print -x -o $(XHTTRG) $(ADPOPT)"
MANOPT		= "-i $(ADPAXL) cmd-adp-print -m -o $(MANTRG) $(ADPOPT)"
TEXOPT		= "-i $(ADPAXL) cmd-adp-print -l -o $(TEXTRG) $(ADPOPT)"

# ----------------------------------------------------------------------------
# - documentation rules                                                      -
# ----------------------------------------------------------------------------

ifneq ($(MANUAL),)
doc: $(DOCTRG)
.PHONY: doc

# rule: xht
# this rule generate a xhtml documentation

xht: $(XHTTRG)
.PHONY: xht

$(XHTTRG) : $(XMLMAN)
	@$(AEXEC) --prefix=$(ADPPFX) --libdir=$(ADPLIB) \
                  --binexe=$(ADPEXE) --binopt=$(XHTOPT) $(MANUAL)
	@$(MKDIR) $(DOCXHT)
	@$(CP) $(REFCSS) $(DOCXHT)
	@$(CP) $(REFIMG) $(DOCXHT)
	@$(CP) $(XHTTRG) $(DOCXHT)/$(XHTMAN)

# rule: man
# this rule generate a groff documentation

man: $(MANTRG)
.PHONY: man

$(MANTRG) : $(XMLMAN)
	@$(AEXEC) --prefix=$(ADPPFX) --libdir=$(ADPLIB) \
                  --binexe=$(ADPEXE) --binopt=$(MANOPT) $(MANUAL)
	@$(MKDIR) $(DOCMAN)
	@$(CP) $(MANTRG) $(DOCMAN)/$(MANMAN)

# rule: tex
# this rule generate a latex documentation

tex: $(TEXTRG)
.PHONY: tex

$(TEXTRG) : $(XMLMAN)
	@$(AEXEC) --prefix=$(ADPPFX) --libdir=$(ADPLIB) \
                  --binexe=$(ADPEXE) --binopt=$(TEXOPT) $(MANUAL)
	@$(MKDIR) $(DOCTEX)
	@$(CP) $(TEXTRG) $(DOCTEX)/$(TEXMAN)

endif

# ----------------------------------------------------------------------------
# - generic clean                                                            -
# ----------------------------------------------------------------------------

clean::
	@$(RM) *~ core *.core
.PHONY: clean
