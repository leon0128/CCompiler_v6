#pragma once

#include <string>
#include <vector>

struct TP3Token;

class Compiler
{
public:
    Compiler();
    ~Compiler();

    bool execute(int argc, char** argv);

private:
    void checkArgs(int argc);

    std::string mSrc;
    std::vector<TP3Token*> mTP3TokenVec;

    bool mIsValid;
};