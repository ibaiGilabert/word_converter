#ifndef OUTPUTFILE_H
#define OUTPUTFILE_H

#include "IOutput.h"

#include <fstream>
#include <mutex>

class OutputFile : public output::IOutput
{
public:
    explicit OutputFile(const std::string& filename_);
    ~OutputFile() override;

    int Initialize() override;
    int Finalize() override;

    int Write(const std::string& data_) const override;

private:
    int Close();

private:
    mutable std::mutex mMtx;
    mutable std::ofstream mStream;
    std::string mFilename;
};

#endif // OUTPUTFILE_H
