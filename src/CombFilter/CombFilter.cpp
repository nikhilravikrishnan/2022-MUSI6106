# include "CombFilter.h"
# include "CombFilterIf.h"
# include "RingBuffer.h"

CCombFilterBase::CCombFilterBase()
{
    this->initDefaults();
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
    m_fSampleRateInHz = 16000;
    m_gain = 0.5;
    m_iNumChannels = 2;

    CRingBuffer<float> **pCRingBuff = new CRingBuffer<float>* [m_iNumChannels];
    for (int i = 0; i < m_iNumChannels; i++)
        pCRingBuff[i] = new CRingBuffer<float>(m_fDelayInSamples);

}

Error_t CCombFilterBase::init(CombFilterType_t eFilterType, int iNumChannels)
{
    m_eFilterType = eFilterType;
    m_iNumChannels = iNumChannels;
}


Error_t CCombFilterBase::processBuffer(float **ppfInputBuffer, float **ppfOutPutBuffer, int iNumberOfFrames)
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
        for (int j = 0; j < iNumberOfFrames; j++)
        {
            ppfOutPutBuffer[i][j] = ppfInputBuffer[i][j] + m_gain*pCRingBuff[i]->getPostInc();

            if (this->m_eFilterType == 0)
                pCRingBuff[i]->putPostInc(ppfInputBuffer[i][j]);
            else if(this->m_eFilterType == 1)
                pCRingBuff[i]->putPostInc(ppfInputBuffer[i][j]);
            else
                return Error_t::kFunctionInvalidArgsError;


        }
    }
}

Error_t CCombFilterBase::setParameters(CCombFilterIf::FilterParam_t eParam, float fParamValue) {

    if (eParam == kParamGain)
        m_gain = fParamValue;
    else if (eParam == kParamDelay)
        m_fDelayInSamples = fParamValue;
    else
        return Error_t::kInvalidString;
}

float CCombFilterBase::getParameter(CCombFilterIf::FilterParam_t eParam) {
    if (eParam == kParamDelay)
        return m_fDelayInSamples;
    else if (eParam == kParamGain)
        return m_gain;
}




