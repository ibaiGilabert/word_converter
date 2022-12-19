#include "Translator.h"
#include "InputParserLine.h"
#include "OutputConsole.h"
#include "OutputFile.h"

#include "tclap/CmdLine.h"  // third-party library to parse command line arguments

#include <filesystem>
#include <iostream>

int main(int argc, char **argv)
{
    // some default constants
    const char kDefaultDelimiter = '.';
    const unsigned int kDefaultNumThreads = 1;
    const unsigned int kMaxThreadsAllowed = 8;

    try {
        TCLAP::CmdLine cmd("Text to number converter", ' ', "0.1");

        // define arguments
        TCLAP::ValueArg<std::string> cmdInputFile("i", "input_file", "Input file to parse.", true, "", "string");
        TCLAP::ValueArg<std::string> cmdOutputFile("o", "output_file", "Output file.", false, "", "string");
        TCLAP::ValueArg<unsigned int> cmdMaxThread("j", "max_threads", "Maximum number of threads allowed for concurrence (up to 8).", false, kDefaultNumThreads, "unsigned int");

        // add arguments
        cmd.add(cmdInputFile);
        cmd.add(cmdOutputFile);
        cmd.add(cmdMaxThread);

        // parse them
        cmd.parse(argc, argv);

        // get the input file
        std::string inputFile = cmdInputFile.getValue();
        if (!std::filesystem::exists(inputFile)) {
            std::cerr << "[ERROR] input file <" << inputFile << "> does not exist." << std::endl;
            return 0;
        }

        // get the maximum number of threads (optional)
        unsigned int maxThreads = cmdMaxThread.getValue() > kMaxThreadsAllowed ? kMaxThreadsAllowed : cmdMaxThread.getValue();

        // create translator
        Translator translator(maxThreads);

        // input streams
        std::unique_ptr<input::IInput> parserLine1 = std::make_unique<InputParserLine>(inputFile, kDefaultDelimiter);
        translator.AddInputStream(parserLine1);

        // output streams [console log]
        std::unique_ptr<output::IOutput> parserOutputConsole = std::make_unique<OutputConsole>();
        translator.AddOutputStream(parserOutputConsole);

        // output streams [output file] (optional)
        if (cmdOutputFile.isSet()) {
            std::unique_ptr<output::IOutput> parserOutputFile = std::make_unique<OutputFile>(cmdOutputFile.getValue());
            translator.AddOutputStream(parserOutputFile);
        }

        // run translator
        translator.Run();
    }
    catch (TCLAP::ArgException &e)  // catch exceptions
    {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }
    return 0;
}
