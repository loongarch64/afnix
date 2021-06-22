# ----------------------------------------------------------------------------
# - afnix-raid                                                               -
# - afnix android rule makefile definitions                                  -
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

# rule: android-x64-build
# build an android x64 specific version
android-x64-build:
	@$(TEST) -f bld/cnf/afnix-comp.mak && \
         $(TEST) -f bld/cnf/afnix-plat.mak || \
	./cnf/bin/afnix-setup -o --sdknam=android --cpunam=x64
	${MAKE} -C src all
.PHONY: android-x64-build

# rule: android-x64
# build an android x64 specific version
android-x64:
	@$(RMDIR)  bld/afnix-$(VERSION)-android-x64
	${MAKE}    distri SRCDST=afnix-$(VERSION)-android-x64
	${MAKE} -C bld/afnix-$(VERSION)-android-x64 android-x64-build
.PHONY: android-x64

# rule: android-ia32-build
# build an android ia32 specific version
android-ia32-build:
	@$(TEST) -f bld/cnf/afnix-comp.mak && \
         $(TEST) -f bld/cnf/afnix-plat.mak || \
	./cnf/bin/afnix-setup -o --sdknam=android --cpunam=ia32
	${MAKE} -C src all
.PHONY: android-ia32-build

# rule: android-ia32
# build an android ia32 specific distribution
android-ia32:
	@$(RMDIR)  bld/afnix-$(VERSION)-android-ia32
	${MAKE}    distri SRCDST=afnix-$(VERSION)-android-ia32
	${MAKE} -C bld/afnix-$(VERSION)-android-ia32 android-ia32-build
.PHONY: android-ia32

# rule: android-arm-build
# build an android arm (32) specific version
android-arm-build:
	@$(TEST) -f bld/cnf/afnix-comp.mak && \
         $(TEST) -f bld/cnf/afnix-plat.mak || \
	./cnf/bin/afnix-setup -o --sdknam=android --cpunam=arm
	${MAKE} -C src all
.PHONY: android-arm-build

# rule: android-arm
# build an android arm specific distribution
android-arm:
	@$(RMDIR)  bld/afnix-$(VERSION)-android-arm
	${MAKE}    distri SRCDST=afnix-$(VERSION)-android-arm
	${MAKE} -C bld/afnix-$(VERSION)-android-arm android-arm-build
.PHONY: android-arm

# rule: android-aarch64-build
# build an android arm (64) specific version
android-aarch64-build:
	@$(TEST) -f bld/cnf/afnix-comp.mak && \
         $(TEST) -f bld/cnf/afnix-plat.mak || \
	./cnf/bin/afnix-setup -o --sdknam=android --cpunam=aarch64
	${MAKE} -C src all
.PHONY: android-aarch64-build

# rule: android-aarch64
# build an android aarch64 specific distribution
android-aarch64:
	@$(RMDIR)  bld/afnix-$(VERSION)-android-aarch64
	${MAKE}    distri SRCDST=afnix-$(VERSION)-android-aarch64
	${MAKE} -C bld/afnix-$(VERSION)-android-aarch64 android-aarch64-build
.PHONY: android-aarch64-build

# rule: android
# build an android distribution
android: android-x64 android-ia32 android-arm android-aarch64
.PHONY: android
