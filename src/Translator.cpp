#include "Translator.h"
#include "TextValue.h"


Translator::Translator(unsigned int maxThreads_) :
    mThreadPool(maxThreads_)
{}

Translator::~Translator()
{
    for (auto& thread : mThreads)
    {
        thread.join();
    }
}

void Translator::AddInputStream(std::unique_ptr<input::IInput>& input_)
{
    mInputList.emplace_back(std::move(input_));
}

void Translator::AddOutputStream(std::unique_ptr<output::IOutput> &output_)
{
    mOutputList.emplace_back(std::move(output_));
}

void Translator::Run()
{
    // Initialize streams
    InitializeStreams();

    // Bind process threads
    mThreads.resize(mInputList.size());
    unsigned int i = 0;
    for (auto& input : mInputList)  // (c++20) range-based loop with initializer : )
    {
        mThreads[i++] = std::thread(&Translator::ProcessInput, this, input.get());
    }
}

void Translator::InitializeStreams()
{
    for (auto& input: mInputList)
    {
        if (input->Initialize() != 0) {
            // should we manage errors?
            return;
        }
    }
    for (auto& output: mOutputList)
    {
        if (output->Initialize() != 0) {
            // should we manage errors?
            return;
        }
    }
}

void Translator::ProcessInput(input::IInput* input_)
{
    std::string dataIn;
    input::InputCode res = input_->Read(dataIn);
    while (res != input::InputCode::kInputCodeEOF &&
           res != input::InputCode::kInputCodeError)
    {
        if (res == input::InputCode::kInputCodeDelim) {
            std::string dataOut("");
            TranslateData(dataIn, dataOut);

            dataOut += input_->GetDelim();
            ProcessOutput(dataOut);
        }
        else if (res == input::InputCode::kInputCodeEndOfLine) {
            ProcessOutput(dataIn);
        }
        res = input_->Read(dataIn);
    }
}


void Translator::ProcessOutput(const std::string& dataOut_)
{
    // send data processed to output streams
    for (const auto& output : mOutputList)
    {
        const output::IOutput* raw = output.get();
        // enqueue in Translator's thread's pool
        mThreadPool.Enqueue([=] {
            raw->Write(dataOut_);
        });
    }
}


void Translator::TranslateData(const std::string& dataIn_, std::string& dataOut_)
{
    TextValue textValue;
    textValue.Parse(dataIn_, dataOut_);
}
