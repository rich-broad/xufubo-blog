/*********************************************************************************
 FileName: CmdProcess.cpp
 Author: xufubo
 Date:  2018-12-26
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#include "CmdProcess.h"
using namespace std;
using namespace tars;
using namespace HardwareApplet;

int32_t CmdProcess::excute(AppletContextPtr ctx)
{
    int ret = 0;
    AppletCommHead commHead;
    vector<char> reqData;
    ProtocolHandlerPtr protocol_handler = new ProtocolHandler(ctx);
    ret = protocol_handler->CreateAppletCommHead(commHead);
    
    ret = protocol_handler->Json2Tars(reqData);
    DEBUGLOG("excute enter: reqData.size = " << reqData.size() << endl);
    //用于异步回调的callback，注意第二个参数哦，因为客户端请求会引发多个并行异步调用
    CmdProcessCallbackPtr cbPtr = new CmdProcessCallback(ctx);
    map<string, string> tarsContext;
    map<string, string> tarsStatus;
    string sFuncName = ctx->_funcName;
    try
    {
        //发起异步调用
        tars::TarsOutputStream<tars::BufferWriter> os;
        os.write(commHead, 1);
        os.write(reqData, 2);
        DEF_CFG_SINGLETON->_mpCmdServant[sFuncName]->tars_invoke_async(tars::TARSNORMAL, sFuncName, os.getByteBuffer(), tarsContext, tarsStatus, cbPtr);
        ret = 0;
    }
    __COMMON_EXCEPTION_CATCH_EXT__("cmd excute exception|" << sFuncName << "|" << REQ_HEAD_ALL_INFO(ctx->_reqHead));
    ctx->_tarsRet = ret;

    return ret;
}

int32_t CmdProcessCallback::onDispatch(ReqMessagePtr msg)
{
    if (msg->response.iRet != tars::TARSSERVERSUCCESS)
    {
        callback_excuteCmd_exception(msg->response.iRet, msg->request.sFuncName);
        return msg->response.iRet;
    }

    tars::TarsInputStream<tars::BufferReader> is;
    is.setBuffer(msg->response.sBuffer);
    int32_t ret = 0;
    vector<char> cmdRsp;
    is.read(ret, 0, true);
    is.read(cmdRsp, 3, true);
    CallbackThreadData* pCbtd = CallbackThreadData::getData();
    assert(pCbtd != NULL);
    pCbtd->setResponseContext(msg->response.context);
    callback_excuteCmd(ret, msg->request.sFuncName, cmdRsp);
    pCbtd->delResponseContext();

    return tars::TARSSERVERSUCCESS;
}

void CmdProcessCallback::callback_excuteCmd(int ret, const string& funcName, const vector<char>& cmdRsp)
{
    DEBUGLOG(ret << "|" << funcName << "|" << cmdRsp.size() << endl);
    _ctx->_rpcRet = ret;
    if (!cmdRsp.empty())
    {
        try
        {
            int retBusi = 0;
            tars::TarsInputStream<tars::BufferReader> is;
            is.setBuffer(cmdRsp);
            is.read(retBusi, 0, true);
            _ctx->_busiRet = retBusi;
        }
        __COMMON_EXCEPTION_CATCH_EXT__("read busiRet exception|" << funcName << "|" << cmdRsp.size());
    }

    _ctx->_vtRsp = cmdRsp;
    _ctx->_rspHead.ret = E_SUCCESS;
    _ctx->respond();
    _ctx->logResponse();
    tars::StatReport::StatResult statResult = (_ctx->_rspHead.ret == E_SUCCESS) ? tars::StatReport::STAT_SUCC : tars::StatReport::STAT_EXCE;
    _ctx->reportMonitorMsg(_ctx->_funcName, statResult, _ctx->_rspHead.ret);
}

void CmdProcessCallback::callback_excuteCmd_exception(int ret, const string& funcName)
{
    _ctx->_tarsRet = ret;
    _ctx->_rspHead.ret = E_BUSINESS_EXCEPTION;
    _ctx->respond();
    _ctx->logResponse();
    tars::StatReport::StatResult statResult = (ret == tars::TARSSERVERQUEUETIMEOUT || ret == tars::TARSASYNCCALLTIMEOUT) ? tars::StatReport::STAT_TIMEOUT : tars::StatReport::STAT_EXCE;
    _ctx->reportMonitorMsg(funcName, statResult, _ctx->_rspHead.ret);
}