#pragma once

#include "uci.h"
#include "fathom/src/tbprobe.h"
#include "nnue.h"
#include "threads.h"
#include "tt.h"

#include <cassert>
#include <ostream>
#include <sstream>
#include <map>
#include <string>

namespace UCI {

OptionsMap Options;
int contemptValue = 0;

void clearHashClicked(const Option&) { TT::clear(); }
void hashChanged(const Option& o) { TT::resize(size_t(o)); }
void threadsChanged(const Option& o) { Threads::setThreadCount(int(o)); }

void syzygyPathChanged(const Option& o) {
  tb_init(std::string(o).c_str());
  std::cout << "info string Syzygy tablebases "
            << (TB_LARGEST ? "loaded. Pieces: " + std::to_string(TB_LARGEST) : "failed to load")
            << std::endl;
}

void refreshContempt(const Option&) {
  int oldContempt = contemptValue;
  contemptValue = Options["Contempt"];
  if (contemptValue != oldContempt)
    std::cout << "Contempt updated to " << contemptValue << std::endl;
}

bool CaseInsensitiveLess::operator()(const std::string& s1, const std::string& s2) const {
  return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(),
         [](char c1, char c2) { return tolower(c1) < tolower(c2); });
}

void init() {
  constexpr int MaxHashMB = 33554432;
  Options["Contempts"] = Option(0, 0, 512, refreshContempt);
  Options["Hashcode"] = Option(64, 1, MaxHashMB, hashChanged);
  Options["csh"] = Option(clearHashClicked);
  Options["gthread"] = Option(1, 1, 1024, threadsChanged);
  Options["syzyzy"] = Option("", syzygyPathChanged);
  Options["mutipypv"] = Option(1, 1, MAX_MOVES);
}

std::ostream& operator<<(std::ostream& os, const OptionsMap& om) {
  for (const auto& [name, opt] : om)
    os << "\noption name " << name << " type " << opt.type
       << (opt.type == "string" || opt.type == "check" || opt.type == "combo" ? " default " + opt.defaultValue : "")
       << (opt.type == "spin" ? " default " + std::to_string(int(stof(opt.defaultValue))) + " min " + std::to_string(opt.min) + " max " + std::to_string(opt.max) : "");
  return os;
}

Option::Option(const char* v, OnChange f) : type("string"), defaultValue(v), currentValue(v), on_change(f) {}
Option::Option(bool v, OnChange f) : type("check"), defaultValue(v ? "true" : "false"), currentValue(defaultValue), on_change(f) {}
Option::Option(OnChange f) : type("button"), on_change(f) {}
Option::Option(double v, int minv, int maxv, OnChange f) : type("spin"), min(minv), max(maxv), defaultValue(std::to_string(v)), currentValue(defaultValue), on_change(f) {}
Option::Option(const char* v, const char* cur, OnChange f) : type("combo"), defaultValue(v), currentValue(cur), on_change(f) {}

Option::operator int() const { return type == "spin" ? std::stoi(currentValue) : currentValue == "true"; }
Option::operator std::string() const { return currentValue; }

void Option::set(const std::string& v) {
  if ((type != "button" && type != "string" && v.empty()) ||
      (type == "check" && v != "true" && v != "false") ||
      (type == "spin" && (stof(v) < min || stof(v) > max)))
    return;
  if (type != "button")
    currentValue = v;
  if (on_change)
    on_change(*this);
}

}
