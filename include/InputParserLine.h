#ifndef INPUTPARSERLINE_H
#define INPUTPARSERLINE_H

#include "IInput.h"

#include <fstream>
#include <mutex>

class InputParserLine : public input::IInput
{
public:

    explicit InputParserLine(const std::string& fileName_, char delim_);
    ~InputParserLine() override;

    int Initialize() override;
    int Finalize() override;

    input::InputCode Read(std::string& data_) override;

private:
    int Close();

private:
    std::mutex mMtx;
    std::ifstream mStream;
    std::string mFilename;
};

#endif // INPUTPARSERLINE_H
