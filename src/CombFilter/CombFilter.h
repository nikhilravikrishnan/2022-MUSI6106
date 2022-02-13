#include "CombFilterIf.h"
#include "CombFilter.cpp"

class CCombFilterBase : public CCombFilterIf()
{
    public:

        Error_t initializeParams();

        Error_t getParam();



    protected:
        CCombFilterBase();
        virtual ~CCombFilterBase();

    private:

    float m_fMaxDelayInSamples;
    
           
};

class CCombFilterIIR : public CCombFilterBase ()
{

};

class CCombFilterFIR : public CCombFilterBase()
{

};



