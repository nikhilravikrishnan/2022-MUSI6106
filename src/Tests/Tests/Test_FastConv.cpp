#include "MUSI6106Config.h"

#ifdef WITH_TESTS
#include "Synthesis.h"
#include <stdio.h>
#include <stdlib.h>
#include "Vector.h"
#include "FastConv.h"

#include "gtest/gtest.h"


namespace fastconv_test {
    void CHECK_ARRAY_CLOSE(float* buffer1, float* buffer2, int iLength, float fTolerance)
    {
        for (int i = 0; i < iLength; i++)
        {
            EXPECT_NEAR(buffer1[i], buffer2[i], fTolerance);
        }
    }

    class FastConv: public testing::Test
    {
    protected:
        void SetUp() override
        {
            m_pCFastConv = new CFastConv;
        }

        virtual void TearDown()
        {
            m_pCFastConv->reset();
            m_pCFastConv = 0;
        }

        CFastConv *m_pCFastConv = 0;
    };

    TEST_F(FastConv, IdentityTest)
    {
        int IRLength = 51;
        float* ImpulseResponse = new float[IRLength];
        float *InputSignal = new float[10];
        float *outputBuffer = new float[10];

        //      Generating Impulse Response
        for (int i = 0; i < IRLength; i++)
        {
            ImpulseResponse[i]  = i==2 ? ImpulseResponse[i] = 1 : ImpulseResponse[i] = 0;
        }

        //      Generating Input Signal

        for (int i = 0; i < 10; i++)
        {
            InputSignal[i] = rand() % 10;
        }

        CFastConv fastConv;
        fastConv.init(ImpulseResponse, IRLength, 10);
        fastConv.process(outputBuffer, InputSignal, 10);

        CHECK_ARRAY_CLOSE(outputBuffer + 3, InputSignal, 7, 1e-3);
    }
}

#endif //WITH_TESTS

