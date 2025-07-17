#ifndef CXXSUPPORT_H
#define CXXSUPPORT_H

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <string>



namespace csp
{
  namespace file
  {
    inline std::string Open(const std::filesystem::path& filepath)
    {
      std::ifstream ifs(filepath, std::ios::binary | std::ios::ate);
      if (!ifs) {
        throw std::runtime_error("Failed to open file: " + filepath.string());
      }

      const auto fileSize = ifs.tellg();
      if (fileSize == -1) {
        throw std::runtime_error(
          "Failed to determine file size: " + filepath.string()
        );
      }

      std::string content;
      content.resize(static_cast<std::size_t>(fileSize));
      ifs.seekg(0);
      ifs.read(content.data(), content.size());

      if (!ifs) {
        throw std::runtime_error("Error reading file: " + filepath.string());
      }

      return content;
    }


    inline void Write(
      const std::filesystem::path& filepath,
      const std::string content,
      const bool append = false
    )
    {
      std::ios_base::openmode mode = std::ios::binary;
      if (append) {
        mode |= std::ios::app;
      } else {
        mode |= std::ios::out;
      }
      std::ofstream ofs(filepath, mode);
      if (!ofs) {
        throw std::runtime_error("Failed to open file: " + filepath.string());
      }

      ofs.write(content.data(), content.size());
      if (!ofs) {
        throw std::runtime_error("Error writing file: " + filepath.string());
      }
    }
  }


  namespace utils
  {
    inline std::filesystem::path PathNormalize(
      const std::filesystem::path& raw_path
    )
    {
      if (raw_path.filename() == std::filesystem::path("")) {
        return raw_path.parent_path();
      } else {
        return raw_path;
      }
    }


    inline std::string ToString(const double value, const int precision)
    {
      std::ostringstream out;
      out << std::fixed << std::setprecision(precision) << value;
      return out.str();
    }
  }
}



#endif // CXXSUPPORT_H
