#pragma once

// #include "output.h"

// #include <string>
// #include <list>
// #include <memory>
// #include <algorithm>
// #include <chrono>
// #include <ostream>
// #include <typeinfo>
// #include <format>
// #include <fstream>

class ICmd {
public:
    ICmd() {}
    virtual ~ICmd() = default;
    virtual bool exec() = 0;
    virtual std::string getResult() = 0;
};

class NoCmd : public ICmd {
public:
    NoCmd() {}
    virtual ~NoCmd() = default;
    bool exec() override { return false; }
    std::string getResult() override { return std::string("no command"); }
};

