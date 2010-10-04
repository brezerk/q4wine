#
# spec file for package q4wine (Version 0.120)
#
# Copyright (c) 2009, 2010 Kyrill Detinov
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#

Name:           q4wine
Version:        0.120
Release:        0
URL:            http://q4wine.brezblock.org.ua/
License:        GPLv3
Source:         %{name}-%{version}.tar.bz2
Group:          System/Emulators/PC
Summary:        Qt4 GUI for WINE
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
BuildRequires:  libqt4-devel
BuildRequires:  cmake
BuildRequires:  fuseiso
BuildRequires:  icoutils
BuildRequires:  update-desktop-files
BuildRequires:  fdupes
Requires:       wine
Requires:       sudo
Requires:       sqlite3
Requires:       fuseiso
Requires:       icoutils

%description
Q4Wine is an Qt4-based GUI for WINE. It will help you to manage wine prefixes
and installed applications.

General features:
- Can export Qt color theme into wine colors settings.
- Can easy work with different wine versions at the same time.
- Easy creating, deleting and managing prefixes (WINEPREFIX).
- Easy controlling for wine process.
- Autostart icons support.
- Easy cd-image use.
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
cmake .. -DCMAKE_INSTALL_PREFIX=%{_prefix} -DWITH_WINETRIKS=ON
%{__make} %{?_smp_mflags}

%install
pushd build
%{makeinstall}
popd
%fdupes -s %{buildroot}
%suse_update_desktop_file %{name}

%clean
%{__rm} -rf %{buildroot}

%files
%defattr(-,root,root)
%doc AUTHORS ChangeLog README
%{_datadir}/%{name}
%{_bindir}/*
%{_libdir}/%{name}
%doc %{_mandir}/man1/*
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png

%changelog

# kate: space-indent on; indent-width 4;
