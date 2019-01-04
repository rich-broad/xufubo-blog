/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: ³£ÓÃ×Ö·û´®²Ù×÷
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#include "string.h"

namespace base_utils
{

std::string& StringReplace(std::string& sData, const std::string& sSrc, const std::string& sDst)
{
    std::string::size_type pos = 0;
    std::string::size_type slen = sSrc.size();
    std::string::size_type dlen = sDst.size();
    while ((pos=sData.find(sSrc, pos)) != std::string::npos)
    {
        sData.replace(pos, slen, sDst);
        pos += dlen;
    }

    return sData;
}

std::vector<std::string>& StringSplit(const std::string& sData, const std::string& sDelim, std::vector<std::string>& vItems)
{
    vItems.clear();

    std::string::size_type bpos = 0;
    std::string::size_type epos = 0;
    std::string::size_type nlen = sDelim.size();
    while ((epos=sData.find(sDelim, epos)) != std::string::npos)
    {
        vItems.push_back(sData.substr(bpos, epos-bpos));
        epos += nlen;
        bpos = epos;
    }

    vItems.push_back(sData.substr(bpos, sData.size()-bpos));

    return vItems;
}

std::vector<std::string>& StringSplitTrim(const std::string& sData, const std::string& sDelim, std::vector<std::string>& vItems)
{
    vItems.clear();

    std::string::size_type bpos = 0;
    std::string::size_type epos = 0;
    std::string::size_type nlen = sDelim.size();

    while(sData.substr(epos,nlen) == sDelim)
    {
        epos += nlen;
    }

    bpos = epos;

    while ((epos=sData.find(sDelim, epos)) != std::string::npos)
    {
        vItems.push_back(sData.substr(bpos, epos-bpos));

        epos += nlen;

        while(sData.substr(epos,nlen) == sDelim)
        {
            epos += nlen;
        }

        bpos = epos;
    }

    if(bpos != sData.size())
    {
        vItems.push_back(sData.substr(bpos, sData.size()-bpos));
    }

    return vItems;
}

std::set<std::string>& StringSplitTrim(const std::string& sData, const std::string& sDelim, std::set<std::string>& vItems)
{
    vItems.clear();

    std::string::size_type bpos = 0;
    std::string::size_type epos = 0;
    std::string::size_type nlen = sDelim.size();

    while(sData.substr(epos,nlen) == sDelim)
    {
        epos += nlen;
    }

    bpos = epos;

    while ((epos=sData.find(sDelim, epos)) != std::string::npos)
    {
        vItems.insert(sData.substr(bpos, epos-bpos));

        epos += nlen;

        while(sData.substr(epos,nlen) == sDelim)
        {
            epos += nlen;
        }

        bpos = epos;
    }

    if(bpos != sData.size())
    {
        vItems.insert(sData.substr(bpos, sData.size()-bpos));
    }

    return vItems;
}

std::string StringTrim(std::string& sData, const std::string& sDelim)
{
    // trim right
    sData.erase(sData.find_last_not_of(sDelim)+1);
    // trim left
    return sData.erase(0,sData.find_first_not_of(sDelim));
}

std::string StringJoin(const std::vector<std::string>& vstElem,
        const std::string& sDelim)
{
    std::string sText;
    std::vector<std::string>::const_iterator bpos = vstElem.begin();
    std::vector<std::string>::const_iterator epos = vstElem.end();
    std::vector<std::string>::const_iterator cpos = bpos;
    while (cpos != epos)
    {
        if (cpos != bpos)
        {
            sText += sDelim;
        }
        sText += *cpos++;
    }
    return sText;
}

std::string StringHumanJoin(const std::vector<std::string>& vstElem,
        const std::string& lastDelim, const std::string& listDelim)
{
    size_t size = vstElem.size();
    if(size<3)
    {
        return StringJoin(vstElem, lastDelim);
    }
    std::string sText(vstElem.front());
    --size;
    for(size_t i=1; i<size; ++i)
    {
        sText.append(listDelim);
        sText.append(vstElem.at(i));
    }
    sText.append(lastDelim);
    sText.append(vstElem.back());
    return sText;
}

std::string& StringChnCut(std::string& sData, int nSize)
{
    if (nSize >= (int)sData.size())
    {
        return sData;
    }
    sData.erase(CheckChn(sData.data(), nSize), std::string::npos);
    return sData;
}

int CheckChn(const char* pcData, int nSize)
{
    const char* pcEnd = pcData+nSize;
    while (pcData < pcEnd)
    {
        if (*pcData >= 0)
        {
            ++pcData;
        }
        else
        {
            pcData += 2;
        }
    }
    return (pcData==pcEnd) ? (nSize) : (nSize-1);
}

int Wordlen(const std::string& sData)
{
    int nlen = 0;
    const char* bp = sData.data();
    const char* ep = bp+sData.size();
    while (bp < ep)
    {
        nlen++;
        if (*bp>=0)
        {
            ++bp;
        }
        else
        {
            bp += 2;
        }
    }
    return nlen;
}

std::string Wordreserve(const std::string& sData, int nSize)
{
    const char* bp = sData.data();
    const char* ep = bp+sData.size();
    while (nSize>0 && bp<ep)
    {
        nSize--;
        if (*bp>=0)
        {
            ++bp;
        }
        else
        {
            bp += 2;
        }
    }
    if (bp==ep)
    {
        return sData;
    }
    return sData.substr(0, bp-sData.data());
}

std::string HtmlEncode(string &sString)
{
    int length = sString.length();
    char *buf = new char[length * 10 + 128];

    char* it = (char*) sString.c_str();
    char *out = buf;

    for (int i = 0; i < length; i++)
    {
        switch (*it)
        {
            case '<':
                memcpy(out, "&lt;", strlen("&lt;"));
                out += strlen("&lt;");
                break;

            case '>':
                memcpy(out, "&gt;", strlen("&gt;"));
                out += strlen("&gt;");
                break;


            case '&':
                memcpy(out, "&amp;", strlen("&amp;"));
                out += strlen("&amp;");
                break;

                case '\"':
                    memcpy(out, "&quot;", strlen("&quot;"));
                out += strlen("&quot;");
                break;

            case '\'':
                memcpy(out, "&#39;", strlen("&#39;"));
                out += strlen("&#39;");
                break;

            default:
                *out = *it;
                out++;
        }
        it++;
    }
    *out = '\0';

    sString = buf;
    delete []buf;
    return sString;
}

void HtmlSpecialchar(const string & sSrc, string &sResult)
{
    sResult = "";
    sResult.reserve(1024);
    char szTmp[4] = {0};
    for (unsigned int i = 0; i < sSrc.size(); i++)
    {
        if (isalnum(sSrc.at(i)) || (sSrc.at(i) == '-')
                ||(sSrc.at(i) == '_') || (sSrc.at(i) == '.'))
        {
            sResult.push_back(sSrc.at(i));
        }
        else
        {
            if (sSrc.at(i) == ' ')
            {
                sResult.push_back('+');
            }
            else
            {
                memset(szTmp, 0, sizeof(szTmp));
                snprintf(szTmp, sizeof(szTmp), "%%%02x", sSrc.at(i));
                sResult.append(szTmp);
            }

        }
    }
}

}
