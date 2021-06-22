Name:		afnix
Version:	3.4.0
Release:	1%{?dist}
License:	AFNIX License
URL:		http://www.afnix.org/
BuildRequires:	ncurses-devel
Source:		http://www.afnix.org/ftp/%{name}-src-%{version}.tgz
Summary:	Version 3.4 of the Afnix Writing System

%description
The Afnix writing system is a multithreaded functional writing system with
dynamic symbol bindings that support the object oriented paradigm. The system
features a state of the art run-time engine that supports both 32 and 64 bits
platforms.
            
%package libs
Summary:	Library for afnix
Provides:	afnix = %{version}

%description libs
This package contains the afnix development libraries

%package devel
Summary:	Development files for afnix
Requires:	%{name} = %{version}

%description devel
This package contains the header files and libraries for an application
embedded afnix.

%prep
%setup -q -n %{name}-src-%{version}

%build
%{_builddir}/%{name}-src-%{version}/cnf/bin/afnix-setup -o \
  --prefix=%{buildroot} --pkgnam=rpm \
  --pkgbin=%{_bindir} --pkglib=%{_libdir} --pkghdr=%{_includedir}/afnix \
  --pkgetc=%{_sysconfdir} --pkgman=%{_mandir} --pkgdoc=%{_defaultdocdir}
make status
make %{?_smp_mflags} world

%install
make install

%post   -p /sbin/ldconfig libs

%postun -p /sbin/ldconfig libs

%files
%{_bindir}/axi
%{_bindir}/axl
%{_bindir}/axc
%{_bindir}/axd
%{_libdir}/afnix/axl/*.axl

%files libs
%{_libdir}/libafnix-*.a
%{_libdir}/libafnix-*.so.*

%files devel
%{_includedir}/afnix/*/*.hpp
%{_libdir}/libafnix-*.so

%changelog
* Mon Jan 04 2021 Afnix Release Engineering <contact@afnix.org> - 3.4.0-1
- Initial build for Fedora 33
* Fri Aug 07 2020 Afnix Release Engineering <contact@afnix.org> - 3.3.0-1
- Initial build for Fedora 32
* Thu Mar 12 2020 Afnix Release Engineering <contact@afnix.org> - 3.2.0-1
- Initial build for Fedora 31
* Sat Dec 28 2019 Afnix Release Engineering <contact@afnix.org> - 3.1.0-1
- Initial build for Fedora 31
* Mon Sep 23 2019 Afnix Release Engineering <contact@afnix.org> - 3.0.3-1
- Initial build for Fedora 30
* Sat Mar 23 2019 Afnix Release Engineering <contact@afnix.org> - 3.0.0-1
- Initial build for Fedora 29
