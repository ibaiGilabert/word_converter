#ifndef IOUTPUT_H
#define IOUTPUT_H

#include <string>

namespace output {

class IOutput
{
public:
    virtual ~IOutput() = default;

    virtual int Initialize() = 0;
    virtual int Finalize() = 0;

    virtual int Write(const std::string& data_) const = 0;
};

}   // end namespace

#endif // IOUTPUT_H
