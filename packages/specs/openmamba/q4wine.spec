Name:          q4wine
Version:       0.120
Release:       2mamba
Summary:       Q4Wine is a Qt4 GUI for wine.
Summary(it):   Q4Wine è un interfaccia grafica per wine
Group:         Graphical Desktop/Applications/Utilities
Vendor:        openmamba
Distribution:  openmamba
Packager:      Ercole 'ercolinux' Carpanetto <ercole69@gmail.com>
URL:           http://q4wine.brezblock.org.ua/
#Source:	       http://q4wine.brezblock.org.ua/media/rc/q4wine-%{version}-rc1.tar.bz2
Source:        http://ignum.dl.sourceforge.net/project/q4wine/q4wine/q4wine%20%{version}/q4wine-%{version}.tar.bz2 
#Source1:       it_it.ts
#Source2:       q4wine.desktop
#Source3:       en_us.ts
License:       GPL
BuildRequires: cmake
BuildRoot:     %{_tmppath}/%{name}-%{version}-root
## AUTOBUILDREQ-BEGIN
BuildRequires: glibc-devel
BuildRequires: libfontconfig-devel
BuildRequires: libfreetype-devel
BuildRequires: libfuse-devel
BuildRequires: libgcc
BuildRequires: libglib-devel
BuildRequires: libICE-devel
BuildRequires: libopenssl-devel
BuildRequires: libpng-devel
BuildRequires: libqt4-devel
BuildRequires: libSM-devel
BuildRequires: libstdc++6-devel
BuildRequires: libX11-devel
BuildRequires: libXext-devel
BuildRequires: libXfixes-devel
BuildRequires: libXinerama-devel
BuildRequires: libXrender-devel
BuildRequires: libz-devel
BuildRequires: libzip-devel
## AUTOBUILDREQ-END
requires: wine
requires: icoutils
requires: fuseiso

%description
Q4Wine is a Qt4 GUI for Wine. It will help you manage Wine prefixes and installed applications.
    * Can export Qt color theme into Wine colors settings.
    * Can easy work with different Wine versions at same time;
    * Easy creating, deleting and managing prefixes (WINEPREFIX);
    * Easy controlling for Wine process;
    * Easy installer wizard for Wine applications; (Not yet. Wait for v. 0.120)
    * Autostart icons support;
    * Easy cd-image use;
    * You can extract icons from PE files (.exe .dll);
    * Easy backup and restore for managed prefixes.
    * Winetricks support.
    * And more... Explore it! ;) 

%description -l it 
Q4Wine è un'interfaccia grafica per wine. Permette di gestire i prefissi di wine e le applicazioni installate
    * Può esportare i temi di colore di QT nei settaggi colore di wine.
    * Permette di lavorare facilmente con versioni diverse di wine contemporaneamente;
    * Permette di creare, cancellare e gestire facilmente i prefissi (WINEPREFIX);
    * Controllo semplice dei processi di wine;
    * Wizard per l'installazione semplificata delle applicazioni in wine; (Non ancora. Attendete la  v. 0.120)
    * Supporto per le icone Autostart;
    * Uso semplificato delle immagini dei cd;
    * Permette di estrarre le icone dai file PE (.exe .dll);
    * Backup and restore facilitato per i prefessi gestiti.
    * Supporto a winetricks.
    * e molto altro... Esploratelo! ;)

%prep
%setup -q -n %{name}-%{version}
#cp %{SOURCE1} src/i18n/
#cp %{SOURCE3} src/i18n/
#cp %{SOURCE2} ./
#%patch -p1 
%build
cmake -d build -DWITH_APPSDB=ON -DWITH_EMBEDDED_FUSEISO=ON -DCMAKE_INSTALL_PREFIX=/usr .
%make

%install
[ "%{buildroot}" != / ] && rm -rf "%{buildroot}"
%makeinstall 

%clean
[ "%{buildroot}" != / ] && rm -rf "%{buildroot}"

%files
%defattr(-,root,root)
%{_datadir}/applications/q4wine.desktop
%{_datadir}/pixmaps/q4wine.png
%{_bindir}/q4wine
%{_bindir}/q4wine-cli
%{_bindir}/q4wine-mount
%{_bindir}/q4wine-helper
%{_libdir}/q4wine/libq4wine-core.so
%{_datadir}/q4wine/i18n/*.qm
%{_datadir}/q4wine/icons/*.png
%{_datadir}/q4wine/theme/nuvola/data/*.png
%{_mandir}/man1/*.1.gz
%doc ChangeLog COPYING README

%changelog
* Mon Oct 04 2010 Ercole 'ercolinux' Carpanetto <ercole69@gmail.com> 0.120-2mamba
- ... add a changelog entry

* Mon Mar 22 2010 Ercole 'ercolinux' Carpanetto <ercole69@gmail.com> 0.117-r6-1mamba
- update to 0117-r6

* Wed Nov 18 2009 Ercole 'ercolinux' Carpanetto <ercole69@gmail.com> 0.114-r1-2mamba
- added requires for fuseiso and icoutils 

* Tue Nov 17 2009 Ercole 'ercolinux' Carpanetto <ercole69@gmail.com> 0.114-r1-1mamba
- update to 0.114-r1

* Sun Nov 15 2009 Ercole 'ercolinux' Carpanetto <ercole69@gmail.com> 0.114-2mamba
- update to 0.114

* Wed Nov 11 2009 Ercole 'ercolinux' Carpanetto <ercole69@gmail.com> 0.114-1mamba
- update to 0.114-rc1
- remove it_it.ts source because it has been integrated in main trunk

* Fri Nov 06 2009 Ercole 'ercolinux' Carpanetto <ercole69@gmail.com> 0.113-4mamba
- rebuild with improved italian and english translation
- new desktop file with italian translation
- added requires for wine
- added a patch to fix a problem with winetricks install

* Thu Nov 05 2009 Ercole 'ercolinux' Carpanetto <ercole69@gmail.com> 0.113-3mamba
- fixed a typo in cmake

* Thu Nov 05 2009 Ercole 'ercolinux' Carpanetto <ercole69@gmail.com> 0.113-2mamba
- rebuild with italian translation
- Added support to winetricks
 
* Wed Nov 04 2009 Ercole 'ercolinux' Carpanetto <ercole69@gmail.com> 0.113-1mamba
- package created by autospec
