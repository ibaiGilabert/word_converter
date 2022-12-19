#include "OutputFile.h"

#include <iostream>

OutputFile::OutputFile(const std::string& filename_) :
    output::IOutput(),
    mFilename(filename_)
{}

OutputFile::~OutputFile()
{
    Close();
}

int OutputFile::Initialize()
{
    std::unique_lock<std::mutex> lock(mMtx);
    if (mStream.is_open()) {
        std::cout << "File already opened!\n";
        return -1;
    }
    mStream.open(mFilename);
    return 0;
}

int OutputFile::Finalize()
{
    return Close();
}

int OutputFile::Write(const std::string& data_) const
{
    std::unique_lock<std::mutex> lock(mMtx);
    mStream << data_;
    return 0;
}

int OutputFile::Close()
{
    std::unique_lock<std::mutex> lock(mMtx);
    // ensure properly closing
    if (mStream.is_open()) {
        mStream.close();
    }
    else {
        std::cout << "File already closed!\n";
        return -1;
    }
    return 0;
}
