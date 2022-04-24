
#include "FastConv.h"

CFastConv::CFastConv( void )
{
}

CFastConv::~CFastConv( void )
{
    reset();
}

Error_t CFastConv::init(float *pfImpulseResponse, int iLengthOfIr, int iBlockLength)
{

    m_ImpulseResponse = pfImpulseResponse;
    m_blockLength = iBlockLength;
    m_IRlen = iLengthOfIr;
    m_pcRingBuff = new CRingBuffer<float>(iLengthOfIr);
    CFft::createInstance(fft);
    fft->initInstance(iBlockLength);

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

        for (int j = m_IRlen - 1; j >= 0; j--)
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

void CFastConv::processFrequencyDomain(float *pfOutputBuffer, const float* pfInputBuffer, int iLengthofBuffers)
{
    long seqLen = sizeof(*pfInputBuffer)/sizeof (pfInputBuffer[0]) + m_blockLength - 1;
    long numIRBlocks = floor(m_IRlen/m_blockLength);

    pfOutputBuffer = new float[seqLen];

    float **ImpulseResponseBlocks = nullptr;
    CFft::complex_t **ImpulseResponseBlocksSpectrum = nullptr;

    long realStartIdx = 0;
    long imagStartIdx = m_blockLength;

//    To account for zero-padding
    float *realInputBlock = new float[m_blockLength];
    float *imagInputBlock = new float[m_blockLength];
//    std::complex<float> *inputToFFT = new std::complex<float>[2*m_blockLength];
    float *inputToFFT = new float[2*m_blockLength];
    CFft::complex_t *outputFFT = new float[2*m_blockLength];
    float* convFFT = new float[2*m_blockLength];


    std::memset(realInputBlock, 0, sizeof(float)*m_blockLength);
//    std::memset(imagInputBlock, 0, sizeof(float)*m_blockLength);
    std::memset(inputToFFT, 0, sizeof(float)*m_blockLength*2);


    for (int i = 0; i < numIRBlocks; i++)
    {
        *ImpulseResponseBlocks = new float [2*m_blockLength];
        std::memset (ImpulseResponseBlocks[i], 0, sizeof(float)*m_blockLength*2);
    }

//    Generate blocks for Impulse Response
    for (int i = 0; i < numIRBlocks; i++)
    {
        for (int j = 0; j < m_blockLength; j++)
        {
            ImpulseResponseBlocks[i][j] = m_ImpulseResponse[i*m_blockLength + j];
        }
        fft->doFft(ImpulseResponseBlocksSpectrum[i], ImpulseResponseBlocks[i]);
    }



//    Main Loop
    while (realStartIdx < seqLen)
    {
        std::memcpy(realInputBlock, pfInputBuffer + realStartIdx, (m_blockLength)*sizeof(float));
//        std::memcpy(imagInputBlock, pfInputBuffer + imagStartIdx, (m_blockLength)*sizeof(float));

//     Generate blocks for FFT computation
//        for (long i = 0; i < m_blockLength; i++)
//        {
//            inputToFFT[i] = realInputBlock[i];
//        }
        std::memcpy(inputToFFT, realInputBlock, (m_blockLength)*sizeof (float));
        fft->doFft(outputFFT, inputToFFT);

//        Computing convolution
        for (int i = 0; i < numIRBlocks; i++)
        {
            for (int j = 0; j < m_blockLength*2; j++)
            {
                convFFT[j] = outputFFT[j]*ImpulseResponseBlocksSpectrum[i][j];
            }

        }

    }
}
