#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <iostream>
#include <string>

class Logger {
public:
  explicit Logger(const std::string &filename) : filename(filename) {}

  void log(const std::string &message) {
    std::ofstream file;
    file.open(filename, std::ofstream::out | std::ofstream::trunc);
    if (file.is_open()) {
      file << message << std::endl;
      file.close();
    } else {
      std::cerr << "Error opening file: " << filename << std::endl;
    }
  }

private:
  std::string filename;
};

#endif // LOGGER_HPP
