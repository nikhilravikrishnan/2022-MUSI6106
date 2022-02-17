# include "CombFilter.h"
# include "CombFilterIf.h"
# include "RingBuffer.h"

CCombFilterBase::CCombFilterBase()
//m_fDelayInSamples(0),
//m_fSampleRateInHz(0),
//m_gain(0),
//m_iNumChannels(0)
{
    m_fDelayInSamples = 0;
    m_gain = 0;
    m_iNumChannels = 0;
}
CCombFilterBase::~CCombFilterBase()
{
    this->initDefaults();
}
Error_t CCombFilterBase::initDefaults()
{

    // m_fDelayInSamples = getParam(FilterParam_t::kParamDelay);
    // m_gain = getParam(FilterParam_t::kParamGain);

    m_fDelayInSamples = 100;
    m_gain = 0.5;
    m_iNumChannels = 1;



}

Error_t CCombFilterBase::init(int iNumChannels)
{
    m_iNumChannels = iNumChannels;
}


Error_t CCombFilterBase::processBuffer(float **ppfInputBuffer, float **ppfOutPutBuffer, int iNumberOfFrames)
{

    // Initializing delay line with zeros

    // Iterating over blocks to add delay
    for (int i = 0; i < m_iNumChannels; i++)
    {
        for (int j = 0; j < iNumberOfFrames; j++)
        {
            ppfOutPutBuffer[i][j] = ppfInputBuffer[i][j] + m_gain*pCRingBuff[i]->getPostInc();

            if (m_eFilterType == kCombFIR)
                pCRingBuff[i]->putPostInc(ppfInputBuffer[i][j]);
            else if(m_eFilterType == kCombIIR)
                pCRingBuff[i]->putPostInc(ppfOutPutBuffer[i][j]);
            else
                return Error_t::kFunctionInvalidArgsError;


        }
    }
}


Error_t CCombFilterBase::setParameters(CCombFilterIf::FilterParam_t eParam, float fParamValue) {

    if (eParam == kParamGain)
        m_gain = fParamValue;
    else if (eParam == kParamDelay) {
        m_fDelayInSamples = fParamValue;
        initBuffer(pCRingBuff);
    }
    else
        return Error_t::kInvalidString;
}

float CCombFilterBase::getParameter(CCombFilterIf::FilterParam_t eParam) {
    if (eParam == kParamDelay)
        return m_fDelayInSamples;
    else if (eParam == kParamGain)
        return m_gain;
}

Error_t CCombFilterBase::setFilterType(CCombFilterIf::CombFilterType_t filterType) {
    m_eFilterType = filterType;
}

Error_t CCombFilterBase::initBuffer(CRingBuffer<float> **pCRingBuffer) {
    pCRingBuff = new CRingBuffer<float> *[m_iNumChannels];
    for (int i = 0; i < m_iNumChannels; i++)
        pCRingBuff[i] = new CRingBuffer<float>(m_fDelayInSamples);

    for (int i = 0; i < m_iNumChannels; i++) {
        for (int j = 0; j < m_fDelayInSamples; j++) {
            pCRingBuff[i]->putPostInc(0.F);
        }
    }
}



Error_t CCombFilterFIR::processBuffer(float **ppfInputBuffer, float **ppfOutPutBuffer, int iNumberOfFrames) {
//    for (int i = 0; i < m_iNumChannels; i++) {
//        for (int j = 0; j < m_fDelayInSamples; j++) {
//            pCRingBuff[i]->putPostInc(0.F * j);
//        }
//    }
//    for (int i = 0; i < m_iNumChannels; i++)
//        pCRingBuff[i]->setReadIdx(0);
//
//    for (int i = 0; i < m_iNumChannels; i++) {
//        for (int j = 0; j < iNumberOfFrames; j++) {
//            ppfOutPutBuffer[i][j] = ppfInputBuffer[i][j] + m_gain * pCRingBuff[i]->getPostInc();
//            pCRingBuff[i]->putPostInc(ppfInputBuffer[i][j]);
//        }
//    }
    return Error_t::kUnknownError;
}

Error_t CCombFilterIIR::processBuffer(float **ppfInputBuffer, float **ppfOutPutBuffer, int iNumberOfFrames) {
//    for (int i = 0; i < m_iNumChannels; i++) {
//        for (int j = 0; j < m_fDelayInSamples; j++) {
//            pCRingBuff[i]->putPostInc(0.F * j);
//        }
//    }
//    for (int i = 0; i < m_iNumChannels; i++)
//        pCRingBuff[i]->setReadIdx(0);
//
//    for (int i = 0; i < m_iNumChannels; i++) {
//        for (int j = 0; j < iNumberOfFrames; j++) {
//            ppfOutPutBuffer[i][j] = ppfInputBuffer[i][j] + m_gain * pCRingBuff[i]->getPostInc();
//            pCRingBuff[i]->putPostInc(ppfOutPutBuffer[i][j]);
//        }
//    }
    return Error_t::kUnknownError;
}