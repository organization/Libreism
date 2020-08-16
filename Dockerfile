FROM alpine:3.12 AS builder

RUN apk add --no-cache build-base cmake boost-dev jsoncpp-dev openssl-dev util-linux-dev zlib-dev

WORKDIR /usr/src/libreism
COPY . .

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=RELEASE \
    && cmake --build build --parallel $(nproc)

FROM alpine:3.12

RUN apk add --no-cache tzdata jsoncpp libuuid
ENV TZ Asia/Seoul

WORKDIR /usr/bin/libreism
COPY --from=builder /usr/src/libreism/build/src/libreism .

EXPOSE 8080
CMD ["./libreism"]
