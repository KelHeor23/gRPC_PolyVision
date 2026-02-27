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

  bool GetShowPolygons() const { return showPolygons_; }

  bool isHelpRequested() const { return helpRequested_; }

  void printHelp();

 private:
  bool showPolygons_ = false;

  po::options_description desc_;
  po::variables_map vm_;
  bool helpRequested_ = false;
};