
// standard headers

// project headers
#include "MUSI6106Config.h"

#include "ErrorDef.h"
#include "Util.h"

#include "CombFilterIf.h"
#include "CombFilter.h"

static const char*  kCMyProjectBuildDate = __DATE__;


CCombFilterIf::CCombFilterIf() :
    m_bIsInitialized(false),
    m_pCCombFilter(0),
    m_fSampleRate(0)
{
    // this should never hurt
    this->reset ();
}


CCombFilterIf::~CCombFilterIf()
{
    this->reset ();
}

const int  CCombFilterIf::getVersion(const Version_t eVersionIdx)
{
    int iVersion = 0;

    switch (eVersionIdx)
    {
    case kMajor:
        iVersion    = MUSI6106_VERSION_MAJOR; 
        break;
    case kMinor:
        iVersion    = MUSI6106_VERSION_MINOR; 
        break;
    case kPatch:
        iVersion    = MUSI6106_VERSION_PATCH; 
        break;
    case kNumVersionInts:
        iVersion    = -1;
        break;
    }

    return iVersion;
}
const char*  CCombFilterIf::getBuildDate ()
{
    return kCMyProjectBuildDate;
}

Error_t CCombFilterIf::create (CCombFilterIf*& pCCombFilter)
{
    pCCombFilter = new CCombFilterBase();
}

Error_t CCombFilterIf::destroy (CCombFilterIf*& pCCombFilter)
{
    delete [] pCCombFilter;
}

Error_t CCombFilterIf::init (CombFilterType_t eFilterType, float fMaxDelayLengthInS, float fSampleRateInHz, int iNumChannels)
{
    m_pcCombFilter->init(eFilterType, fMaxDelayLengthInS, fSampleRateInHz, iNumChannels);
    
}

Error_t CCombFilterIf::reset ()
{
    return Error_t::kNoError;
}

Error_t CCombFilterIf::process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
    m_pCCombFilter->processBuffer(ppfInputBuffer, ppfOutputBuffer, iNumberOfFrames);
}

Error_t CCombFilterIf::setParam (FilterParam_t eParam, float fParamValue)
{
    eParam = fParamValue;
}

float CCombFilterIf::getParam (FilterParam_t eParam) const
{
    return eParam;
}
