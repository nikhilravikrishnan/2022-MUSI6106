//
// Created by Nikhil Ravikrishnan on 2/17/22.
//

# include "test.h"
# include <iostream>

using std::cout;

testCombFilter::testCombFilter() {

    this->testSetup();

}

void  testCombFilter::testSetup()
{
    {
        CAudioFileIf::create(phAudioFile);
        CAudioFileIf::create(hOutputFile);
        CCombFilterIf::create(testFilter);

        inputSequence = new float*[stFileSpec.iNumChannels];
        outputSequence = new float*[stFileSpec.iNumChannels];
        for (int i = 0; i < stFileSpec.iNumChannels; i++) {
            inputSequence[i] = new float[blockSize];
            outputSequence[i] = new float[blockSize];
        }

        phAudioFile->openFile(inputFilename, CAudioFileIf::kFileRead);

        phAudioFile->getFileSpec(stFileSpec);

        hOutputFile->openFile(outputFilename, CAudioFileIf::kFileWrite, &stFileSpec);


        testFilter->init(CCombFilterIf::kCombFIR, 1000, stFileSpec.fSampleRateInHz, 1);
        testFilter->setParam(CCombFilterIf::kParamGain, gain);
        testFilter->setParam(CCombFilterIf::kParamDelay, delay*stFileSpec.fSampleRateInHz);
    }
}

void testCombFilter::runFilter()
{
    while (!phAudioFile->isEof())
    {
        // set block length variable
        long long int iNumFrames = blockSize;

        // read data (iNumOfFrames might be updated!)
        phAudioFile->readData(inputSequence, iNumFrames);

        testFilter->process(inputSequence, outputSequence, iNumFrames);

        hOutputFile->writeData(outputSequence, iNumFrames);
    }
}

int testCombFilter::feedforwardFIRTest()
{
    inputSequence = new float*[blockSize];
    outputSequence = new float*[blockSize];

    runFilter();

    for (int i = 0; i < blockSize; i++)
    {
        if(*outputSequence[i] != 0)
        {
//            TODO: Delete objects before returning
            return 0;
        }
    }
    return 1;
}

int testCombFilter::feedforwardIIRTest()
{

    inputSequence = new float*[blockSize];
    outputSequence = new float*[blockSize];

    testFilter->init(CCombFilterIf::kCombIIR, 1000, stFileSpec.fSampleRateInHz, 1);
    runFilter();

    return 1;


}

int testCombFilter::testVaryingBlockSize()
{
    blockSize = 512;
    inputSequence = new float*[blockSize];
    outputSequence = new float*[blockSize];

    try {
        runFilter();
    }
    catch (int e)
    {
        cout<<"Exception:"<<e<<'\n';
        return 0;
    }
        return 1;


}

int testCombFilter::testZeroInputSignal()
{
    float** zeroBuffer = 0;
    for (int i = 0; i < blockSize; i++)
    {
        *zeroBuffer[i] = 0;
    }
    testFilter->process(zeroBuffer, outputSequence, 1024);

    return 1;

}

void testCombFilter::freeTestMemory()
{
    delete[] inputSequence;
    delete[] outputSequence;
}




