FROM ubuntu:24.04

# Отключаем интерактивные запросы при установке пакетов
ENV DEBIAN_FRONTEND=noninteractive

# Устанавливаем все необходимые пакеты
RUN --mount=type=cache,target=/var/cache/apt \
    apt-get update && apt-get install -y --no-install-recommends \
    # Базовые инструменты сборки
    build-essential \
    cmake \
    git \
    wget \
    curl \
    unzip \
    pkg-config \
    ca-certificates \
    # Инструменты разработки (autotools для возможных подпроектов)
    autoconf \
    automake \
    libtool \
    # Boost для JSON
    libboost-dev \
    libboost-json-dev \
    # OpenCV для проекта
    libopencv-dev \
    # Protobuf и gRPC из официальных репозиториев Ubuntu 24.04
    libprotobuf-dev \
    protobuf-compiler \
    libgrpc++-dev \
    protobuf-compiler-grpc \
    # Библиотека GTest из репозитория (нужна для заголовков)
    libgtest-dev \
    # Инструмент форматирования (версия 18 доступна в репозитории)
    clang-format-18 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

CMD ["/bin/bash"]