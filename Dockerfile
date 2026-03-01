FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    wget \
    curl \
    unzip \
    pkg-config \
    ca-certificates \
    autoconf \
    automake \
    libtool \
    libboost-dev \
    libboost-json-dev \
    libboost-program-options-dev \
    libopencv-dev \
    libprotobuf-dev \
    protobuf-compiler \
    libgrpc++-dev \
    protobuf-compiler-grpc \
    libgtest-dev \
    clang-format-18 \
    x11-apps \
    libgtk-3-0 \
    libcanberra-gtk3-module \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build
# Копируем исходный код проекта
COPY . .

# Сборка проекта (предполагается, что есть CMakeLists.txt)
RUN mkdir build && cd build && \
    cmake .. && \
    make

# ---- стадия запуска ----
FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

# Копируем только необходимые рантайм-библиотеки и собранные бинарники
RUN apt-get update && apt-get install -y --no-install-recommends \
    libboost-json1.83.0 \
    libopencv-core406t64 \
    libopencv-imgproc406t64 \
    libopencv-highgui406t64 \
    libopencv-imgcodecs406t64 \
    libopencv-calib3d406t64 \
    libopencv-contrib406t64 \
    libopencv-dnn406t64 \
    libopencv-features2d406t64 \
    libopencv-flann406t64 \
    libopencv-ml406t64 \
    libopencv-objdetect406t64 \
    libopencv-photo406t64 \
    libopencv-shape406t64 \
    libopencv-stitching406t64 \
    libopencv-superres406t64 \
    libopencv-video406t64 \
    libopencv-videoio406t64 \
    libopencv-videostab406t64 \
    libopencv-viz406t64 \
    libprotobuf32t64 \
    libgrpc++1.51t64 \
    libgrpc29t64 \
    libgcc-s1 \
    libstdc++6 \
    libboost-program-options1.83.0 \
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update && apt-get install -y --no-install-recommends \
    libboost-json1.83.0 \
    libboost-program-options1.83.0 \
    libopencv-dev \
    libprotobuf32t64 \
    libgrpc++1.51t64 \
    libgrpc29t64 \
    libgcc-s1 \
    libstdc++6 \
    libgtk-3-0 \
    libcanberra-gtk3-module \
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update && apt-get install -y --no-install-recommends \
    libqt5gui5 \
    libxcb-xinerama0 \
    libxcb-icccm4 \
    libxcb-image0 \
    libxcb-keysyms1 \
    libxcb-randr0 \
    libxcb-render-util0 \
    libxcb-shape0 \
    libxcb-sync1 \
    libxcb-xfixes0 \
    libxcb-xkb1 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
# Копируем собранные исполняемые файлы из стадии builder
COPY --from=builder /build/build/src/server/server /app/server
COPY --from=builder /build/build/src/client/client /app/client

# Команда по умолчанию (можно переопределить)
CMD ["/app/server"]