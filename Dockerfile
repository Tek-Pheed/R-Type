FROM ubuntu:latest

SHELL ["/bin/bash", "-c"]

WORKDIR /r-type/

COPY . /r-type

RUN apt update -y && apt upgrade -y

RUN apt install git gcc g++ make cmake -y

RUN apt install libxrandr-dev libxcursor-dev libudev-dev libopenal-dev libflac-dev libvorbis-dev libgl1-mesa-dev libegl1-mesa-dev libdrm-dev libgbm-dev libcriterion-dev libfreetype-dev libfreetype6 libfreetype6-dev -y