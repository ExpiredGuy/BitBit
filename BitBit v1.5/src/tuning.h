#pragma once

#include <iostream>

struct EngineParam {
  std::string name;
  int value, min, max;

  EngineParam(std::string _name, int _value, int _min, int _max)
      : name(_name), value(_value), min(_min), max(_max) {
    registerParam(this);
  }

  EngineParam(std::string _name, int _value, int _step)
      : EngineParam(_name, _value, _value - 10 * _step, _value + 10 * _step) {}

  inline operator int() const { return value; }
};

void registerParam(EngineParam* param);
EngineParam* findParam(std::string name);
std::string paramsToUci();
std::string paramsToSpsaInput();

#ifdef DO_TUNING
constexpr bool doTuning = true;
#define DEFINE_PARAM_S(_name, _value, _step) EngineParam _name(#_name, _value, _step)
#define DEFINE_PARAM_B(_name, _value, _min, _max) EngineParam _name(#_name, _value, _min, _max)
#else
constexpr bool doTuning = false;
#define DEFINE_PARAM_S(_name, _value, _step) constexpr int _name = _value
#define DEFINE_PARAM_B(_name, _value, _min, _max) constexpr int _name = _value
#endif
