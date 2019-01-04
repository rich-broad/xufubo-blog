/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 限速模块
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/


#include "limit_tps.h"


const unsigned DEFAULT_TPS = 100;
const unsigned MAX_ACCURACY = 1000;

bool g_isApproach( unsigned a, unsigned b, unsigned uiRatio = 10 );
namespace base_utils
{
    LimitTPS::LimitTPS( const unsigned uiTps, const unsigned uiPrintInterval = 1, const unsigned uiAccuracy = 0 )
    {
        m_uiTps = uiTps;
        m_uiAccuracy = uiAccuracy;
        m_uiPrintInterval = uiPrintInterval;

        m_dTotalCount = 0;
        m_dTotalSec = 0;
        m_uiSecCount = 0;
        m_uiPartCount = 0;
        m_uiSecTimes = 0;
        m_uiSleepCount = 0;
        m_uiSleepTime = 0;
        m_bNextSec = false;
        m_uiMaxSpeed = 0;
        m_uiMinSpeed = 0;
        m_uiAutoAdjustInterval = 5;
        m_uiAutoAdjustPacks = 0;
        memset(&m_tNow, 0, sizeof(0));

        Init();
        gettimeofday( &m_tLastSec, NULL );
        m_tBegin = m_tLastSec;
    }

    void LimitTPS::Init()
    {
        if ( m_uiTps == 0 )
        {
            m_uiTps = DEFAULT_TPS;
        }

        if( m_uiAccuracy == 0 )
        {
            m_uiAccuracy = m_uiTps/10;
        }

        if ( m_uiAccuracy > m_uiTps || m_uiAccuracy == 0 )
        {
            m_uiAccuracy = m_uiTps;
        }
        else if ( m_uiAccuracy > MAX_ACCURACY )
        {
            m_uiAccuracy = MAX_ACCURACY;
        }

        if( m_uiPrintInterval == 0 )
        {
            m_uiPrintInterval = 1;
        }

        if( m_uiTps % m_uiAccuracy != 0 )
            m_uiParticleCount = m_uiTps/m_uiAccuracy + 1;
        else
            m_uiParticleCount = m_uiTps/m_uiAccuracy;

        m_uiParticleUMS   = 1000000/m_uiAccuracy;
        m_uiParticleCountTmp = 1;

        printf( "pid=%u, m_uiTps = %u, m_uiAccuracy = %u, m_uiParticleCount = %u, m_uiParticleUMS = %u\n\n",
               getpid(), m_uiTps, m_uiAccuracy, m_uiParticleCount, m_uiParticleUMS );

    }

    void LimitTPS::AutoAdjust()
    {
        unsigned uiRealTps = (unsigned)floor( m_uiAutoAdjustPacks / m_uiAutoAdjustInterval );
        if( g_isApproach( uiRealTps, m_uiTps ) || m_uiSleepCount * 100 / m_uiAccuracy > 20 )
        {
            uiRealTps = m_uiTps;
        }

        unsigned uiParticleCountTmp = uiRealTps / m_uiAccuracy;
        unsigned uiExpectedCount = 0;

        if( uiRealTps < 1 )
        {
            uiExpectedCount = 1;
        }
        else if( uiRealTps < 100 )
        {
            uiExpectedCount = 4;
        }
        else if( uiRealTps < 1000 )
        {
            uiExpectedCount = 10;
        }
        else if( uiRealTps > 10000 )
        {
            uiExpectedCount = uiRealTps / MAX_ACCURACY;
        }
        else
        {
            uiExpectedCount = 20;
        }

        unsigned uiAccuracy = m_uiAccuracy;
        if( uiExpectedCount > 0 )
        {
            while( uiParticleCountTmp != uiExpectedCount )
            {
                uiParticleCountTmp = uiRealTps / uiAccuracy;
                if( uiParticleCountTmp > uiExpectedCount )
                {
                    uiAccuracy++;
                    if( uiAccuracy > MAX_ACCURACY )
                    {
                        uiAccuracy = MAX_ACCURACY;
                        break;
                    }
                }
                else if( uiParticleCountTmp < uiExpectedCount )
                {
                    uiAccuracy--;
                    if( uiAccuracy == 0 )
                    {
                        uiAccuracy = 1;
                        break;
                    }
                }
            }
        }

        if( !g_isApproach( m_uiAccuracy, uiAccuracy ) )
        {
            m_uiAccuracy = uiAccuracy;

            if( m_uiTps % uiAccuracy != 0 )
                m_uiParticleCount = m_uiTps/m_uiAccuracy + 1;
            else
                m_uiParticleCount = m_uiTps/m_uiAccuracy;

            m_uiParticleUMS   = 1000000/m_uiAccuracy;
            m_uiParticleCountTmp = 1;

            printf( "*****AutoAdjust: pid=%u, RealTps = %u, MaxRate = %u, Accuracy: %u, PartCount = %u, Partums = %u\n",
                   getpid(), uiRealTps, m_uiParticleCount * m_uiAccuracy, m_uiAccuracy, m_uiParticleCount, m_uiParticleUMS );
        }

    }

    void LimitTPS::Delay()
    {
        m_uiPartCount++;

        if ( m_uiPartCount >= m_uiParticleCount || m_uiPartCount >= m_uiParticleCountTmp )
        {
            gettimeofday( &m_tNow, NULL );
            unsigned uiSleep = 0;
            unsigned uiUsed = g_uiSub( (unsigned)m_tNow.tv_sec, (unsigned)m_tLastSec.tv_sec ) * 1000000
            + m_tNow.tv_usec - m_tLastSec.tv_usec;

            if( uiUsed > m_uiParticleUMS * ( m_uiSecTimes + 1 ) && m_uiParticleCountTmp > 1 )
            {
                m_uiParticleCountTmp--;
                //printf( "AutoAdjust--: m_uiParticleCount = %u, m_uiParticleCountTmp = %u\n",
                //      m_uiParticleCount, m_uiParticleCountTmp );
            }
            else if( m_uiParticleCountTmp < m_uiParticleCount )
            {
                if ( uiUsed < m_uiParticleUMS * ( m_uiSecTimes + 1 ) )
                {
                    m_uiParticleCountTmp++;
                    //printf( "AutoAdjust++: m_uiParticleCount = %u, m_uiParticleCountTmp = %u\n",
                    //      m_uiParticleCount, m_uiParticleCountTmp );
                    return;
                }
            }

            m_uiSecTimes++;
            m_uiSecCount += m_uiPartCount;
            m_uiPartCount = 0;
            m_bNextSec = false;

            //printf( "use:%u\n", uiUsed );

            if ( m_uiSecTimes >= m_uiAccuracy || uiUsed > 1000000 )
            {
                m_bNextSec = true;
                if ( uiUsed <= 1000000 )
                {
                    uiSleep = 1000000 - uiUsed;
                    m_tLastSec.tv_sec++;
                    //printf( "sleep1: %u\n", uiSleep );
                }
                else
                {
                    m_tLastSec = m_tNow;
                }
            }
            else if ( uiUsed < m_uiParticleUMS * m_uiSecTimes )
            {
                uiSleep = m_uiParticleUMS * m_uiSecTimes - uiUsed;
                //printf( "sleep2: %u\n", uiSleep );
            }

            if ( uiSleep > 0 && uiSleep < 1000000 )
            {
                //printf( "sleep3:%u\n", uiSleep );
                usleep( uiSleep );
                m_uiSleepTime += uiSleep;
                m_uiSleepCount++;
            }

            if( m_bNextSec )
            {
                if( m_uiSecCount > m_uiMaxSpeed ) m_uiMaxSpeed = m_uiSecCount;
                if( m_uiSecCount < m_uiMinSpeed || m_uiMinSpeed == 0 ) m_uiMinSpeed = m_uiSecCount;

                m_dTotalCount += m_uiSecCount;
                m_dTotalSec = g_uiSub( (unsigned)m_tNow.tv_sec, (unsigned)m_tBegin.tv_sec );
                m_uiAutoAdjustPacks += m_uiSecCount;

                if( m_dTotalSec == 0 )
                {
                    //printf( "m_tNow.tv_sec = %u, %u\n", (unsigned)m_tNow.tv_sec, (unsigned)m_tBegin.tv_sec );
                    m_dTotalSec = 1;
                }

                if( (unsigned)(floor(m_dTotalSec)) % m_uiPrintInterval == 0 )
                {
                    printf( "pid=%u, TP = %03.0lf, ET = %02.0lf(s), TPS = %0.2lf, Curr = %02u   [min = %u, max = %u] "
                           "[SleepInfo: %02u(times), %02u(ms), pc = %u/%u]\n",
                           getpid(), m_dTotalCount, m_dTotalSec, m_dTotalCount/m_dTotalSec,
                           m_uiSecCount, m_uiMinSpeed, m_uiMaxSpeed,
                           m_uiSleepCount, m_uiSleepTime/1000,
                           m_uiParticleCountTmp, m_uiParticleCount );
                }
                m_uiSecCount = 0;
                m_uiSecTimes = 0;
                m_uiSleepCount = 0;
                m_uiSleepTime = 0;
            }
        }

        return;
    }
}



