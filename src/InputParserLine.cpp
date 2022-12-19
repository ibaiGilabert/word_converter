#include "InputParserLine.h"
#include <iostream>

InputParserLine::InputParserLine(const std::string& filename_, char delim_) :
    input::IInput(delim_),
    mFilename(filename_)
{}

InputParserLine::~InputParserLine()
{
    Close();
}

int InputParserLine::Initialize()
{
    std::unique_lock<std::mutex> lock(mMtx);
    if (mStream.is_open()) {
        std::cout << "Parser already opened!\n";
        return -1;
    }
    mStream.open(mFilename);
    return 0;
}

int InputParserLine::Finalize()
{
    return Close();
}

input::InputCode InputParserLine::Read(std::string& data_)
{
    std::unique_lock<std::mutex> lock(mMtx);
    // take into account empty lines within the file
    input::InputCode res(input::InputCode::kInputCodeError);
    if (mStream.is_open()) {
        switch (mStream.peek()) {
        case '\r':  //CR
        case '\n':  //LF
            // don't want to treat CR+LF
            std::getline(mStream, data_);
            data_ = "\n";
            res = input::InputCode::kInputCodeEndOfLine;
            break;
        case EOF:
            //mStream.close();
            res = input::InputCode::kInputCodeEOF;
            break;
        default:
            std::getline(mStream, data_, mDelim);
            //data_ += mDelim;
            res = input::InputCode::kInputCodeDelim;
            break;
        }
    }
    else {
        std::cout << "Parser line needs to be initialized.\n";
        res = input::InputCode::kInputCodeError;
    }
    return res;
}

int InputParserLine::Close()
{
    std::unique_lock<std::mutex> lock(mMtx);
    // ensure properly closing
    if (mStream.is_open()) {
        mStream.close();
    }
    else {
        std::cout << "Parser already closed!\n";
        return -1;
    }
    return 0;
}
