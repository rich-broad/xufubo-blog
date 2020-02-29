
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

    // ����bitmap�е�ĳһλ��ֵ
    bitmap &setbit(size_type pos);

    // ʹbitmap��ÿһλ��Ϊ0
    bitmap &resetbit(size_type pos);

    // ������Ӧλ��1����0
    bool testbit(size_type pos) const;

    // ��ȡλͼ������1��id
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
    size_type _bit_nums;   //bitmap��λ�ĸ��� 
    size_type _size;       //word_type�ĸ���
    word_type *_p_bits;    //�洢bitλ
    word_type _mask;       //����
}; // end of bitmap
#endif
