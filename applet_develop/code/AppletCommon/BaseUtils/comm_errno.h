/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 通用错误码
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef __BITSTRING_H__
#define __BITSTRING_H__

namespace base_utils
{
enum CommonErrNo
{
    //-------文件--------------
    ERR_OPEN_FILE                 = -100101,  // 打开文件失败
    ERR_WRITE_FILE                = -100102,  // 写文件失败
    ERR_READ_FILE                 = -100103,  // 读文件失败
    ERR_CONFIG_INIT               = -100104,  // config init失败
    ERR_OPEN_DIR                  = -100105,  // open dir失败
    ERR_OPEN_SERVICE_CONF         = -100106,  // 打开service配置文件失败
    ERR_STAT_FILE                 = -100107,  // stat file失败

    //-------进程--------------
    ERR_FORK                         = -100201,    //fork进程失败
    ERR_EXECL                       = -100202,    //execl进程失败
    ERR_WAIT_CHILD_PROCESS        = -100203,    //wait接口失败
    ERR_CHILD_PROCESS_EXCEPTION   = -100204,    //子进程退出异常

    //-------管道-------------
    ERR_PIPE                      = -100301,  // 管道错误
    ERR_READ_PIPE                 = -100302,  // 读取管道错误

    //-------DOWUP-------------
    ERR_DOWUP_SOCKET              = -100401,     //打开socket失败
    ERR_DOWUP_SEND                = -100402,     //发送请求失败
    ERR_DOWUP_RECV                = -100403,     //接收应答失败
    ERR_DOWUP_CONNECT             = -100404,     //连接失败
    ERR_DOWUP_PACKET_INVALID      = -100405,     //检测报文失败
    ERR_DOWUP_BUFFER_SIZE         = -100406,     //接收空间不够
    ERR_DOWUP_CONN_CLOSED         = -100407,     //后端主动关闭连接

    //-------json请求参数错误-----
    ECODE_INVALID_JSON_REQ_DATA        = 120025,      //json请求参数错误

    //-----------------------------------逻辑错误-------------------------------------
    ERR_URL_CHECK_PACK     =  200001,   // url check pack
    ERR_URL_CHECK_UNPACK   =  200002,   // url check unpack
    ERR_URL_CHECK_API      =  -200001,  // url check api
    ERR_DATE_TIME_FORMAT   =  200003,   // 时间格式不对
    ERR_PACKET_TOO_LARGE   =  -200004,  // 包太大
    ECODE_CMD_NOT_REGISTER = 200005,    // cmd未注册
    ERR_PACKET_TOO_SMALL   =  -200005,  // 包太小
    ERR_INVALID_CMD        = 200006,    // 非法cmd

    ERR_SC_REQ_PACKAGE_INVALID  = 200016,       //请求包异常
    ERR_SC_REQ_PACKAGE_TOO_LONG  = 200018,       //请求内容过长
    ERR_SC_REQ_PACKAGE_TYPE_WRONG  = 200019,       //请求账号类型错误
    ERR_SC_REQ_PACKAGE_BODY_WRONG  = 200020,       //请求包体错误
    ERR_SC_RSP_PACKAGE_INVALID  = 200017,      //回复包异常
    ERR_SC_RSP_PACKAGE_TOO_SHORT  = 200021,      //回复包过短
    ERR_SC_RSP_PACKAGE_HEADER_WORONG  = 200022,      //回复头错误
    ERR_SC_RSP_PACKAGE_DATA_WORONG  = 200023,      //回复数据字段
    ERR_SC_RSP_PACKAGE_BODY_WORONG  = 200024,      //回复包体

    //-----------------------------------系统错误-------------------------------------
    //内存错误
    ERR_MEM_MALLOC          = -200100,  //内存分配失败
    ERR_MEM_NEW          = -200101,  //内存分配失败

    //参数相关
    ERR_PARAM_EMPTY         = -200200,  //参数内容为空
    ERR_PARAM_INTER            = -200201,  //内部调用参数失败
    ERR_PARAM_CGI            = -200202,  //CGI参数较验失败
    ERR_PARAM_NULL            = -200203,  //NULL指针
    ERR_PARAM_ILLEGAL       = -200204,  //参数错误
    ERR_PACKET_FORMAT       = -200205,     // 参数无效一般返回码
    ERR_PARAM_SECTION_ID_NULL  = -200206,  //在话别列表里找不到section id

    ECODE_UIN_PARAM                 = 200200,     // cgi中uin参数错误
    ECODE_URL_PARAM                 = 200201,     // cgi中URL参数错误
    ERR_PARAM_INVALID               = 200202,     // 参数无效一般返回码

    //编解码
    ERR_TARS_DECODE_FAIL        = -200300,  //TARS解码失败
    ERR_TARS_ENCODE_FAIL        = -200301,  //TARS编码失败

    ECODE_JSON_PARSE                = -200327,     // JSON PARSE
    ECODE_JSON_EXCEPTION            = -200328,     // JSON 异常
    ECODE_JSON_IS_NOT_MEMBER        = -200335,     // 不是member成员
    ECODE_JSON_IS_NOT_ARRAY         = -200336,     // 不是array对象
    ECODE_JSON_IS_NOT_STRING        = -200337,     // 不是string
    ECODE_PARSE_JSON_ERROR          = -200338,     // 文件内容不是合法json
    ECODE_JSON_TABLE_NOT_FOUND      = -200339,     // json表不存在
    ECODE_BASE64_ENCODE          = -200340,     // base64编码错误


    
};
};
#endif


