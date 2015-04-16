Summary: A simple countdown and timer application.

%define version 1.2.1

License: GPL-2.0+
Group: Applications/Education
Name: countdown
Prefix: /usr
Release: 1
Source: countdown-%{version}.tar.gz
URL: https://github.com/hmatuschek/countdown
Version: %{version}
Buildroot: /tmp/countdownrpm
BuildRequires: gcc-c++, cmake
%if 0%{?suse_version}
BuildRequires: libqt5-qtbase-devel, linqt5-qtmultimedia-devel, update-desktop-files
Requires: libqt5-qtbase, libqt5-multimedia 
%endif
%if 0%{?fedora}
BuildRequires: qt5-qtbase-devel, qt5-qtmultimedia-devel
Requires: qt5-qtbase, qt5-multimedia
%endif

%description
A simple countdown and timer application.


%prep
%setup -q


%build
cmake -DCMAKE_BUILD_TYPE=RELEASE \
      -DCMAKE_INSTALL_PREFIX=$RPM_BUILD_ROOT/usr
make


%install
make install
%if 0%{?suse_version}
%suse_update_desktop_file countdown Education Teaching
%endif

%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-, root, root, -)
%attr(755, root, root) %{_prefix}/bin/countdown
%{_prefix}/share/applications/countdown.desktop
%{_prefix}/share/icons/countdown.png
