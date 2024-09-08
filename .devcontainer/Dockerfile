# Use the official Alpine image as a base
FROM alpine:latest

# Install dependencies
RUN apk update && apk add --no-cache \
    wget \
    gnupg \
    build-base \
    git \
    cmake \
    ninja \
    clang \
    musl-dev \
    gdb

# Create a non-root user and switch to it
RUN adduser -D -s /bin/sh vscode
USER vscode

# Set the working directory
WORKDIR /workspace