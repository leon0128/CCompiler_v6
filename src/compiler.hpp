    #pragma once

class Compiler
{
public:
    Compiler();
    ~Compiler();

    bool execute(int argc, char** argv);

private:
    void checkArgs(int argc);

    bool mIsValid;
};