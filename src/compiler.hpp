#pragma once

#include <string>

class Compiler
{
public:
    Compiler();
    ~Compiler();

    bool execute(int argc, char** argv);

private:
    void checkArgs(int argc);

    std::string mSrc;

    bool mIsValid;
};