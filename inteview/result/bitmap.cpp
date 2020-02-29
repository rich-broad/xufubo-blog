#include "bitmap.h"
using namespace std;
bitmap::bitmap(size_type bit_num)
{
    _bit_nums = bit_num;
    size_type free_bits = (WORD_BITS - _bit_nums % WORD_BITS) % WORD_BITS;
    _size = _bit_nums / WORD_BITS + (free_bits == 0 ? 0 : 1);
    _p_bits = new word_type[_size];

    if (_p_bits == NULL)
    {
        cout << "no enough memorry!" << endl;
        exit(0);
    }

    clear();
    _mask = ~word_type(0);
    _mask >>= free_bits;
}

inline bitmap::~bitmap()
{
    delete [] _p_bits;
}

bitmap &bitmap::setbit(size_type pos)
{
    if (pos > _bit_nums || pos <= 0)
    {
        cout << "position is not right" << endl;
        exit(0);
    }

    size_type tempNum = pos % WORD_BITS;
    size_type eleNum = tempNum ? pos / WORD_BITS : pos / WORD_BITS - 1;
    size_type bitNum = tempNum ? tempNum - 1 : WORD_BITS - 1;

    word_type mask = 1;
    mask <<= bitNum;
    _p_bits[eleNum] |= mask;

    return (*this);
}

bitmap &bitmap::resetbit(size_type pos)
{
    if (pos > _bit_nums || pos <= 0)
    {
        cout << "position is not right" << endl;
        exit(0);
    }

    size_type tempNum = pos % WORD_BITS;
    size_type eleNum = tempNum ? pos / WORD_BITS : pos / WORD_BITS - 1;
    size_type bitNum = tempNum ? tempNum - 1 : WORD_BITS - 1;

    word_type mask = 1;
    mask <<= bitNum;

    _p_bits[eleNum] &= ~mask;
    return (*this);
}

void bitmap::traversal(std::vector<size_type> & res)
{
    for (bitmap::size_type i = 1; i <= _bit_nums; ++i)
    {
        size_type tempNum = _bit_nums % WORD_BITS;
        size_type eleNum = tempNum ? _bit_nums / WORD_BITS : _bit_nums / WORD_BITS - 1;
        size_type bitNum = tempNum ? tempNum - 1 : WORD_BITS - 1;

        word_type mask = 1;
        mask <<= bitNum;

        if (_p_bits[eleNum] & mask)
        {
            res.push_back(_bit_nums);
        }
    }
}

bool bitmap::testbit(bitmap::size_type pos) const
{
    if (pos > _bit_nums || pos<= 0)
    {
        cout << "position is not right" << endl;
        exit(0);
    }

    size_type tempNum = pos % WORD_BITS;
    size_type eleNum = tempNum ? pos / WORD_BITS : pos / WORD_BITS - 1;
    size_type bitNum = tempNum ? tempNum - 1 : WORD_BITS - 1;

    word_type mask = 1;
    mask <<= bitNum;

    return _p_bits[eleNum] & mask;
}

ostream & operator << (ostream & out,const bitmap & elem)
{
    bitmap::size_type j = 0;
    bitmap::size_type mask = 1;
    mask <<= (elem._bit_nums % bitmap::WORD_BITS-1);

    for (bitmap::size_type i = elem._bit_nums; i > 0; --i)
    {
        if (i % (bitmap::WORD_BITS) == 0)
        {
            j++;
            mask = 1;
            mask <<= (bitmap::WORD_BITS-1);
        }
        out << (elem._p_bits[elem._size-j-1] & mask ? 1 : 0);
        mask >>= 1;
    }
    return out;
}
int main(int argc, char** argv)
{
    bitmap temp(50);
    cout << temp << endl;

    temp.setbit(20);
    cout << temp << endl;

    temp.resetbit(20);
    cout << temp << endl;

    temp.setbit(20);
    cout << temp << "|" << temp.testbit(20) << "|" << temp.testbit(21) << endl;

    temp.clear();
    cout << temp << endl;

    temp.setbit(21);temp.setbit(1);temp.setbit(2);
    cout << temp << "|" << temp.testbit(20) << "|" << temp.testbit(21) << endl;

    vector<bitmap::size_type> result;
    temp.traversal(result);
    cout << "[ ";
    for (bitmap::size_type i = 0; i < result.size(); ++i)
    {
        cout << result[i] << ",";
    }

    cout << " ]" << endl;
    return 0;
}