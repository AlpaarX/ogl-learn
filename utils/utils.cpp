#include <fstream>
#include <iostream>
#include <string>
#include "utils.hpp"
#include "../include/tinygltf/tiny_gltf_v3.h"

namespace utils {
    std::string LoadShaderAsString(const std::string& filename)
    {
        std::string result = "";

        std::string line = "";
        std::ifstream myFile(filename.c_str());

        if (myFile.is_open())
        {
            while (std::getline(myFile, line))
            {
                result += line + '\n';
            }
            myFile.close();
        }
        
        if (result.size() >= 3 &&
        (unsigned char)result[0] == 0xEF &&
        (unsigned char)result[1] == 0xBB &&
        (unsigned char)result[2] == 0xBF)
        {
        result.erase(0, 3);
        }

        return result;
    }

    

    void LoadGltf(const char *path) {
        tinygltf3::Model model;
        tinygltf3::ErrorStack errors;
        tg3_parse_options opts;
        tg3_parse_options_init(&opts);
        opts.validate_indices = 1;
        opts.images_as_is = 1;

        tg3_error_code err = tinygltf3::parse_file(model, errors, path, &opts);
        if (err != TG3_OK || errors.has_error()) {
            std::cerr << "glTF load failed\n";
            for (uint32_t i = 0; i < errors.count(); ++i) {
                const auto *entry = errors.entry(i);
                std::cerr << entry->message << "\n";
            }
            return;
        }

        // Use model.get() to access the raw tg3_model pointer
        const tg3_model *m = model.get();
    }
}


