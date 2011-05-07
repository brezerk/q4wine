#
# spec file for package q4wine
#
# Copyright (c) 2011 SUSE LINUX Products GmbH, Nuernberg, Germany.
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.

# Please submit bugfixes or comments via http://bugs.opensuse.org/
#



Name:           q4wine
Version:        0.120+r1
Release:        1
License:        GPLv3
Summary:        Qt4 GUI for WINE
Url:            http://q4wine.brezblock.org.ua/
Group:          System/Emulators/PC
Source0:        %{name}-%{version}.tar.bz2
BuildRequires:  cmake
BuildRequires:  fdupes
BuildRequires:  fuseiso
BuildRequires:  icoutils
BuildRequires:  libqt4-devel
BuildRequires:  update-desktop-files
Requires:       fuseiso
Requires:       icoutils
Requires:       libqt4-sql-sqlite
Requires:       sqlite3
Requires:       sudo
Requires:       wine
BuildRoot:      %{_tmppath}/%{name}-%{version}-build

%description
Q4Wine is an Qt4-based GUI for WINE. It will help you to manage wine
prefixes and installed applications.

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
%setup -qn %{name}-0.120-r1

%build
%__mkdir build
cd build
export CFLAGS="%{optflags}"
export CXXFLAGS="%{optflags}"
cmake .. -DCMAKE_INSTALL_PREFIX=%{_prefix} -DWITH_WINETRIKS=ON
%__make %{?_smp_mflags}

%install
pushd build
%make_install
popd
%fdupes -s %{buildroot}
%suse_update_desktop_file %{name}

%clean
%{?buildroot:%__rm -rf "%{buildroot}"}

%files
%defattr(-,root,root)
%doc AUTHORS ChangeLog LICENSE README TODO
%{_bindir}/*
%{_datadir}/%{name}
%{_libdir}/%{name}
%doc %{_mandir}/man?/*
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png

%changelog
