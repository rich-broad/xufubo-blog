

#ifndef __HYDRA_JCE_ENDECODE_H__
#define __HYDRA_JCE_ENDECODE_H__

#include <string>
#include <iostream>
#include "tup/Tars.h"


namespace HardwareApplet 
{

template <typename TTarsStruct>
int TarsEncode(const TTarsStruct& js, std::string& output)
{
    try
    {
        tars::TarsOutputStream<tars::BufferWriter> bw;
        js.writeTo(bw);
        output.assign(bw.getBuffer(), bw.getLength());
        return 0;
    }
    catch (...)
    {
        return 1;
    }
}

template <typename TTarsStruct>
int TarsEncode(const TTarsStruct& js, std::vector<char>& output)
{
    std::string temp;
    try
    {
        tars::TarsOutputStream<tars::BufferWriter> bw;
        js.writeTo(bw);
        temp.assign(bw.getBuffer(), bw.getLength());
        output.assign(temp.begin(), temp.end());
        return 0;
    }
    catch (...)
    {
        return 1;
    }
}

template <typename TTarsStruct>
int TarsDecode(const char* inbuf, unsigned inlen, TTarsStruct& js)
{
    try
    {
        tars::TarsInputStream<tars::BufferReader> br;
        br.setBuffer((char*)inbuf, inlen);
        js.readFrom(br);
        return 0;
    }
    catch (...)
    {
        return 2;
    }
}

template <typename TTarsStruct>
int TarsDecode(const std::vector<char> &invt, TTarsStruct& js)
{
    try
    {
        tars::TarsInputStream<tars::BufferReader> br;
        br.setBuffer(invt);
        js.readFrom(br);
        return 0;
    }
    catch (...)
    {
        return 2;
    }
}

template <typename TTarsStruct>
int TarsDecode(const std::string& input, TTarsStruct& js)
{
    std::vector<char> tempvt(input.begin(), input.end());
    return TarsDecode<TTarsStruct>(tempvt, js);
}


}

#endif
