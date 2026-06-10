#ifndef UTILS_HPP
#define UTILS_HPP
#include <string>
#include <glad/glad.h>

namespace utils {
    std::string LoadShaderAsString(const std::string& filename);
    void LoadGltf(const char *path);
}

#endif


