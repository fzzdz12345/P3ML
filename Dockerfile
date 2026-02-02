FROM ubuntu:18.04

RUN apt-get update \
    && apt-get install -y \
        git \
        make \
        g++ \
        libssl-dev \
    && rm -r /var/lib/apt/lists/*


