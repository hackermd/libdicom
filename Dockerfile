FROM debian:latest

RUN export DEBIAN_FRONTEND=noninteractive && \
    export DEBCONF_NONINTERACTIVE_SEEN=true && \
    apt-get update && \
    apt-get install -y --no-install-suggests --no-install-recommends \
    autogen \
    dh-autoreconf \
    build-essential \
    check \
    dumb-init \
    libtool \
    pkg-config \
    shtool \
    valgrind && \
    apt-get clean

COPY data ./data
COPY lib ./lib
COPY src ./src
COPY tests ./tests
COPY tools ./tools
COPY supp ./supp
COPY autogen.sh ./autogen.sh
COPY configure.ac ./configure.ac
COPY Makefile.am ./Makefile.am

ENV LD_PATH="/usr/local/bin:${LD_LIBRARY_PATH}"
ENV LD_LIBRARY_PATH="/usr/local/lib:${LD_LIBRARY_PATH}"
RUN CFLAGS="-O0 -DDEBUG" ./autogen.sh --prefix=/usr/local
RUN make && make install

ENTRYPOINT ["/usr/bin/dumb-init", "--"]
CMD make check; cat test-suite.log && \
    valgrind \
        --leak-check=full \
        dcm-dump ./data/test_files/sm_image.dcm > /dev/null
