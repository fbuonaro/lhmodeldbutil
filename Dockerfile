##################################################################################
# STAGE 0 - base environment with first set of runtime dependencies
##################################################################################
FROM centos:centos7 as lhmodeldbutil-s0-base-env
LABEL lhmodeldbutil-stage-base-env="yes"
LABEL lhmodeldbutil-stage-build-env="no"
LABEL lhmodeldbutil-stage-build="no"
LABEL lhmodeldbutil-stage-test-env="no"
LABEL lhmodeldbutil-stage-main="no"

# okay repo is for gtest/gmock 1.8, 1.6 is permanently broken
RUN yum -y --enablerepo=extras install epel-release && \
    yum -y install https://repo.ius.io/ius-release-el7.rpm && \
    yum -y install http://repo.okay.com.mx/centos/7/x86_64/release/okay-release-1-1.noarch.rpm && \
    yum clean all

##################################################################################
# STAGE 1 - build tools and libraries needed to build lhmodeldbutil
##################################################################################
FROM lhmodeldbutil-s0-base-env as lhmodeldbutil-s1-build-env
LABEL lhmodeldbutil-stage-base-env="no"
LABEL lhmodeldbutil-stage-build-env="yes"
LABEL lhmodeldbutil-stage-build="no"
LABEL lhmodeldbutil-stage-test-env="no"
LABEL lhmodeldbutil-stage-main="no"

# for compiling and unit testing
# x86_64 versions from okay repo for gmock/gtest 1.8
RUN yum -y install \
        cmake3 \
        gcc \
        gcc-c++ \
        gtest-devel.x86_64 \
        gmock-devel.x86_64 \
        rapidjson-devel \
        wget \
        bzip2 \
        git \
        make && \
    yum clean all

# cppdb db accessors
RUN mkdir /cppdb && \
    cd /cppdb && \
    wget https://sourceforge.net/projects/cppcms/files/cppdb/0.3.1/cppdb-0.3.1.tar.bz2 && \
    tar -xf cppdb-0.3.1.tar.bz2 && \
    cd cppdb-0.3.1 && \
    mkdir build && \
    cd build && \
    cmake3 \
        -DCMAKE_INSTALL_PREFIX=/usr \
        -DDISABLE_SQLITE=ON \
        -DDISABLE_PQ=ON \
        -DMYSQL_BACKEND_INTERNAL=ON \
        ../ && \
    make all && \
    make install

# lhmodel codegen
RUN git clone https://github.com/fbuonaro/lhmodel.git /lhmodel && \
    cd /lhmodel && \
    mkdir ./build && \
    cd ./build && \
    cmake3 \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr \
        ../ && \
    make && \
    make test && \
    make install-lhmodel && \
    make install-lhmodelutil

RUN ldconfig

ENTRYPOINT [ "bash" ]

##################################################################################
# STAGE 2 - the lhmodeldbutil source and compiled binaries
##################################################################################
FROM lhmodeldbutil-s1-build-env as lhmodeldbutil-s2-build
LABEL lhmodeldbutil-stage-base-env="no"
LABEL lhmodeldbutil-stage-build-env="no"
LABEL lhmodeldbutil-stage-build="yes"
LABEL lhmodeldbutil-stage-test-env="no"
LABEL lhmodeldbutil-stage-main="no"

ADD . /lhmodeldbutil
RUN cd /lhmodeldbutil && \
    mkdir ./build && \
    cd ./build && \
    cmake3 \
        -DCMAKE_BUILD_TYPE=Release \
        ../ && \
    make && \
    make test

##################################################################################
# STAGE 3 - the base image with additional built runtime dependencies, lhmodeldbutil 
#           binaries and test binaries needed for running integration tests
#           includes everything from build-env
##################################################################################
FROM lhmodeldbutil-s2-build as lhmodeldbutil-s3-test-env
LABEL lhmodeldbutil-stage-base-env="no"
LABEL lhmodeldbutil-stage-build-env="no"
LABEL lhmodeldbutil-stage-build="no"
LABEL lhmodeldbutil-stage-test-env="yes"
LABEL lhmodeldbutil-stage-main="no"

RUN cd /lhmodeldbutil/build && \
    make install-lhmodeldbutil

##################################################################################
# STAGE 4 - the base image with additional built runtime dependencies and 
#           lhmodeldbutil binaries includes nothing from build-env
##################################################################################
FROM lhmodeldbutil-s0-base-env as lhmodeldbutil-s4-main-env
LABEL lhmodeldbutil-stage-base-env="no"
LABEL lhmodeldbutil-stage-build-env="no"
LABEL lhmodeldbutil-stage-build="no"
LABEL lhmodeldbutil-stage-test-env="no"
LABEL lhmodeldbutil-stage-main="yes"

COPY --from=lhmodeldbutil-s2-build /usr/ /usr/
COPY --from=lhmodeldbutil-s2-build /lhmodeldbutil/ /lhmodeldbutil/
RUN cd /lhmodeldbutil/build && \
    make install-lhmodeldbutil && \
    cd / && \
    rm -rf /lhmodeldbutil
