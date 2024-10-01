#include <cstddef>
#include <fstream>
#include <unordered_map>

#include "parser.hpp"

namespace automata::game_of_life {

namespace templates {

////////////////////////////////////////////////////////////////////////////////////

// TODO переделать через enum? ДА

std::string base_path{"../resources/template_data_storage/"};

std::unordered_map<std::string, std::string> template_name_to_path_txt{
    {"Glider", base_path + "glider.txt"},
    {"Point", base_path + "point.txt"},
    {"Glider Shuttle", base_path + "glider_shuttle.txt"},
    {"Pulsar", base_path + "pulsar.txt"},
    {"Pulsars", base_path + "pulsars.txt"},
    {"Quasar", base_path + "quasar.txt"},
    {"Quasar2", base_path + "quasar2.txt"},
    {"Quasar3", base_path + "quasar3.txt"},
    {"Snark", base_path + "snark.txt"},
    {"Scholar", base_path + "scholar.txt"},
    {"Magic Snark", base_path + "snarkcatalystvariants.txt"},
    {"Sir Robin", base_path + "sir_robin.txt"},
    {"Quasar", base_path + "quasar.txt"},
    {"Quasar2", base_path + "quasar2.txt"},
    {"Quasar3", base_path + "quasar3.txt"},
    {"Bhassler", base_path + "p21bhassler.txt"},
    {"Block puffer", base_path + "p28blockpuffer.txt"},
    {"Spider", base_path + "spider.txt"},
    {"Backrake 2", base_path + "backrake2.txt"},
    {"Pufferfish rake", base_path + "pufferfish_rake.txt"},
    {"Blinker puffer 2", base_path + "blinker_puffer2.txt"},
    {"Slow puffer 1", base_path + "slow_puffer1.txt"},
    {"Hivenudger 2", base_path + "hivenudger2.txt"},
    {"Piorbital3", base_path + "piorbital3.txt"},
    {"Puffer blinker", base_path + "p112pufferblinker.txt"},
    {"Frothing puffer", base_path + "frothing_puffer.txt"},
    {"Gunstar 2", base_path + "gunstar2.txt"},
    {"Glider shuttle", base_path + "glider_shuttle.txt"},
    {"Nurik", base_path + "nurik.txt"},
    {"Cordership gun", base_path + "cordership_gun.txt"},
    {"Cordership rake", base_path + "cordership_rake.txt"},
    {"Switch engine", base_path + "switch_engine.txt"},
};

std::unordered_map<std::string, std::string> template_name_to_path_rle{
    {"Turinge Machine", base_path + "turinge_machine.rle"},
    {"Pufferfish breeder", base_path + "pufferfish_breeder.rle"},
    {"Frothing puffer rake", base_path + "frothingpufferrake.rle"},
};

////////////////////////////////////////////////////////////////////////////////////

Parser::Parser(GameConfig* config)
    : config_(config) {
}

void Parser::Parse(std::string template_name, Cell where) {
  if (template_name_to_path_txt.find(template_name) !=
      template_name_to_path_txt.end()) {
    return ParseTxt(template_name_to_path_txt[template_name], where);
  }

  ParseRle(template_name_to_path_rle[template_name], where);
}

////////////////////////////////////////////////////////////////////////////////////

void Parser::ParseTxt(std::string file_path, Cell where) {
  living_cells_.clear();

  std::fstream file(std::move(file_path));

  std::string line;
  std::size_t row = 0;

  for (; std::getline(file, line); ++row) {
    columns_count = std::max(columns_count, line.size());
    std::size_t column = 0;

    for (const auto& item : line) {
      if (item != '.') {
        if (config_->is_endless_board) {
          living_cells_.emplace(row + where.x_coordinate,
                                column + where.y_coordinate);
        } else {
          living_cells_.emplace(
              (row + where.x_coordinate) % config_->rows_count,
              (column + where.y_coordinate) % config_->columns_count);
        }
      }

      ++column;
    }
  }

  rows_count = row;
  file.close();
}

////////////////////////////////////////////////////////////////////////////////////

void Parser::ParseRle(std::string filename, Cell where) {
  std::ifstream file(filename);

  file.is_open();
  std::string input((std::istreambuf_iterator<char>(file)),
                    (std::istreambuf_iterator<char>()));
  file.close();

  living_cells_.clear();

  bool ignore = false;
  int step = 1;
  std::size_t x = where.x_coordinate;
  std::size_t y = where.y_coordinate;

  for (size_t i = 0; i < input.length(); ++i) {
    if (ignore) {
      if (input[i] == '\n') {
        ignore = false;
      }
      continue;
    }
    switch (input[i]) {
      case '#':
      case 'x':
      case '!':
        ignore = true;
        continue;
      case '$':
        if (x > columns_count)
          columns_count = x;
        x = 0;
        y += step;
        continue;
      case 'b':
        x += step;
        step = 1;
        continue;
      case 'o':
        for (int j = 0; j < step; ++j) {
          living_cells_.emplace(x++, y);
        }
        step = 1;
        continue;
    }
    size_t pos = input.find_first_not_of("0123456789", i);
    if (pos != std::string::npos) {
      std::string numberStr = input.substr(i, pos - i);
      int number = std::stoi(numberStr);
      step = number;
      i += numberStr.length() - 1;
    }
  }

  rows_count = y;
}

////////////////////////////////////////////////////////////////////////////////////

}  // namespace templates

}  // namespace automata::game_of_life
