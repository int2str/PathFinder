#ifndef READ_FILE_HH
#define READ_FILE_HH

#include <filesystem>
#include <fstream>
#include <string>

namespace Utils {

//
// readFile() reads a single file specified by |path| line by line and returns a
// single string with its content. Nelines are stripped.
//
// Returns an empty string if the file could not be read.
//
[[nodiscard]] auto readFile(const std::filesystem::path& path) -> std::string {
  auto file    = std::ifstream(path);
  auto content = std::string{};
  auto line    = std::string{};
  while (std::getline(file, line)) content.append(line);
  return content;
}

}  // namespace Utils

#endif  // READ_FILE_HH
