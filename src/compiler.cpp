#include "compiler.hpp"
#include "tp1/translation_phase_1.hpp"
#include <iostream>

Compiler::Compiler():
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
        TP1 tp1;
        mIsValid = tp1.execute(argv[1]);
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