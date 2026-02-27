#include "impl/CmdOptions.h"

#include <format>
#include <iostream>

CommandOptions::CommandOptions() : desc_("Allowed options") {
  desc_.add_options()("help,h", "Show help")(
      "show_polygons,p", po::bool_switch(&showPolygons_), "show polygons");
}

CommandOptions::~CommandOptions() = default;

void CommandOptions::Parse(int argc, char *argv[]) {
  try {
    po::store(po::parse_command_line(argc, argv, desc_), vm_);

    if (vm_.count("help")) {
      helpRequested_ = true;
      return;
    }
    po::notify(vm_);
  } catch (const po::error &e) {
    throw std::runtime_error{
        std::format("Error parsing command line: {}\n", e.what())};
  } catch (const std::runtime_error &e) {
    throw e;
  } catch (...) {
    throw std::runtime_error{std::format("Error parsing command line: {}\n",
                                         "Something unexpected happened")};
  }
}

void CommandOptions::printHelp() { std::cout << desc_ << "\n"; }