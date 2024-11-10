Name: rdb
Version: 1
Release: alt1

Summary: rdb api client
Summary(ru_RU.UTF-8): rdb api клиент
License: Apache-2.0
Group: Development/Other

Url: https://github.com/appr0ve/test
VCS: https://github.com/appr0ve/test.git
Source: %name-%version.tar
Packager: Joe Hacker <joe@altlinux.org>

Requires: glib2 libgio libjson-glib

BuildRequires(pre): rpm-macros-meson
BuildRequires: meson
BuildRequires: glib2-devel
BuildRequires: libjson-glib-devel
BuildRequires: libgio-devel

%description
none

%prep
%setup -n %name-%version

%build
%if %{with_build_gnu}
%autoreconf
%configure
%make_build
%else
%meson
%meson_build
%endif

%install
%if %{with_build_gnu}
%makeinstall
%else
%meson_install
%endif

%check
%if %{with_build_gnu}
%else
%meson_test
%endif

%files
%_bindir/%name
%_libdir/lib%{name}.so
%_datadir/locale/*/LC_MESSAGES/%name.mo
%doc README.org

%changelog
* Thu Oct 24 2024 Joe Hacker <joe@altlinux.org> 1-alt1
- Initial build for ALT Linux Sisyphus.
