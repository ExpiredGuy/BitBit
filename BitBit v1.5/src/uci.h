#pragma once

#include "position.h"
#include "types.h"
#include <map>
#include <string>

namespace UCI {

  struct CaseInsensitiveLess {
    bool operator()(const std::string&, const std::string&) const;
  };

  using OptionsMap = std::map<std::string, std::string, CaseInsensitiveLess>;

  class Option {
  public:
    using OnChange = void (*)(const Option&);
    Option(const std::string& v = "", OnChange = nullptr);
    void set(const std::string&);
    operator std::string() const;
  private:
    std::string value;
    OnChange on_change;
  };

  void init();
  void loop(int argc, char* argv[]);
  int normalizeToCp(Score v);
  std::string scoreToString(Score v);
  std::string squareToString(Square s);
  std::string moveToString(Move m);
  Move stringToMove(const Position& pos, std::string& str);

  extern OptionsMap Options;
  extern int contemptValue;

}
