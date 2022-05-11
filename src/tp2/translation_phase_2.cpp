#include "translation_phase_2.hpp"
#include <iostream>
#include <fstream>

unsigned int TP2::TP2::NUM_EXEC = 0;

TP2::TP2::TP2(std::string& str):
    mSrc(str),
    mIsValid(true)
{
}

bool TP2::TP2::execute()
{
    splice();

    if(mIsValid)
        outputResult();

    return mIsValid;
}

void TP2::TP2::splice()
{
    for(std::size_t pos = mSrc.find("\\\n"); pos + 1 < mSrc.size() && pos != std::string::npos; pos = mSrc.find("\\\n", pos))
        mSrc.replace(pos, 2, "");
}

void TP2::TP2::outputResult() const
{
    std::string resFilename = "res/tp2_";
    resFilename += std::to_string(NUM_EXEC);
    resFilename += ".log";

    std::ofstream fstr(resFilename.c_str(), std::ios::out);
    
    if(fstr.is_open())
    {
        fstr << "===== Translation phase 2 =====\n"
                ">> NUM_EXEC: " << NUM_EXEC << "\n"
                ">> result:\n"
             << mSrc;
        fstr.close();
    }
    else
    {
        std::cout << "TP2 warning:\n"
                     "    what: TP2 log file cannot be created because log file cannot be opened.\n"
                     "    filename: " << resFilename
                  << std::endl;
    }

    NUM_EXEC++;
}