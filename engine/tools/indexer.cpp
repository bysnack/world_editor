#include <fstream>
#include <istream>
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <sstream>
#include <tuple>
#include <filesystem>

template<typename element_t> auto begin(element_t& elem) { return reinterpret_cast<uint8_t*>(&elem); }
template<typename element_t> auto end(element_t& elem) { return reinterpret_cast<uint8_t*>(&elem) + sizeof(element_t); }

template<typename element_t>
void push_bytes(std::vector<uint8_t>& res, element_t element) {
    std::copy(begin(element), end(element), std::back_inserter(res));
}

std::vector<uint8_t> to_binary(const std::filesystem::path& path) {
    std::ifstream file{ path };
    std::vector<uint8_t> res;

    std::string line;
    while(std::getline(file, line)) {
        // remove "Grh"
        line.erase(0, 3);

        // substract grh number
        auto pos = line.find("=");
        uint32_t grh = std::stoi(line.substr(0, pos));
        line.erase(0, pos + 1);

        // substract type
        pos = line.find("-");
        auto type = std::stoi(line.substr(0, pos));
        std::cout << grh << '-' << type << '\n';
        line.erase(0, pos + 1);

        // ignore compositions for now
        if (type != 1) {
            continue;
        }
        
        // copy it to byte array
        push_bytes(res, grh);

        // compose a string stream with the texture data
        std::istringstream texture{ line };
        uint16_t index = 0;
        while(std::getline(texture, line, '-')) {
            // convert to integer
            auto data = std::stoi(line);
            // first element is uint32_t
            if (index == 0) {
                push_bytes(res, static_cast<uint32_t>(data));
            } else {
                push_bytes(res, static_cast<uint16_t>(data));
            }
            ++index; 
        }    
    }

    return res;
}

int main(int argc, char** argv) {
    std::filesystem::path path{ argv[argc - 1] };
    auto res = to_binary(path);

    std::ofstream output{ path.replace_extension(".ind"), std::ios::binary };
    std::copy( 
        res.begin(), 
        res.end(),
        std::ostreambuf_iterator<char>(output)
    );
}