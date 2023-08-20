%global __requires_exclude ^libcppdb\\.so.*$

Name:       libcppdb-lhdist
Version:    1
Release:    1
Summary:    libcppdb packaged in an RPM
License:    See http://cppcms.com/sql/cppdb/
BuildRequires:   mariadb-libs mariadb-devel
Requires:   mariadb-libs mariadb-devel

%description
libcppdb packaged in an RPM

%prep
wget https://sourceforge.net/projects/cppcms/files/cppdb/0.3.1/cppdb-0.3.1.tar.bz2
tar -xf cppdb-0.3.1.tar.bz2

%build
cd cppdb-0.3.1
mkdir build
cd build
cmake3 -DCMAKE_INSTALL_PREFIX=/usr -DDISABLE_SQLITE=ON -DDISABLE_PQ=ON -DMYSQL_LIB=/usr/lib64/mysql/libmysqlclient.so.18 -DMYSQL_PATH=/usr/include/mysql -DMYSQL_BACKEND_INTERNAL=ON ../
make all

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}/usr
cd cppdb-0.3.1/build
make install DESTDIR=%{buildroot}
cd %{buildroot}
find ./ -type f -or -type l | grep usr | cut -d '.' -f 2- > %{buildroot}/../libcppdb_install_files.txt
find ./ -type f -or -type l | grep usr | xargs -I{} chmod 777 {}

%files -f %{buildroot}/../libcppdb_install_files.txt

%changelog
