# syntax=docker/dockerfile:1
FROM debian:bookworm-slim AS builder

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build -j"$(nproc)"

FROM debian:bookworm-slim AS runtime

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        libstdc++6 \
        libgcc-s1 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=builder /app/build/app /usr/local/bin/app

ENTRYPOINT ["/usr/local/bin/app"]
