FROM ubuntu:18.04

# Install environment components
RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y git autoconf automake libtool curl make g++ unzip cmake

# Build protobuf
RUN git clone https://github.com/protocolbuffers/protobuf.git
RUN cd protobuf
RUN git submodule update --init --recursive
RUN ./configure
RUN make
RUN make check
RUN make install
RUN ldconfig

CMD ["/bin/bash"]
