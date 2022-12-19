#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "ThreadPool.h"
#include "IInput.h"
#include "IOutput.h"

#include <thread>
#include <vector>
#include <list>

class Translator
{
public:
    explicit Translator(unsigned int maxThreads_);
    ~Translator();

    void AddInputStream(std::unique_ptr<input::IInput>& input_);
    void AddOutputStream(std::unique_ptr<output::IOutput>& output_);

    void Run();

private:
    void InitializeStreams();
    void ProcessInput(input::IInput* input_);
    void ProcessOutput(const std::string& dataOut_);

    void TranslateData(const std::string& dataIn_, std::string &dataOut_);

private:
    std::list<std::unique_ptr<input::IInput>> mInputList;
    std::list<std::unique_ptr<output::IOutput>> mOutputList;
    std::vector<std::thread> mThreads;

    ThreadPool mThreadPool;
};

#endif // TRANSLATOR_H
