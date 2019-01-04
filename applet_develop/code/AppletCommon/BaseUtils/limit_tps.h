/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 限速模块
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef  __LIMIT_TPS_H_
#define  __LIMIT_TPS_H_
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "tool_utils.h"

const unsigned DEFAULT_TPS = 100;
const unsigned MAX_ACCURACY = 1000;

bool g_isApproach( unsigned a, unsigned b, unsigned uiRatio = 10 );
namespace base_utils
{
class LimitTPS
{
public:
    LimitTPS( const unsigned uiTps, const unsigned uiPrintInterval = 1, const unsigned uiAccuracy = 0 );

    void Init();
    
    void AutoAdjust();

    void Delay();

private:
    //全局控制
    double m_dTotalCount;
    double m_dTotalSec;

    //每秒内累计
    unsigned m_uiSecCount;
    unsigned m_uiSecTimes;
    unsigned m_uiPartCount;

    //系统参数
    unsigned m_uiTps;
    unsigned m_uiAccuracy;
    unsigned m_uiParticleCount;
    unsigned m_uiParticleUMS;
    unsigned m_uiParticleCountTmp;

    struct timeval m_tBegin;
    struct timeval m_tLastSec;
    struct timeval m_tNow;

    unsigned m_uiPrintInterval;
    unsigned m_uiSleepCount;
    unsigned m_uiSleepTime;
    bool m_bNextSec;

    unsigned m_uiMaxSpeed;
    unsigned m_uiMinSpeed;
    unsigned m_uiAutoAdjustInterval;
    unsigned m_uiAutoAdjustPacks;

};

};
#endif

