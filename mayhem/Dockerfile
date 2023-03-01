# Build Stage
FROM --platform=linux/amd64 ubuntu:20.04 as builder

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y cmake clang

ADD . /cAT
WORKDIR /cAT

RUN mkdir build
WORKDIR build

RUN CC=clang CXX=clang++ cmake -DBUILD_FUZZ_TARGETS=ON ..
RUN make -j$(nproc) fuzz_parse

# Package Stage
FROM --platform=linux/amd64 ubuntu:20.04

COPY --from=builder /cAT/build/bin/fuzz_parse /fuzz_binaries/