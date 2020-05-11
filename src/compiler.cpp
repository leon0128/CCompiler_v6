#include "compiler.hpp"
#include "tp1/translation_phase_1.hpp"
#include "tp2/translation_phase_2.hpp"
#include "tp3/translation_phase_3.hpp"
#include "tp4/translation_phase_4.hpp"
#include <iostream>

Compiler::Compiler():
    mSrc(),
    mTP3TokenVec(),
    mPPTokenVec(),
    mIsValid(true)
{
}

Compiler::~Compiler()
{
}

bool Compiler::execute(int argc, char** argv)
{
    checkArgs(argc);

    if(mIsValid)
    {
        TP1 tp1(mSrc);
        mIsValid = tp1.execute(argv[1]);
    }

    if(mIsValid)
    {
        TP2 tp2(mSrc);
        mIsValid = tp2.execute();
    }

    if(mIsValid)
    {
        TP3 tp3(mSrc, mTP3TokenVec);
        mIsValid = tp3.execute();
    }

    if(mIsValid)
    {
        TP4 tp4(mTP3TokenVec, mPPTokenVec);
        mIsValid = tp4.execute();
    }

    return mIsValid;
}

void Compiler::checkArgs(int argc)
{
    if(argc != 2)
    {
        mIsValid = false;
        std::cout << "Compiler error:\n"
                     "    what: number of command line arguments is invalid.\n"
                     "    num of args: " << argc
                  << std::endl;
    }
}