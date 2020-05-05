#include "translation_phase_1.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

const std::unordered_map<char, char> TP1::TRIGPAPH_MAP
    = {{'=', '#'},
       {'(', '['},
       {'/', '\\'},
       {')', ']'},
       {'\'', '^'},
       {'<', '{'},
       {'!', '|'},
       {'>', '}'},
       {'-', '~'}};

unsigned int TP1::NUM_EXEC = 0;

TP1::TP1(std::string& str):
    mSrc(str),
    mIsValid(true)
{
}

bool TP1::execute(const char* filename)
{
    checkFilename(filename);
    
    if(mIsValid)
        replaceTrigraphSequence();

    if(mIsValid)
        outputResult(filename);

    return mIsValid;
}

void TP1::checkFilename(const char* filename)
{
    std::ifstream fstr(filename, std::ios::in);
    
    if(fstr.is_open())
    {
        std::stringstream sstr;
        sstr << fstr.rdbuf();
        fstr.close();
        mSrc = sstr.str();
    }
    else
    {
        mIsValid = false;
        std::cout << "TP1 error:\n"
                     "    what: source file cannot be opened.\n"
                     "    file name: " << filename
                  << std::endl;
    }
}

void TP1::replaceTrigraphSequence()
{
    for(std::size_t pos = mSrc.find("??"); pos + 2 < mSrc.size() && pos != std::string::npos; pos = mSrc.find("??", pos))
    {
        auto iter = TRIGPAPH_MAP.find(mSrc[pos + 2]);
        if(iter != TRIGPAPH_MAP.end())
            mSrc.replace(pos, 3, 1, iter->second);
        
        pos++;
    }
}

void TP1::outputResult(const char* filename) const
{
    std::string resFilename = "res/tp1_";
    resFilename += std::to_string(NUM_EXEC);
    resFilename += ".log";

    std::ofstream fstr(resFilename.c_str(), std::ios::out);
    
    if(fstr.is_open())
    {
        fstr << "===== Translation phase 1 =====\n"
                ">> filename: " << filename << "\n"
                ">> NUM_EXEC: " << NUM_EXEC << "\n"
                ">> result:\n"
             << mSrc;
        fstr.close();
    }
    else
    {
        std::cout << "TP1 warning:\n"
                     "    what: TP1 log file cannot be created because log file cannot be opened.\n"
                     "    filename: " << resFilename
                  << std::endl;
    }

    NUM_EXEC++;
}