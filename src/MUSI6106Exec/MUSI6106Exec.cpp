
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

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
    std::string sInputFilePath,                 //!< file paths
                sOutputFilePath;

    static const int kBlockSize = 1024;

//    clock_t time = 0;

    float **ppfAudioData = 0;
    float **ppfProcessedData = 0;

    CAudioFileIf *phAudioFile = 0;
    CAudioFileIf *hOutptFile = 0;
    CCombFilterIf *pcCombFilter = 0;
    CAudioFileIf::FileSpec_t stFileSpec;

    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    if (argc < 2)
    {
        cout << "Missing audio input path!";
        return -1;
    }
    else
    {
        sInputFilePath = "/Users/nikhilravikrishnan/Spring2022/ASE/2022-MUSI6106/testFiles/Voice_Mary_Sadness_1.wav";
        sOutputFilePath = "/Users/nikhilravikrishnan/Spring2022/ASE/2022-MUSI6106/testFiles/output.wav";
    }

    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    CAudioFileIf::create(phAudioFile);
    CAudioFileIf::create(hOutptFile);
    CCombFilterIf::create(pcCombFilter);

    pcCombFilter->init(CCombFilterIf::kCombFIR, 0.5, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);

//    Setting the parameters for delay
    pcCombFilter->setParam(CCombFilterIf::kParamGain, 0.3);
    pcCombFilter->setParam(CCombFilterIf::kParamDelay, 500);
    



    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phAudioFile->isOpen())
    {
        cout << "Wave file open error!";
        CAudioFileIf::destroy(phAudioFile);
        return -1;
    }
    phAudioFile->getFileSpec(stFileSpec);

    phAudioFile->openFile(sOutputFilePath, CAudioFileIf::kFileWrite);
    if (!hOutptFile->isOpen())
    {
        cout << "Wave file open error!";
        CAudioFileIf::destroy(hOutptFile);
        return -1;
    }



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
        long long iNumFrames = kBlockSize;

        // read data (iNumOfFrames might be updated!)
        phAudioFile->readData(ppfAudioData, iNumFrames);
        pcCombFilter->process(ppfAudioData, ppfProcessedData, iNumFrames);

        cout << "\r" << "reading and writing";

        // write
//        for (int i = 0; i < iNumFrames; i++)
//        {
//            for (int c = 0; c < stFileSpec.iNumChannels; c++)
//            {
//
//                hOutputFile << ppfProcessedData[c][i] << "\t";
//            }
//            hOutputFile << endl;
//        }
        hOutptFile->writeData(ppfProcessedData, iNumFrames);
    }

    cout << "\nreading/writing done in: \t" << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up (close files and free memory)
    CAudioFileIf::destroy(phAudioFile);
    CAudioFileIf::destroy(hOutptFile);
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

