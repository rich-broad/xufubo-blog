
#ifndef _BITMAP_H
#define _BITMAP_H
#include <stdlib.h>

#include <limits>
#include <cstring>
#include <iostream>
#include <vector>

class bitmap
{
    friend std::ostream & operator << (std::ostream & out, const bitmap & elem);
public: 
    typedef unsigned long word_type;
    typedef unsigned long size_type;

public:
    bitmap(size_type bit_num);
    bitmap(const bitmap &val);
    bitmap &operator=(const bitmap &val);
    ~bitmap();

    // 设置bitmap中的某一位的值
    bitmap &setbit(size_type pos);

    // 使bitmap中每一位都为0
    bitmap &resetbit(size_type pos);

    // 测试相应位是1还是0
    bool testbit(size_type pos) const;

    // 获取位图中所有1的id
    void traversal(std::vector<size_type> & res);

    void clear()
    {
        memset(_p_bits,0,_size * WORD_BYTES);
    }
private:
    enum 
    {
        WORD_BYTES = sizeof(word_type)
    };
    enum 
    {
        WORD_BITS = std::numeric_limits<word_type>::digits
    };
private:
    size_type _bit_nums;   //bitmap中位的个数 
    size_type _size;       //word_type的个数
    word_type *_p_bits;    //存储bit位
    word_type _mask;       //掩码
}; // end of bitmap
#endif
