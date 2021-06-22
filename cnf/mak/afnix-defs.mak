# ----------------------------------------------------------------------------
# - afnix-defs                                                               -
# - afnix definition make file configuration                                 -
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
# - version                                                                  -
# ----------------------------------------------------------------------------

VERSION		= $(MAJOR).$(MINOR).$(PATCH)

# ----------------------------------------------------------------------------
# - distribution path definition                                             -
# ----------------------------------------------------------------------------

DIRCNF          = $(TOPDIR)/cnf
DIRSRC		= $(TOPDIR)/src
DIRTST		= $(TOPDIR)/tst
DIRDOC		= $(TOPDIR)/doc
DIRETC		= $(TOPDIR)/etc
DIREXP		= $(TOPDIR)/exp

# ----------------------------------------------------------------------------
# - build path definition                                                    -
# ----------------------------------------------------------------------------

BLDDIR		= $(TOPDIR)/bld
BLDBIN		= $(BLDDIR)/bin
BLDLIB		= $(BLDDIR)/lib
BLDPRJ		= $(BLDDIR)/prj
BLDHDR		= $(BLDDIR)/hdr
BLDDVL		= $(BLDDIR)/dvl
BLDPKG		= $(BLDDIR)/pkg

SRCDST		= afnix-src-$(VERSION)
DOCDST		= afnix-doc-$(VERSION)

BLDDST		= $(BLDDIR)/$(SRCDST)
BLDDOC		= $(BLDDIR)/$(DOCDST)
DOCXHT		= $(BLDDOC)/xht
DOCMAN		= $(BLDDOC)/man
DOCTEX		= $(BLDDOC)/tex

BLDAXL		= $(BLDPRJ)/axl

# ----------------------------------------------------------------------------
# - install path definition                                                  -
# ----------------------------------------------------------------------------

BINDIR		= $(PREFIX)/bin
LIBDIR		= $(PREFIX)/lib
PRJDIR		= $(PREFIX)/lib/afnix
HDRDIR		= $(PREFIX)/include/afnix
DOCDIR		= $(SHRDIR)/doc/afnix
MANDIR		= $(SHRDIR)/man
ETCDIR		= $(ALTDIR)/etc/afnix
WWWDIR		= $(ALTDIR)/var/www/afnix
SRVDIR		= $(ALTDIR)/var/srv/afnix

# ----------------------------------------------------------------------------
# - other shared commands and scripts                                        -
# ----------------------------------------------------------------------------

AEXEC		= $(DIRCNF)/bin/afnix-aexec
BEXEC		= $(DIRCNF)/bin/afnix-bexec

# ----------------------------------------------------------------------------
# - default testing command and options                                      -
# ----------------------------------------------------------------------------

TSTREF		= $(DIRTST)/ref
TSTMOD		= $(DIRTST)/mod
AXIDIR		= $(BLDDIR)
AXILIB		= $(BLDLIB)
AXIEXE		= "axi"
AXIOPT		= "-e UTF-8 -f assert"

# ----------------------------------------------------------------------------
# - configuration definitions                                                -
# ----------------------------------------------------------------------------

CNFDEFINES	= -DPLATNAME=\"$(PLATNAME)\" 
CNFDEFINES     += -DPLATVERS=\"$(PLATVERS)\"
CNFDEFINES     += -DPLATVMAJ=\"$(PLATVMAJ)\"
CNFDEFINES     += -DPLATVMIN=\"$(PLATVMIN)\"
CNFDEFINES     += -DPROCNAME=\"$(PROCNAME)\"
CNFDEFINES     += -DPLTMAJOR=\"$(MAJOR)\"
CNFDEFINES     += -DPLTMINOR=\"$(MINOR)\"
CNFDEFINES     += -DPLTPATCH=\"$(PATCH)\"
CNFDEFINES     += -DLINKTYPE=\"$(LKTYPE)\"

