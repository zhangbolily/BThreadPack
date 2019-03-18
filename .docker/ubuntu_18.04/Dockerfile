FROM ubuntu:18.04

# Install environment components
RUN apt-get update && apt-get upgrade -y \
	&& apt-get install -y git autoconf automake libtool curl make g++ unzip cmake

# Build protobuf
ADD protobuf.tar.gz /home/
RUN cd /home/protobuf \
	&& ls -al \
	&& ./autogen.sh \
	&& ./configure \
	&& make -j 4 \
	&& make install \
	&& ldconfig

CMD ["/bin/bash"]
