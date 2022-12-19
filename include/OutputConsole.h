#ifndef OUTPUTCONSOLE_H
#define OUTPUTCONSOLE_H

#include "IOutput.h"

#include <mutex>

class OutputConsole : public output::IOutput
{
public:
    explicit OutputConsole();
    ~OutputConsole() override = default;

    int Initialize() override;
    int Finalize() override;

    int Write(const std::string& data_) const override;

private:
    mutable std::mutex mMtx;
};

#endif // OUTPUTCONSOLE_H
