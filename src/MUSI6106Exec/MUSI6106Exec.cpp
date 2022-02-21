
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include "MUSI6106Config.h"
#include "test.h"
#include <assert.h>

#include "AudioFileIf.h"
#include "CombFilterIf.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{

    if(argc == 1 )
    {
        testCombFilter* testFilter;
        testFilter = new testCombFilter();
//        testFilter->testSetup();
        assert(testFilter->feedforwardFIRTest() == 1);
        assert(testFilter->feedforwardIIRTest() == 1);
        assert(testFilter->testVaryingBlockSize() == 1);
        assert(testFilter->testZeroInputSignal() == 1);

        return 0;


    }



    std::string sInputFilePath,                 //!< file paths
    sOutputFilePath;

    static const int kBlockSize = 1024;

//    clock_t time = 0;

    float **ppfAudioData = 0;
    float **ppfProcessedData = 0;

    CAudioFileIf *phAudioFile = 0;
    CAudioFileIf *hOutputFile = 0;
    CCombFilterIf *pcCombFilter = 0;
    CAudioFileIf::FileSpec_t stFileSpec;

    showClInfo();
    // parse command line arguments
    if (argc < 4)
    {
        cout << "Missing audio input path!";
        return -1;
    }
    else
    {
        sInputFilePath = argv[1];
        sOutputFilePath = argv[2];
    }

    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file, output file and
    CAudioFileIf::create(phAudioFile);
    CAudioFileIf::create(hOutputFile);
    CCombFilterIf::create(pcCombFilter);


    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phAudioFile->isOpen())
    {
        cout << "Wave file open error!";
        CAudioFileIf::destroy(phAudioFile);
        return -1;
    }
    phAudioFile->getFileSpec(stFileSpec);

    hOutputFile->openFile(sOutputFilePath, CAudioFileIf::kFileWrite, &stFileSpec);
    if (!hOutputFile->isOpen())
    {
        cout << "Wave file open error!";
        CAudioFileIf::destroy(hOutputFile);
        return -1;
    }


    //////////////////////////////////////////////////////////////////////////////
//  Init Filter
    pcCombFilter->init(CCombFilterIf::kCombFIR, 0.5, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);

//    Setting the parameters for delay
    pcCombFilter->setParam(CCombFilterIf::kParamGain, atof(argv[3]));
    pcCombFilter->setParam(CCombFilterIf::kParamDelay, atof(argv[4])*stFileSpec.fSampleRateInHz);

    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    ppfAudioData = new float*[stFileSpec.iNumChannels];
    ppfProcessedData = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++) {
        ppfAudioData[i] = new float[kBlockSize];
        ppfProcessedData[i] = new float[kBlockSize];
    }

    if (ppfAudioData == 0)
    {
        CAudioFileIf::destroy(phAudioFile);
        return -1;
    }
    if (ppfAudioData[0] == 0)
    {
        CAudioFileIf::destroy(phAudioFile);
        return -1;
    }

    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output text file (one column per channel)
    while (!phAudioFile->isEof())
    {
        // set block length variable
        long long int iNumFrames = kBlockSize;

        // read data (iNumOfFrames might be updated!)
        phAudioFile->readData(ppfAudioData, iNumFrames);

        pcCombFilter->process(ppfAudioData, ppfProcessedData, iNumFrames);


        hOutputFile->writeData(ppfProcessedData, iNumFrames);
    }

    cout << "\nreading/writing done in: \t" << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up (close files and free memory)
    CAudioFileIf::destroy(phAudioFile);
    CAudioFileIf::destroy(hOutputFile);
    for (int i = 0; i < stFileSpec.iNumChannels; i++) {
        delete[] ppfAudioData[i];
        delete[] ppfProcessedData[i];
    }
    delete[] ppfAudioData;
    delete[] ppfProcessedData;
    ppfAudioData = 0;
    ppfProcessedData = 0;

    // all done
    return 0;

}


void     showClInfo()
{
    cout << "MUSI6106 Assignment Executable" << endl;
    cout << "(c) 2014-2022 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

