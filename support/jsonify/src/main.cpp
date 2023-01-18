#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main() {
  std::ifstream file{"resources/INIT/Graficos.ini"};
  std::string line;
  nlohmann::ordered_json json;
  while (std::getline(file, line)) {
    line.erase(line.size() - 1);
    auto begin = line.find_first_of('h') + 1;
    auto end = line.find_first_of('=');
    auto number = line.substr(begin, end - begin);
    line = line.substr(end + 1);

    end = line.find_first_of('-');
    auto amount = line.substr(0, end);
    line = line.substr(end + 1);
    nlohmann::ordered_json grh;

    if (amount == "1") {
      end = line.find_first_of('-');
      auto graph = line.substr(0, end);
      line = line.substr(end + 1);

      end = line.find_first_of('-');
      auto left = line.substr(0, end);
      line = line.substr(end + 1);

      end = line.find_first_of('-');
      auto top = line.substr(0, end);
      line = line.substr(end + 1);

      end = line.find_first_of('-');
      auto width = line.substr(0, end);
      line = line.substr(end + 1);

      auto height = line;

      grh["type"] = "static";
      grh["graph"] = std::stoi(graph);
      grh["left"] = std::stoi(left);
      grh["top"] = std::stoi(top);
      grh["width"] = std::stoi(width);
      grh["height"] = std::stoi(height);
    } else {
      grh["type"] = "animation";
      std::vector<int> sequence;
      sequence.reserve(std::stoi(amount));
      for (auto i = 0; i < std::stoi(amount); ++i) {
        end = line.find_first_of('-');
        auto number = line.substr(0, end);
        sequence.push_back(std::stoi(number));
        line = line.substr(end + 1);
      }
      grh["sequence"] = sequence;
      grh["speed"] = std::stoi(line);
    }

    json[number] = grh;
  }

  std::ofstream output{"output.json"};
  output << std::setw(4) << json << std::endl;

  return 0;
}