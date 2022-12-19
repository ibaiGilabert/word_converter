#include "OutputConsole.h"

#include <iostream>

OutputConsole::OutputConsole() :
    output::IOutput()
{}


int OutputConsole::Initialize()
{
    // do nothing
    return 0;
}

int OutputConsole::Finalize()
{
    // do nothing
    return 0;
}

int OutputConsole::Write(const std::string& data_) const
{
    std::unique_lock<std::mutex> lock(mMtx);
    std::cout << data_;
    return 0;
}
