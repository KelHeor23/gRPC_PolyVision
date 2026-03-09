#include "impl/cmd_options.h"

#include <format>
#include <iostream>

CommandOptions::CommandOptions() : desc_("Allowed options") {
  desc_.add_options()("help,h", "Show help")(
      "image,i", po::value<std::string>(&image_file_),
      "Path to the image file")("polygons,p",
                                po::value<std::string>(&polygons_file_),
                                "Path to the polygons file");
}

CommandOptions::~CommandOptions() = default;

void CommandOptions::Parse(int argc, char *argv[]) {
  try {
    po::store(po::parse_command_line(argc, argv, desc_), vm_);

    if (vm_.count("help")) {
      help_requested_ = true;
      return;
    }
    po::notify(vm_);
  } catch (const po::error &e) {
    throw std::runtime_error{
        std::format("Error parsing command line: {}\n", e.what())};
  } catch (const std::runtime_error &e) {
    throw;
  } catch (...) {
    throw std::runtime_error{std::format("Error parsing command line: {}\n",
                                         "Something unexpected happened")};
  }
}

void CommandOptions::PrintHelp() const { std::cout << desc_ << "\n"; }