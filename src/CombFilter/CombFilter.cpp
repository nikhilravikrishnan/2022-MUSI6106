# include "CombFilter.h"
# include "CombFilterIf.h"

CCombFilterBase::CCombFilterBase() :
{
    this->initDefaults()
}

Error_t CCombFilterBase::initDefaults()
{

     m_fMaxDelayInSamples = 100;
     m_iNumChannels = 2;
}

Error_t CCombFilterBase::processBuffer(float **inputBuffer, float **outputBuffer, int numberOfFrames)
{
    
}

