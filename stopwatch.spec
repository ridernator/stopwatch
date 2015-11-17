%global gituser    ridernator
%global gitrepo    stopwatch
%global gittag     1.1.0
%global gitlicense MIT

Name:           %{gitrepo}
Version:        %{gittag}
Release:        0
Summary:        Command line stopwatch written in C with split / lap support
License:        %{gitlicense}
Source:         https://github.com/%{gituser}/%{gitrepo}/archive/%{gittag}.tar.gz#/%{gitrepo}-%{gittag}.tar.gz

%description
Command line stopwatch written in C with split / lap support

%prep
%setup -q

%install
make
install --directory $RPM_BUILD_ROOT/usr/bin
install -m 555 stopwatch $RPM_BUILD_ROOT/usr/bin

%files
/usr/bin/stopwatch