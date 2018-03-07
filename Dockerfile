FROM debian:stable

RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install git curl nano vim make gcc unzip build-essential libxaw7-dev xfonts-utils
RUN git clone https://github.com/cc65/cc65.git && cd cc65 && make && PREFIX=/root/.local make install
RUN curl -L https://sourceforge.net/projects/vice-emu/files/releases/vice-2.3.tar.gz/download -o vice-2.3.tar.gz && tar -zxvf vice-2.3.tar.gz
RUN cd vice-2.3 && ./configure --without-resid && make && make install

COPY CBM-Command  /CBM-Command 
RUN cd CBM-Command && mkdir c128 && mkdir c64 && mkdir vic20 && mkdir plus4 && ./build.sh

CMD (cd /CBM-Command && cp cbmcommand* /flash/)
