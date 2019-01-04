/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 常用字符串操作
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#ifndef __STRING_H__
#define __STRING_H__ \

#include <string>
#include <vector>
#include <set>
#include "base_define.h"

namespace base_utils
{

using namespace std;

/*!
 * \brief 字符串替换
 * \param[in,out] sData 要进行替换的对象并对之进行替换
 * \param[in] sSrc 要替换的子串
 * \param[in] sDst 用于替换的子串
 * \return std::string 返回sData对象引用
 */
std::string& StringReplace(std::string& sData, const std::string& sSrc, const std::string& sDst);

/*!
 * \brief 分解字符串
 * \param[in] sData 要进行分解的字符串
 * \param[in] sDelim 分隔字符串
 * \param[out] vItems 返回字符串列表包含空串
 * \return std::vector<std::string> vItems的引用
 */
std::vector<std::string>& StringSplit(const std::string& sData, const std::string& sDelim, std::vector<std::string>& vItems);

/*!
 * \brief 分解字符串
 * \param[in] sData 要进行分解的字符串
 * \param[in] sDelim 分隔字符串
 * \param[out] vItems 返回字符串列表包含空串
 * \return std::set<std::string> vItems的引用
 */
std::set<std::string>& StringSplitTrim(const std::string& sData, const std::string& sDelim, std::set<std::string>& vItems);

/*!
 * \brief 分解字符串(连续的分割符看作一个如 |||==|)
 * \param[in] sData 要进行分解的字符串
 * \param[in] sDelim 分隔字符串
 * \param[out] vItems 返回字符串列表包含空串
 * \return std::vector<std::string> vItems的引用
 */
std::vector<std::string>& StringSplitTrim(const std::string& sData, const std::string& sDelim, std::vector<std::string>& vItems);

/*!
 * \brief 去除字符串前后的空格
 * \param[in,out] sData 要进行Trim的字符串
 * \param[in] sDelim 分隔字符串,默认为空格
 * \return std::string sData
 */
std::string StringTrim(std::string& sData, const std::string& sDelim =" ");

/*!
 * \brief 把数组合成字符串
 * \param[in] vstElem 要合成的数组
 * \param[in] sDelim  分隔符
 * \return std::string 合成的字符串
 */
std::string StringJoin(const std::vector<std::string>& vstElem, const std::string& sDelim);

/*!
 * \brief 把数组合成字符串
 * a
 * a和b
 * a,b和c
 *
 * \param[in] vstElem 要合成的数组
 * \param[in] lastDelim  后分隔符
 * \param[in] listDelim  前分隔符
 * \return std::string 合成的字符串
 */
std::string StringHumanJoin(const std::vector<std::string>& vstElem, const std::string& lastDelim=" and ", const std::string& listDelim=", ");

/*!
 * \brief 把数组合成字符串
 * \param[in] vstElem 要合成的数组
 * \param[in] sDelim  分隔符
 * \return std::string 合成的字符串
 */
std::string StringJoin(const std::vector<int>& vstElem, const std::string& sDelim);


/*! \brief 检查进行中文完整截断保存最多nlen字节 */
std::string& StringChnCut(std::string& sData, int nSize);

/*!
 * \brief 检查双字节字符的截断位置,保证数据的正确性
 * \param[in] pcData 字符指针首地址
 * \param[in] nSize  最大字符数(注:应保证不能大于字符串长度)
 * \return int <= nSize 应该保留的字节数 [ =nSize OR =nSize-1 ]
 */
int CheckChn(const char* pcData, int nSize);


/*!
 * \brief 转义html敏感字符
 * \param[in] sString 被转义字符地址
 * \return 转义后字符
 */
std::string HtmlEncode(string &sString);

/*!
 * \brief 转义html敏感字符  按字节
 * \param[in] sSrc 被转义字符地址
 * \param[out] sResult 转义后字符
 * \return 无
 */
void HtmlSpecialchar(const string & sSrc, string &sResult);


/*!
 * \brief 检查词数(中文词=2字节)
 * \param[in] sData 要检查的字符串
 * \return int 词数
 */
int Wordlen(const std::string& sData);

/*!
 * \param[in] sData 源字符串
 * \param[in] nSize 字符串长度
 * \return std::string 返回目的串
 */
std::string Wordreserve(const std::string& sData, int nSize);

}

#endif
