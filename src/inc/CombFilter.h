#include "CombFilterIf.h"
#include "RingBuffer.h"
#include "ErrorDef.h"
#include <vector>

class CCombFilterBase : public CCombFilterIf
{
    public:
        CCombFilterBase();
        virtual ~CCombFilterBase();
        Error_t init(int iNumChannels);
        Error_t initBuffer(CRingBuffer<float> **buffer);
        Error_t processBuffer(float **ppfInputBuffer, float **ppfOutPutBuffer, int iNumberOfFrames);
        Error_t initDefaults();
        Error_t setParameters(FilterParam_t eParam, float fParamValue);
        float getParameter(FilterParam_t eParam);
        Error_t setFilterType(CombFilterType_t filterType);


    private:
        int m_fDelayInSamples;
        float m_gain;
        float m_fMaxDelayLengthInS;
//        float m_fSampleRate;
        int m_iNumChannels;

        CombFilterType_t m_eFilterType;

        CRingBuffer<float> **pCRingBuff;

           
};

class CCombFilterFIR : public CCombFilterBase
        {
    public:
            CCombFilterFIR();
            virtual ~CCombFilterFIR();
            virtual  Error_t processBuffer(float **ppfInputBuffer, float **ppfOutPutBuffer, int iNumberOfFrames) = 0;
};

class CCombFilterIIR : public CCombFilterBase
{
    public:
            CCombFilterIIR();
            virtual  ~CCombFilterIIR();
            virtual Error_t processBuffer(float **ppfInputBuffer, float **ppfOutPutBuffer, int iNumberOfFrames) = 0 ;

};




