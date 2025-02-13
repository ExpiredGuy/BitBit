#include "tuning.h"
#include <sstream>
#include <vector>

std::vector<EngineParam*> tuningParams;

void registerParam(EngineParam* param) {
  tuningParams.emplace_back(param);
}

EngineParam* findParam(const std::string& name) {
  for (auto* param : tuningParams)
    if (param->name == name)
      return param;
  return nullptr;
}

std::string paramsToUci() {
  std::ostringstream ss;
  for (auto* param : tuningParams)
    ss << "option value " << param->name << " type spin default " 
       << param->value << " minimum -999999999 maximum 999999999\n";
  return ss.str();
}

std::string paramsToSpsaInput() {
  std::ostringstream ss;
  for (auto* param : tuningParams)
    ss << param->name << ", int, " << param->value 
       << ", " << param->min << ", " << param->max 
       << ", " << std::max(0.5, (param->max - param->min) / 20.0) 
       << ", 0.002\n";
  return ss.str();
}
