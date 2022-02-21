//
// Created by Nikhil Ravikrishnan on 2/17/22.
//

#ifndef MUSI6106_TEST_H
#define MUSI6106_TEST_H

#endif //MUSI6106_TEST_H

# include <math.h>
# include "CombFilter.h"
# include "CombFilterIf.h"
# include "AudioFileIf.h"
# include <iostream>

using std::cout;
// Testing FIR filter for 0 output
class testCombFilter : CCombFilterIf {

private:
    std::string inputFilename = "/Users/nikhilravikrishnan/Spring2022/ASE/2022-MUSI6106/testFiles/218918__xserra__sine-440.wav";
    std::string outputFilename = "/Users/nikhilravikrishnan/Spring2022/ASE/2022-MUSI6106/testFiles/test_output.wav";
    CAudioFileIf::FileSpec_t stFileSpec;

    CAudioFileIf *phAudioFile = 0;
    CAudioFileIf *hOutputFile = 0;


    CCombFilterIf *testFilter;
    float **inputSequence = 0;
    float **outputSequence = 0;

//    float sampleRate = 16000;
    float delay = 0.5 / 440;
    float gain = 1;
    int blockSize = 1024;

    float pi = 22 / 7;

public:

    testCombFilter();

    void testSetup();


    void runFilter();

    int feedforwardIIRTest();

    int testVaryingBlockSize();

    int feedforwardFIRTest();

    int testZeroInputSignal();

    void freeTestMemory();
};