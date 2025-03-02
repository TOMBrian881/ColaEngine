#ifndef COLA_START_H
#define COLA_START_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <memory>
#include <functional>
#include <chrono>
#include <numeric>
#include <string.h>
#include <algorithm>
#include <limits>
#include <cstdint>

#define COLA_ENGINE_GRAPHIC_API_VULKAN

#endif

#ifdef Cola_DEFINE_RES_ROOT_DIR
#define COLA_RES_ROOT_DIR Cola_DEFINE_RES_ROOT_DIR
#else
#define COLA_RES_ROOT_DIR "Resource/"
#endif

#define COLA_RES_SHADER_DIR COLA_RES_ROOT_DIR"Shader/"
#define COLA_RES_SPIR_V_DIR COLA_RES_ROOT_DIR"Spir-v/"
