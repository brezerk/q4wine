#
# spec file for package q4wine (Version 0.113rc3)
#
# Copyright (c) 2009 Kyrill Detinov
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#

# norootforbuild

# Do we use icoutils to extract icons from M$ .exe and .dll files? [0,1]
%define with_icons 1

Name:           q4wine
Version:	0.113
Release:	0
URL:		http://q4wine.brezblock.org.ua/
License:	GPLv3
Source:		%{name}-%{version}.tar.bz2
Group:		System/Emulators/PC
Summary:	Qt4 GUI for WINE
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
BuildRequires:  libqt4-devel >= 4.4 cmake update-desktop-files fdupes
Requires:	wine sudo sqlite3 fuseiso
%if %{with_icons}
BuildRequires:  icoutils
Requires:	icoutils
%endif

%description
Q4Wine is an Qt4-based GUI for WINE. It will help you to manage wine prefixes
and installed applications.

General features:
- Works with different wine versions at same time;
- Creating, deleting and managing prefixes (WINEPREFIX);
- Easy controlling for wine process;
- Easy installer wizard for wine applications; (Not yet. Wait for v. 0.120)
- Autostart icons support;
- Support of ISO images;
- Icons can be extracted from PE files (.exe, .dll).

Authors:
--------
    Alexey Malakhov <brezerk@gmail.com>

%prep
%setup -q

%build
%{__mkdir} build
cd build
%if %{with_icons}
cmake .. -DCMAKE_INSTALL_PREFIX:PATH=/usr -DWITH_WINETRIKS=ON
%else
cmake .. -DCMAKE_INSTALL_PREFIX:PATH=/usr -DWITH_WINETRIKS=ON -DWITH_ICOTOOLS=OFF
%endif
%{__make} %{?jobs:-j %jobs}

%install
pushd build
%{makeinstall}
popd
%fdupes -s %{buildroot}
%suse_update_desktop_file -i %{name}

%clean
[ "%{buildroot}" != "/" ] && %{__rm} -rf %{buildroot}

%files
%defattr(-,root,root)
%doc ChangeLog README
%{_datadir}/%{name}
%{_bindir}/*
%{_libdir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png

%changelog
* Fri Oct 05 2009 Kyrill Detinov <lazy.kent.suse@gmail.com> - 0.113
- update to 0.113
- added choice to build with/without icoutils support
- added fuseiso to Requires
- removed INSTALL from docs
- corrected Summary
* Sat Jun 20 2009 Kyrill Detinov <lazy.kent.suse@gmail.com> - 0.112r1
- change compiling outside of the source tree
* Sun May 17 2009 Kyrill Detinov <lazy.kent.suse@gmail.com> - 0.112r1
- compilied with winetricks support
* Fri May 15 2009 Kyrill Detinov <lazy.kent.suse@gmail.com> - 0.112r1
- desktop file updated
* Wed May 06 2009 Kyrill Detinov <lazy.kent.suse@gmail.com> - 0.112r1
- updated to v0.112r1
