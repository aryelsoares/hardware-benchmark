# ----- build stage -----
FROM rust:1.93.1 AS builder

WORKDIR /app

# Cache dependencies
COPY Cargo.toml Cargo.lock ./
RUN mkdir src && echo "fn main() {}" > src/main.rs
RUN cargo build --release
RUN rm -rf src

# Real code
COPY . .
RUN cargo build --release

# ----- runtime stage -----
FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y \
    ca-certificates \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/target/release/hardware-benchmark-app /usr/local/bin/app

CMD ["app"]
