#include "compiler.hpp"

int main(int argc, char** argv)
{
    Compiler compiler;

    if(compiler.execute(argc, argv))
        return 0;
    else
        return 1;
}