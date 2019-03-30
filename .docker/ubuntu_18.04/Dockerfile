FROM ubuntu:18.04

# Install environment components
RUN apt-get update && apt-get upgrade -y \
	&& apt-get install -y git autoconf automake libtool curl make g++ unzip cmake

CMD ["/bin/bash"]
