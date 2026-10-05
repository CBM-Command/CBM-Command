FROM debian:stable

RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install git curl nano vim make gcc unzip build-essential libxaw7-dev xfonts-utils
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install xa65 dos2unix byacc flex pkg-config zlib1g-dev file cc65
RUN curl -L https://sourceforge.net/projects/vice-emu/files/releases/vice-3.10.tar.gz/download -o vice-3.10.tar.gz && tar -zxvf vice-3.10.tar.gz
RUN cd vice-3.10 && ./configure     --prefix="$PWD/install"     --enable-headlessui     --disable-html-docs     --without-alsa     --without-pulse     --without-oss     --without-portaudio     --without-sdlsound     --disable-hardsid     --disable-cpuhistory     --disable-openmp     --disable-rs232     --disable-ipv6     --disable-realdevice     --without-png     --without-libcurl     --without-lame     --without-mpg123     --without-flac     --without-vorbis && make c1541 && cp src/c1541 /usr/bin

COPY CBM-Command  /CBM-Command 
RUN cd CBM-Command && mkdir c128 && mkdir c64 && mkdir vic20 && mkdir plus4 && ./build.sh

CMD (cd /CBM-Command && cp cbmcommand* /flash/)
