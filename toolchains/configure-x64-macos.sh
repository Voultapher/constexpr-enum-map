#!/usr/bin/env bash

set -euo pipefail

SCRIPT_FILE=$(realpath ${0})
SCRIPT_PATH=$(dirname ${SCRIPT_FILE})

mkdir -p "${SCRIPT_PATH}/debug"
cd "${SCRIPT_PATH}/debug"
cmake -G Ninja \
  -D CMAKE_C_COMPILER=clang \
  -D CMAKE_CXX_COMPILER=clang++ \
  -D CMAKE_CXX_FLAGS="-Wall -Wextra -stdlib=libc++ -fno-rtti -fsanitize=address,undefined -fno-omit-frame-pointer -fcolor-diagnostics" \
  -D CMAKE_BUILD_TYPE=Debug \
  -D CMAKE_EXPORT_COMPILE_COMMANDS=1 ..

mkdir -p "${SCRIPT_PATH}/release"
cd "${SCRIPT_PATH}/release"
cmake -G Ninja \
  -D CMAKE_C_COMPILER=clang \
  -D CMAKE_CXX_COMPILER=clang++ \
  -D CMAKE_CXX_FLAGS="-Wall -Wextra -stdlib=libc++ -fno-rtti -flto=thin -fcolor-diagnostics" \
  -D CMAKE_BUILD_TYPE=Release \
  -D CMAKE_EXPORT_COMPILE_COMMANDS=1 ..

# Production hardening
# -fsanitize=scudo | GWP_ASAN_OPTIONS=SampleRate=500 ./binary
