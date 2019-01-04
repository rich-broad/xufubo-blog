/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 随机数
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#include <stdlib.h>
#include <algorithm>
#include "random_utils.h"



using namespace std;

namespace base_utils
{


vector<size_t> RandomUtils::ChooseNByWeight(const std::vector<unsigned> &weight_list, size_t n)
{
    if(n > weight_list.size())
    {
        n = weight_list.size();
    }

    if(n == 0 || weight_list.empty())
    {
        return vector<size_t>();
    }

    vector<unsigned> copy_of_weight_list = weight_list;

    vector<size_t> result_list;
    for( ; result_list.size() < n; )
    {
        size_t choose_idx;
        int ret = ChooseOneByWeight(copy_of_weight_list, choose_idx);
        if(ret != 0)
        {
            // 仅有weight_list为空，或者weight_list内的权重全是０，才会出错
            break;
        }
        copy_of_weight_list[choose_idx] = 0;

        result_list.push_back(choose_idx);
    }

    return result_list;
}

int RandomUtils::ChooseOneByWeight(const std::vector<unsigned> &weight_list, size_t &choose_idx)
{

    if(weight_list.empty())
    {
        // fatal error
        // assert(-1);
    }

    // 权重累加
    vector<unsigned> acc_weight_list; 
    unsigned acc = 0;
    for(size_t i = 0; i < weight_list.size(); ++i)
    {
        acc += weight_list[i];
        acc_weight_list.push_back(acc);
    }
    
    if(acc == 0)
    {
        return -1;
    }

    //随机一个值
    unsigned roll_num = rand() % acc;

    vector<unsigned>::iterator upper = std::upper_bound(acc_weight_list.begin(), acc_weight_list.end(), roll_num);

    choose_idx = upper - acc_weight_list.begin();
    return 0;
}


//---------------------------------------------------------------------------
MersenneTwister::MersenneTwister(unsigned int seed) 
{
    buffer_[0] = seed;
    for (index_ = 1; index_ < MT_N; ++index_) 
    {
        buffer_[index_] = (1812433253UL * (buffer_[index_ - 1] ^ (buffer_[index_ - 1] >> 30)) + index_);
    }
}

unsigned int MersenneTwister::Integer() 
{
    if (index_ >= MT_N) 
    {
        unsigned int i;
        unsigned int x;
        for (i = 0; i < MT_N - MT_M; ++i) 
        {
            x = (buffer_[i] & 0x80000000UL) | (buffer_[i + 1] & 0x7fffffffUL);
            buffer_[i] = buffer_[i + MT_M] ^ (x >> 1) ^ ((x & 1) * 0x9908b0dfUL);
        }
        for (; i < MT_N - 1; ++i) 
        {
            x = (buffer_[i] & 0x80000000UL) | (buffer_[i + 1] & 0x7fffffffUL);
            buffer_[i] = buffer_[i + MT_M - MT_N] ^ (x >> 1) ^ ((x & 1) * 0x9908b0dfUL);
        }
        x = (buffer_[MT_N - 1] & 0x80000000UL) | (buffer_[0] & 0x7fffffffUL);
        buffer_[MT_N - 1] = buffer_[MT_M - 1] ^ (x >> 1) ^ ((x & 1) * 0x9908b0dfUL);
        index_ = 0;
    }
    unsigned int y = buffer_[index_++];
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);
    return y;
}



}
