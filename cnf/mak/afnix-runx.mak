# ----------------------------------------------------------------------------
# - afnix-runx                                                               -
# - afnix unix rule makefile definitions                                     -
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

ifeq ($(CCMODE),debug)
  ENVFLAGS      = $(STDEVFLAGS) $(PLTEVFLAGS)
  CPPFLAGS      = $(STDCCFLAGS) $(PLTCCFLAGS) $(CPPCCFLAGS) $(DEBUGFLAGS)
  CXXFLAGS      = $(STDCCFLAGS) $(PLTCCFLAGS) $(CXXCCFLAGS) $(DEBUGFLAGS)
  CCDEFINE      = $(STDDEFINES) $(PLTDEFINES) $(DBGDEFINES)
endif

ifeq ($(CCMODE),optimized)
  ENVFLAGS      = $(STDEVFLAGS) $(PLTEVFLAGS)
  CPPFLAGS      = $(STDCCFLAGS) $(PLTCCFLAGS) $(CPPCCFLAGS) $(OPTCCFLAGS)
  CXXFLAGS      = $(STDCCFLAGS) $(PLTCCFLAGS) $(CXXCCFLAGS) $(OPTCCFLAGS)
  CCDEFINE      = $(STDDEFINES) $(PLTDEFINES) $(OPTDEFINES)
endif

ifeq ($(CCMODE),profiled)
  ENVFLAGS      = $(STDEVFLAGS) $(PLTEVFLAGS)
  CPPFLAGS      = $(STDCCFLAGS) $(PLTCCFLAGS) $(CPPCCFLAGS) $(PFLCCFLAGS)
  CXXFLAGS      = $(STDCCFLAGS) $(PLTCCFLAGS) $(CXXCCFLAGS) $(PFLCCFLAGS)
  CCDEFINE      = $(STDDEFINES) $(PLTDEFINES) $(PFLDEFINES)
endif

ifeq ($(CCMODE),coverage)
  ENVFLAGS      = $(STDEVFLAGS) $(PLTEVFLAGS)
  CPPFLAGS      = $(STDCCFLAGS) $(PLTCCFLAGS) $(CPPCCFLAGS) $(COVCCFLAGS)
  CXXFLAGS      = $(STDCCFLAGS) $(PLTCCFLAGS) $(CXXCCFLAGS) $(COVCCFLAGS)
  CCDEFINE      = $(STDDEFINES) $(PLTDEFINES) $(DBGDEFINES)
endif

PPFLAGS         = $(CPPFLAGS)   $(EXTCPPFLAGS)
XXFLAGS         = $(CXXFLAGS)   $(EXTCXXFLAGS)
DEFINES         = $(CCDEFINE)   $(EXTCCDEFINE)
PPINCLS		= $(STDINCLUDE) $(INCLUDE)
XXINCLS		= $(STDINCLUDE) $(EXTINCLUDES) $(INCLUDE)

# ----------------------------------------------------------------------------
# - project file rules                                                       -
# ----------------------------------------------------------------------------

HPPSRCS		= $(wildcard *.hpp)
TCCSRCS		= $(wildcard *.tcc)
CPPSRCS		= $(wildcard *.cpp)
CXXSRCS		= $(wildcard *.cxx)

CPPOBJS         = $(CPPSRCS:.cpp=.o)
CXXOBJS         = $(CXXSRCS:.cxx=.o)
CPPLOBJ		= $(CPPLSRC:.cpp=.o)
CXXLOBJ		= $(CXXLSRC:.cxx=.o)
OBJECTS         = $(CPPOBJS) $(CXXOBJS) $(CPPLOBJ) $(CXXLOBJ) $(EXTOBJS)

ifneq ($(TRGLIB),)
ARLIB		= lib$(TRGLIB).$(AREXT)
DYLIB		= lib$(TRGLIB).$(DYEXT)
endif

ifeq ($(LKMODE),soname)
SOMAJ		= $(DYLIB).$(MAJOR)
SOMIN		= $(DYLIB).$(MAJOR).$(MINOR)
SOVRS		= $(DYLIB).$(MAJOR).$(MINOR).$(PATCH)
endif

ifeq ($(LKMODE),dylib)
DYMAJ		= lib$(TRGLIB).$(MAJOR).$(DYEXT)
DYMIN		= lib$(TRGLIB).$(MAJOR).$(MINOR).$(DYEXT)
DYVRS		= lib$(TRGLIB).$(MAJOR).$(MINOR).$(PATCH).$(DYEXT)
endif

# mark the file as loaded
ifeq ($(AFNIX-RULE-LOAD),)
AFNIX-RULE-LOAD = 1
endif

# ----------------------------------------------------------------------------
# - compilation rules                                                        -
# ----------------------------------------------------------------------------

ifeq ($(CCRULE),no)
%.o : %.c
%.o : %.cpp
%.o : %.cxx
else
-include *.d

%.o   : %.c
	$(ENVFLAGS) $(C) $(XXFLAGS) $(DEFINES) $(XXINCLS) -o $@ -c $<

%.o   : %.cpp 
	$(ENVFLAGS) $(CC) $(PPFLAGS) $(DEFINES) $(PPINCLS) -o $@ -c $<

%.o   : %.cxx 
	$(ENVFLAGS) $(CC) $(XXFLAGS) $(DEFINES) $(XXINCLS) -o $@ -c $<
endif

# ----------------------------------------------------------------------------
# - archive rules                                                            -
# ----------------------------------------------------------------------------

ifneq ($(ARLIB),)

ifeq ($(PLATNAME),darwin)
$(ARLIB) : $(OBJECTS)
	@$(RM) $@
	$(ENVFLAGS) $(AR) $(ARFLAGS) $@ $(OBJECTS)
	$(ENVFLAGS) $(RANLIB) $@
	@$(MKDIR) $(BLDLIB)
	@$(CP) $@ $(BLDLIB)
else
$(ARLIB) : $(OBJECTS)
	@$(RM) $@
	$(ENVFLAGS) $(AR) $(ARFLAGS) $@ $(OBJECTS)
	@$(MKDIR) $(BLDLIB)
	@$(CP) $@ $(BLDLIB)
endif

install-arlib: $(ARLIB)
	@$(MKDIR) $(LIBDIR)
	@$(CP)    $(ARLIB) $(LIBDIR)
.PHONY: install-arlib
else
install-arlib: $(ARLIB)
	@$(MKDIR) $(LIBDIR)
.PHONY: install-arlib
endif

# ----------------------------------------------------------------------------
# - dynamic linking rules                                                    -
# ----------------------------------------------------------------------------

# dynamic linking rules
ifeq ($(LKTYPE),dynamic)

# use generic linking mode
ifeq ($(LKMODE),generic)
$(DYLIB) : $(OBJECTS)
	$(ENVFLAGS) $(LD) $(LDFLAGS) -o $@ $(OBJECTS) $(EXTLIB)
	@$(MKDIR)   $(BLDLIB)
	@$(CP)   $@ $(BLDLIB)

install-dylib : $(DYLIB)
	@$(MKDIR)       $(LIBDIR)
	@$(CP) $(DYLIB) $(LIBDIR)
.PHONY: install-dylib
endif

# use soname linking mode
ifeq  ($(LKMODE),soname)
$(SOVRS) : $(OBJECTS)
	$(ENVFLAGS) $(LD) $(LDFLAGS) -o $(SOVRS) $(OBJECTS) $(EXTLIB)

$(DYLIB) : $(SOVRS)
	@$(MKDIR)        $(BLDLIB)
	@$(CP) $(SOVRS)  $(BLDLIB)
	@$(RM) $(BLDLIB)/$(DYLIB)
	@$(RM) $(BLDLIB)/$(SOMAJ)
	@$(LN) $(SOVRS)  $(BLDLIB)/$(DYLIB)
	@$(LN) $(SOVRS)  $(BLDLIB)/$(SOMAJ)

install-dylib : $(DYLIB)
	@$(MKDIR)        $(LIBDIR)
	@$(CP) $(SOVRS)  $(LIBDIR)
	@$(RM) $(LIBDIR)/$(DYLIB)
	@$(RM) $(LIBDIR)/$(SOMAJ)
	@$(LN) $(SOVRS)  $(LIBDIR)/$(DYLIB)
	@$(LN) $(SOVRS)  $(LIBDIR)/$(SOMAJ)
.PHONY: install-dylib
endif

# use dylib linking mode
ifeq  ($(LKMODE),dylib)
$(DYVRS) : $(OBJECTS)
	@$(RM) $@
	$(ENVFLAGS) $(LD) $(LDFLAGS) -install_name $(DYMAJ) -o $(DYVRS) \
                    $(OBJECTS) $(EXTLIB)

$(DYLIB) : $(DYVRS)
	@$(MKDIR)        $(BLDLIB)
	@$(CP) $(DYVRS)  $(BLDLIB)
	@$(RM) $(BLDLIB)/$(DYLIB)
	@$(RM) $(BLDLIB)/$(DYMAJ)
	@$(RM) $(BLDLIB)/$(DYMIN)
	@$(LN) $(DYVRS)  $(BLDLIB)/$(DYLIB)
	@$(LN) $(DYVRS)  $(BLDLIB)/$(DYMAJ)
	@$(LN) $(DYVRS)  $(BLDLIB)/$(DYMIN)

install-dylib : $(DYLIB)
	@$(MKDIR)        $(LIBDIR)
	@$(CP) $(DYVRS)  $(LIBDIR)
	@$(RM) $(LIBDIR)/$(DYLIB)
	@$(RM) $(LIBDIR)/$(DYMAJ)
	@$(RM) $(LIBDIR)/$(DYMIN)
	@$(LN) $(DYVRS)  $(LIBDIR)/$(DYLIB)
	@$(LN) $(DYVRS)  $(LIBDIR)/$(DYMAJ)
	@$(LN) $(DYVRS)  $(LIBDIR)/$(DYMIN)
.PHONY: install-dylib
endif

# else clause for dynamic linking
else
install-dylib:
	@$(MKDIR) $(LIBDIR)
.PHONY: install-dylib
endif

install-lib: install-arlib install-dylib
.PHONY: install-lib

# ----------------------------------------------------------------------------
# - library rules                                                            -
# ----------------------------------------------------------------------------

ifneq ($(HPPSRCS),)
build-hpp : $(HPPSRCS)
	@$(MKDIR) $(BLDHDR)/$(TRGDIR)
	@for i in *.hpp; do\
           $(CMP) $$i $(BLDHDR)/$(TRGDIR)/$$i || \
	   $(CP)  $$i $(BLDHDR)/$(TRGDIR)/$$i; \
         done
else
build-hpp:
.PHONY: build-hpp
endif

ifneq ($(TCCSRCS),)
build-tcc : $(TCCSRCS)
	@$(MKDIR) $(BLDHDR)/$(TRGDIR)
	@for i in *.tcc; do\
           $(CMP) $$i $(BLDHDR)/$(TRGDIR)/$$i || \
	   $(CP)  $$i $(BLDHDR)/$(TRGDIR)/$$i; \
         done
else
build-hpp:
.PHONY: build-tcc
endif

ifneq ($(TRGLIB),)
ifeq  ($(LKTYPE),dynamic)
$(TRGLIB) : $(ARLIB) $(DYLIB) build-hpp
endif
ifeq  ($(LKTYPE),static)
$(TRGLIB) : $(ARLIB) build-hpp
endif
endif

# ----------------------------------------------------------------------------
# - target rules                                                             -
# ----------------------------------------------------------------------------

ifneq ($(TARGETS),)
targets: $(TARGETS)
	@$(MKDIR) $(BLDBIN)
	@$(CP)    $(TARGETS) $(BLDBIN)
.PHONY: targets

ifeq ($(LKTYPE),dynamic)
$(TARGETS) : $(OBJECTS)
	$(ENVFLAGS) $(LK) $(LKFLAGS) -o $@ $(OBJECTS) $(EXELIBS) $(EXESLIB)
endif
ifeq ($(LKTYPE),static)
$(TARGETS) : $(OBJECTS)
	$(ENVFLAGS) $(LK) $(LKFLAGS) -o $@ $(OBJECTS) $(STALIBS) $(EXESLIB)
endif
endif

# ----------------------------------------------------------------------------
# - testers rules                                                            -
# ----------------------------------------------------------------------------

ifneq ($(TESTALS),)
$(TESTALS) :
	@$(AEXEC) -v --prefix=$(AXIDIR) --libdir=$(AXILIB) \
                     --binexe=$(AXIEXE) --binopt=$(AXIOPT) $@
.PHONY: $(TESTALS)

test: $(TESTALS)
.PHONY: test
endif

ifneq ($(TESTERS),)
$(TESTERS) : % : %.o
	$(LK) $(LKFLAGS) -o $@ $@.o $(EXELIBS) $(EXESLIB)

test: $(TESTERS:%=%.exe)
ifeq ($(DLYPATH),)
$(TESTERS:%=%.exe): %.exe : %
	@$(BEXEC) -v --prefix=$(BLDDIR) $<
else
$(TESTERS:%=%.exe): %.exe : %
	@$(BEXEC) -v --prefix=$(BLDDIR) --libdir=$(DLYPATH) $<
endif
.PHONY: test $(TESTERS:%=%.exe)
endif

# ----------------------------------------------------------------------------
# - status rule                                                              -
# ----------------------------------------------------------------------------

status::
	@echo 'build version          :' $(VERSION)
	@echo 'prefix                 :' $(PREFIX)
	@echo 'alternate directory    :' $(ALTDIR)
	@echo 'bin directory (BINDIR) :' $(BINDIR)
	@echo 'lib directory (LIBDIR) :' $(LIBDIR)
	@echo 'hdr directory (HDRDIR) :' $(HDRDIR)
	@echo 'doc directory (DOCDIR) :' $(DOCDIR)
	@echo 'man directory (MANDIR) :' $(MANDIR)
	@echo 'etc directory (ETCDIR) :' $(ETCDIR)
	@echo 'prj directory (PRJDIR) :' $(PRJDIR)
.PHONY: status

# ----------------------------------------------------------------------------
# - generic clean                                                            -
# ----------------------------------------------------------------------------

clean::
	@$(RM) *.o *.d *~ core *.so *.so.* *.a *.dylib *.bb *.bbg *.da
	@$(RM) core *.core core.* .gdb_history gmon.out
.PHONY: clean
