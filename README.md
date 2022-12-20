# word_converter

Converts all numbers English written into digits of the provided text.


## Development notes

* The program has 3 well defined and decoupled components:
	1.	Translator's engine.
	2.	In/Out interfaces.
	3.	Translator module.  

* The Translation class is a skeleton binds all the components in a agnostic way. It accepts a list of both input and output interfaces. 
* Running a translation means initalize the input streams and bind each one to a dedicated thread. Each of these threads will provide data packages which feed the translation module called 'TextValue'. 
* Once the translation has been completed, the Translation class delegates the outcome to all the output streams in parallel. To sum up:
	1.	Each input stream is working on its own thread. Since Translation class does not know anything about the streams' implementation, it must be aware of likely blocking scenerios such as waiting for a user's inoput or polling any other entity. In this particular case we have just one input stream (input file) but we think any piece of code should be always scalable.
	2.	The Translator class has a thread pool available. Once a data package is translated, the Translator delegates the output streaming to the pool. As said before, the Translator cannot know how long an I/O operation will take and must be prepared for slow blocking sceneraios such as system file or database access.
* TextValue is the class in charge of converting text numbers to their values from data received. To carry this task out, it implements a small state machine to identify the written numbers. They are 3 states to identify literal numbers, multiplier factors and connection words/characters in between. The states are defined following the singleton pattern since there is no reason to have multiple instance alive. A given state just defines a workflow, a behavior.
* We are following some of the rules defined [Grammar Book](https://www.grammarbook.com/numbers/numbers.asp). However, there is still to much work to do. The negative and decimal numbers are not supported, to mention just a few.

* The TextValue class could be also implemented as an interface. In that way, we will be able to add/change the translation module's behavior following a 'Plug&Play' philosophy. Therefore, no code changes in Translator's class would be required. 

* Notice the main thread is doing nothing remarkable. It justs parse the command line arguments and starts up the translator. It is a deliberate decision in order to be able to add, for instance, some kind of GUI on top or any other component which may require a full responsive interaction.

* The approach we follow is based on code's maintenance and scalability taking the advantage of:
	1.	Using design patterns to decouple components.
	2.	Writing small and clear functions. Although, we are aware of the lack of code documentation and we apologize for that.


## Build

The application is using CMake as a build system. So, to build the application follow the steps below:

```bash
$mkdir build
$cd build
$cmake ..
```
Then, build the compile's command depending on the toolchain you prefer. The application has been tested under these platforms:

* MinGW 11.2.0 (Windows 10).
* g++ 9.4.0 (Ubuntu 20.04.5).
* clang 13.0.0 (macOS 11.6.7).

Please, just make sure you have a compìler c++20 compliant.


## Usage

The application accepts the following input parameters:

```bash
USAGE: 

   ./word_converter  [-j <unsigned int>] [-o <string>] -i <string> [--]
                     [--version] [-h]


Where: 

   -j <unsigned int>,  --max_threads <unsigned int>
     Maximum number of threads allowed for concurrence (up to 8).

   -o <string>,  --output_file <string>
     Output file.

   -i <string>,  --input_file <string>
     (required)  Input file to parse.

   -h,  --help
     Displays usage information and exits.

```
* The multithreading capabillity is used by the output streams. The more threads you have available, the more output streams will work in parallel.

* The output file is optional.

* The input file is a mandatory field.

As mentioned in the development notes, notice the code's architecture is able to deal with several inputs and outputs streams.

Example:
```bash
$word_converter -j 2 -i ../test/t2.txt -o output.txt
```

## Test
### Sample

You will find at `test` directory some sorts of samples to get used to basic usage.

### Unit test

There is no unit test provided. We are sorry about that.


## Third party libraries
### tclap

We are using the tclap library for parsing command line's arguments. It is a header only library and does not impact neither on code's workflow nor architecture. We are just using it to avoid spending time implementing a low valuable feature for the exercice's intended case. Also, tclap provides a quite decent command's line outcome when catching input errors. For all these reasons, we found it justificable and worthwile.