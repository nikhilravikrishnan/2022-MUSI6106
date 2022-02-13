# include "CombFilter.h"
# include "CombFilterIf.h"
# include "RingBuffer.h"

CCombFilterBase::CCombFilterBase()
{
    this->initDefaults();
}

Error_t CCombFilterBase::initDefaults()
{

    // m_fDelayInSamples = getParam(FilterParam_t::kParamDelay);
    // m_gain = getParam(FilterParam_t::kParamGain);

    m_fDelayInSamples = 100;
    m_fSampleRateInHz = 16000;
    m_gain = 0.5;
    m_iNumChannels = 2;
    kBlockSize = 1024;

    CRingBuffer<float> **pCRingBuff = new CRingBuffer<float>* [m_iNumChannels];
    for (int i = 0; i < m_iNumChannels; i++)
        pCRingBuff[i] = new CRingBuffer<float>(kBlockSize);

}

Error_t CCombFilterBase::init(CombFilterType_t eFilterType, float fMaxDelayLengthInS, float fSampleRateInHz, int iNumChannels)
{
    m_eFilterType = eFilterType;
    m_fMaxDelayLengthInS = fMaxDelayLengthInS;
    m_fSampleRateInHz = fSampleRateInHz;
    m_iNumChannels = iNumChannels;
}


Error_t CCombFilterBase::processBuffer(float **ppfInputBuffer, float **ppfOutPutBuffer, int iNumberOfFrames)
{
    if(CCombFilterBase::CombFilterType_t == 0)
        processFIR(ppfInputBuffer, ppfOutPutBuffer, iNumberOfFrames);
    
    if(CCombFilterBase::CombFilterType_t == 1)
        processIIR(ppfInputBuffer, ppfOutPutBuffer, iNumberOfFrames);
}

Error_t CCombFilterBase::processFIR(float **ppfInputBuffer, float **ppfOutPutBuffer, int iNumberOfFrames)
{
    for (int i =0; i < m_iNumChannels; i++)
        {
            for (int j = 0; j < m_fDelayInSamples; j++) {
                pCRingBuff[i]->putPostInc(0.F * j);
            }
        }
    for (int i = 0; i < m_iNumChannels; i++)
        pCRingBuff[i]->setReadIdx(0);

    for (int i = 0; i < m_iNumChannels; i++)
    {
        for (int j = 0; j < kBlockSize; j++)
        {
            ppfOutPutBuffer[i][j] = ppfInputBuffer[i][j] + m_gain*pCRingBuff[i]->getPostInc();
            pCRingBuff[i]->putPostInc(ppfInputBuffer[i][j]);

        }
    }
    
}
Error_t CCombFilterBase::processIIR(float **ppfInputBuffer, float **ppfOutPutBuffer, int iNumberOfFrames)
{
    for (int i = 0; i < m_iNumChannels; i++)
    {
        for (int j = 0; j < m_fDelayInSamples; j++)
        {
            pCRingBuff[i]->putPostInc(0.F*j);
        }
    }
    for (int i = 0; i < m_iNumChannels; i++)
        pCRingBuff[i]->setReadIdx(0);

    for (int i = 0; i < m_iNumChannels; i++)
    {
        for (int j = 0; j < kBlockSize; j++)
        {
            ppfOutPutBuffer[i][j] = ppfInputBuffer[i][j] + m_gain*pCRingBuff[i]->getPostInc();
            pCRingBuff[i]->putPostInc(ppfOutPutBuffer[i][j]);

        }
    }
    
}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

// CCombFilterIIR::CCombFilterIIR()
// {
//     this->initDefaults();
// }

// Error_t CCombFilterIIR::processBuffer(float **inputBuffer, float **outputBuffer, int numberOfFrames)
// {

// }

// //////////////////////////////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////////////////////////////

// CCombFilterFIR::CCombFilterFIR()
// {
//     this->initDefaults();
// }


