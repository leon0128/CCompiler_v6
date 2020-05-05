#pragma once

#include <string>
#include <unordered_map>

class TP1
{
private:
    static const std::unordered_map<char, char> TRIGPAPH_MAP;
    static unsigned int NUM_EXEC;

public:
    TP1();

    bool execute(const char* filename);

private:
    void checkFilename(const char* filename);
    void replaceTrigraphSequence();
    void outputResult(const char* filename) const;

    std::string mSrc;

    bool mIsValid;
};