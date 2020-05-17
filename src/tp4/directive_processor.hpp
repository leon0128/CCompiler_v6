#pragma once

#include <vector>

struct PreprocessingFile;
struct PreprocessingToken;

class DirectiveProcessor
{
public:
    DirectiveProcessor(const PreprocessingFile* const,
                       std::vector<PreprocessingToken*>&) noexcept(true);

    bool execute();

private:
    const PreprocessingFile* const mPPFile;
    std::vector<PreprocessingToken*>& mPPTokenVec;

    bool mIsValid;
};