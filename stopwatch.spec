%global gituser       ridernator
%global gitrepo       stopwatch
%global gittag        1.1.1
%global gitlicense    MIT

%global release       0
%global packager      Ciaron Rider <ciaron.rider+github@gmail.com>
%global packagerEmail ciaron.rider+github@gmail.com

name:     %{gitrepo}
version:  %{gittag}
release:  %{release}
summary:  Command line stopwatch written in C with split / lap support
license:  %{gitlicense}
packager: %{packager} <%{packagerEmail}>
url:      https://github.com/%{gituser}/%{gitrepo}
source:   https://github.com/%{gituser}/%{gitrepo}/archive/%{gittag}.tar.gz#/%{gitrepo}-%{gittag}.tar.gz

%description
Command line stopwatch written in C with split / lap support

%prep
%setup -q

%install
make
install --directory $RPM_BUILD_ROOT/usr/local/bin
install -m 555 stopwatch $RPM_BUILD_ROOT/usr/local/bin

%files
/usr/bin/stopwatch
