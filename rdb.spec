Name: rdb
Version: 1
Release: alt1

Summary: rdb api client
Summary(ru_RU.UTF-8): rdb api клиент
License: apache-2.0
Group: Development/Other

Url: https://github.com/appr0ve/test
VCS: https://github.com/appr0ve/test.git
Source: %name-%version.tar

Requires: glib2 libgio libjson-glib

BuildRequires(pre): git
BuildRequires: gcc meson glib2-devel libjson-glib-devel libgio-devel

%description
none

%prep
%setup

%build
%meson
%meson_build

%install
%meson_install

%check
%meson_test

%files
%{_libdir}/lib%{name}.so.*
%doc README

%files devel
%{_libdir}/lib%{name}.so
%{_includedir}/%{name}.h

%changelog
* Wed Oct 23 2024 appr0ve <185701845+appr0ve@users.noreply.github.com> 1-alt1
- Initial build for ALT Linux Sisyphus.
