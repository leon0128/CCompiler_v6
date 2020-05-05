#pragma once

#include <string>
#include <vector>

class TP3
{
public:
    TP3(std::string& src);

private:
    std::string& mSrc;

    bool mIsValid = true;
};