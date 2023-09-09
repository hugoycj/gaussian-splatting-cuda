FROM nvidia/cuda:12.2.0-devel-ubuntu20.04

# Set working directory
WORKDIR /workdir

# Install dependencies
# we could pin them to specific versions to be extra sure
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    git \
    python3-dev \
    libtbb-dev \
    libeigen3-dev \
    unzip \
    g++ \
    libssl-dev \
    build-essential \
    checkinstall \
    wget \
    colmap \
    libfreeimage-dev \
    libflann-dev \
 && rm -rf /var/lib/apt/lists/*

# Install cmake 3.22
RUN wget https://github.com/Kitware/CMake/releases/download/v3.22.0/cmake-3.22.0.tar.gz \
 && tar -zvxf cmake-3.22.0.tar.gz \
 && cd cmake-3.22.0 \
 && ./bootstrap \
 && make -j8 \
 && checkinstall --pkgname=cmake --pkgversion="3.20-custom" --default
