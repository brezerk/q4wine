#
# spec file for package q4wine (Version 0.118)
#
# Copyright (c) 2009, 2010 Kyrill Detinov
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#

# norootforbuild

# Do we use icoutils to extract icons from M$ .exe and .dll files? [0,1]
%define with_icons 1

Name:           q4wine
Version:	0.118
Release:	0
URL:		http://q4wine.brezblock.org.ua/
License:	GPLv3
Source:		%{name}-%{version}.tar.bz2
Group:		System/Emulators/PC
Summary:	Qt4 GUI for WINE
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
BuildRequires:  libqt4-devel >= 4.4.1 cmake >= 2.6 update-desktop-files fdupes
Requires:	wine sudo sqlite3 fuseiso
%if %{with_icons}
BuildRequires:  icoutils
Requires:	icoutils
%endif

%description
Q4Wine is an Qt4-based GUI for WINE. It will help you to manage wine prefixes
and installed applications.

General features:
- Can export Qt color theme into wine colors settings.
- Can easy work with different wine versions at the same time.
- Easy creating, deleting and managing prefixes (WINEPREFIX).
- Easy controlling for wine process.
- Autostart icons support.
- Easy CD-image use.
- You can extract icons from PE files (.exe .dll).
- Easy backup and restore for managed prefixes.
- Wine AppDB browser.
- Logging subsystem.
- Winetricks support.

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
[ %{buildroot} != "/" ] && %{__rm} -rf %{buildroot}

%files
%defattr(-,root,root)
%doc AUTHORS ChangeLog README
%{_datadir}/%{name}
%{_bindir}/*
%{_libdir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png

%changelog
