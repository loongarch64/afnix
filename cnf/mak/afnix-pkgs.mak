# ----------------------------------------------------------------------------
# - afnix-pkgs                                                               -
# - afnix packages make file configuration                                   -
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
# - fedora definitions                                                       -
# ----------------------------------------------------------------------------

# check for a rpm package
ifeq ($(PKGNAM),rpm)
# check for bin overwrite
BINDIR		= $(PREFIX)/usr/bin
ifneq ($(PKGBIN),)
BINDIR		= $(PREFIX)$(PKGBIN)
endif
# check for lib overwrite
LIBDIR		= $(PREFIX)/usr/lib
ifeq ($(PROCBITS),64)
LIBDIR		= $(PREFIX)/usr/lib64
endif
ifneq ($(PKGLIB),)
LIBDIR		= $(PREFIX)$(PKGLIB)
endif
# check for project overwrite
PRJDIR		= $(LIBDIR)/afnix
ifneq ($(PKGPRJ),)
PRJDIR		= $(PREFIX)$(PKGPRJ)
endif
# check for header overwrite
HDRDIR		= $(PREFIX)/usr/include/afnix
ifneq ($(PKGHDR),)
HDRDIR		= $(PREFIX)$(PKGHDR)
endif
# check for etc overwrite
ETCDIR		= $(ALTDIR)/etc/afnix
ifneq ($(PKGETC),)
ETCDIR		= $(ALTDIR)$(PKGETC)
endif
# check for man overwrite
MANDIR		= $(PREFIX)/usr/share/man
ifneq ($(PKGMAN),)
MANDIR		= $(PREFIX)$(PKGMAN)
endif
# check for doc overwrite
DOCDIR		= $(PREFIX)/usr/share/doc
ifneq ($(PKGDOC),)
DOCDIR		= $(PREFIX)$(PKGDOC)
endif
# check for www overwrite
WWWDIR		= $(ALTDIR)/var/www/afnix
ifneq ($(PKGWWW),)
WWWDIR		= $(ALTDIR)$(PKGWWW)
endif
# check for srv overwrite
SRVDIR		= $(ALTDIR)/var/srv/afnix
ifneq ($(PKGSRV),)
SRVDIR		= $(ALTDIR)$(PKGSRV)
endif
endif

# ----------------------------------------------------------------------------
# - debian definitions                                                       -
# ----------------------------------------------------------------------------

# check for a deb package
ifeq ($(PKGNAM),deb)
# check for bin overwrite
BINDIR		= $(PREFIX)/usr/bin
ifneq ($(PKGBIN),)
BINDIR		= $(PREFIX)$(PKGBIN)
endif
# check for lib overwrite
LIBDIR		= $(PREFIX)/usr/lib
ifneq ($(PKGLIB),)
LIBDIR		= $(PREFIX)$(PKGLIB)
endif
# check for project overwrite
PRJDIR		= $(LIBDIR)/afnix
ifneq ($(PKGPRJ),)
PRJDIR		= $(PREFIX)$(PKGPRJ)
endif
# check for header overwrite
HDRDIR		= $(PREFIX)/usr/include/afnix
ifneq ($(PKGHDR),)
HDRDIR		= $(PREFIX)$(PKGHDR)
endif
# check for etc overwrite
ETCDIR		= $(ALTDIR)/etc/afnix
ifneq ($(PKGETC),)
ETCDIR		= $(ALTDIR)$(PKGETC)
endif
# check for man overwrite
MANDIR		= $(PREFIX)/usr/share/man
ifneq ($(PKGMAN),)
MANDIR		= $(PREFIX)$(PKGMAN)
endif
# check for doc overwrite
DOCDIR		= $(PREFIX)/usr/share/doc
ifneq ($(PKGDOC),)
DOCDIR		= $(PREFIX)$(PKGDOC)
endif
# check for www overwrite
WWWDIR		= $(ALTDIR)/var/www/afnix
ifneq ($(PKGWWW),)
WWWDIR		= $(ALTDIR)$(PKGWWW)
endif
# check for srv overwrite
SRVDIR		= $(ALTDIR)/var/srv/afnix
ifneq ($(PKGSRV),)
SRVDIR		= $(ALTDIR)$(PKGSRV)
endif
endif
