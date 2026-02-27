#pragma once

#include <boost/program_options.hpp>
#include <string>
#include <unordered_map>

namespace po = boost::program_options;

class CommandOptions {
 public:
  CommandOptions();
  ~CommandOptions();

  void Parse(int argc, char* argv[]);

  std::string GetImageFile() const { return imageFile_; }
  std::string GetPolygonsFile() const { return polygonsFile_; }

  bool isHelpRequested() const { return helpRequested_; }

  void printHelp();

 private:
  std::string imageFile_;
  std::string polygonsFile_;

  po::options_description desc_;
  po::variables_map vm_;
  bool helpRequested_ = false;
};
