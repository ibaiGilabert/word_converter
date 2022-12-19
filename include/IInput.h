#ifndef IINPUT_H
#define IINPUT_H

#include <string>

namespace input {

enum class InputCode {
    kInputCodeError = -1,
    kInputCodeEndOfLine,
    kInputCodeDelim,
    kInputCodeEOF
};

class IInput
{
public:
    explicit IInput(char delim_) : mDelim(delim_) {}
    virtual ~IInput() = default;

    virtual int Initialize() = 0;
    virtual int Finalize() = 0;

    virtual input::InputCode Read(std::string& data_) = 0;

    inline char GetDelim() const { return mDelim; };

protected:
    char mDelim;
};

}   // end namespace

#endif // IINPUT_H
