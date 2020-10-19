#pragma once

#include <tuple>
#include <optional>
#include <fstream>

namespace engine::utils {

    template<typename ...values_t>
    std::optional<std::tuple<values_t...> >read_bytes_from_stream(std::ifstream& file) {
        // create a tuple of the values to read
        std::tuple<values_t...> retval;

        // for each element of the tuple
        std::apply([&file](auto&& ...elems){
            // read it
            (file.read(reinterpret_cast<char*>(&elems), sizeof(elems)), ...);
        }, retval);
        
        // did the stream fail to read?
        if (!file) {
            // corrupt map file
            return {};
        }

        // return the initialized values
        return retval;
    }
}
