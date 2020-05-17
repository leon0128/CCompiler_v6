#include "directive_processor.hpp"
#include "../lexical_elements.hpp"

DirectiveProcessor::DirectiveProcessor(const PreprocessingFile* const ppFile,
                                       std::vector<PreprocessingToken*>& ppTokenVec) noexcept(true):
    mPPFile(ppFile),
    mPPTokenVec(ppTokenVec),
    mIsValid(true)
{
}

bool DirectiveProcessor::execute()
{
    return mIsValid;
}