
#include "FastConv.h"

CFastConv::CFastConv( void )
{
}

CFastConv::~CFastConv( void )
{
    reset();
}

Error_t CFastConv::init(float *pfImpulseResponse, int iLengthOfIr, int iBlockLength /*= 8192*/, ConvCompMode_t eCompMode /*= kFreqDomain*/)
{

    m_ImpulseResponse = pfImpulseResponse;
    m_blockLength = iBlockLength;
    m_IRlen = iLengthOfIr;
    m_pcRingBuff = new CRingBuffer<float>(iLengthOfIr);

    return Error_t::kNoError;
}

Error_t CFastConv::reset()
{
    m_ImpulseResponse = 0;
//    delete [] m_pcRingBuff;
    m_ImpulseResponse = 0;
    m_pcRingBuff = 0;

    return Error_t::kNoError;
}

Error_t CFastConv::process (float* pfOutputBuffer, const float *pfInputBuffer, int iLengthOfBuffers )
{

    std::memset(pfOutputBuffer, 0, iLengthOfBuffers);
    for (int i = 0; i < iLengthOfBuffers; i ++)
    {
        m_pcRingBuff->setReadIdx(m_pcRingBuff->getWriteIdx() + 1);
        m_pcRingBuff->putPostInc(pfInputBuffer[i]);

        for (int j = m_IRlen; j >0; j--)
        {
            pfOutputBuffer[i] += m_ImpulseResponse[j] * m_pcRingBuff->getPostInc();
        }

    }
    return Error_t::kNoError;
}

Error_t CFastConv::flushBuffer(float* pfOutputBuffer)
{
    return Error_t::kNoError;
}
