#pragma once

#include <string>

namespace TP2
{

class TP2
{
private:
    static unsigned int NUM_EXEC;

public:
    TP2(std::string& str);

    bool execute();

private:
    void splice();
    void outputResult() const;

    std::string& mSrc;

    bool mIsValid;
};

}