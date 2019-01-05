/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#ifndef __RANDOM_UTILS_H_
#define __RANDOM_UTILS_H_

#include <vector>
#include <iterator>
#include <stdint.h>
#include <stdio.h>
#include "math.h"
#if __cplusplus >= 201103L
#include <random>
#endif

namespace base_utils
{

class RandomUtils
{
public:
    // 根据权重随机一个值。返回vector的index。
    // 返回大小 <= n
    static std::vector<size_t> ChooseNByWeight(const std::vector<unsigned> &weight_list, size_t n);

public:
    // weight_idx不可为空。
    static int ChooseOneByWeight(const std::vector<unsigned> &weight_list, size_t &choose_idx);

#if __cplusplus >= 201103L
    //闭区间[min,max] min <= max
    static int GetRandomInteger(int min, int max)
    {
        //加个保护 以免assert
        if(min > max)
        {
            return 0;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(min, max);

        return dis(gen);
    }

    template<typename C>
    static bool GetRandomElement(const C &c, typename C::value_type &e)
    {
        if(c.empty())
        {
            return false;
        }

        auto iter = c.begin();
        std::advance(iter, GetRandomInteger(0, c.size() - 1));
        e = *iter;

        return true;
    }

    template<typename C>
    static typename C::value_type GetRandomElement(const C &c, const typename C::value_type &e = typename C::value_type())
    {
        if(c.empty())
        {
            return e;
        }
        auto iter = c.begin();
        std::advance(iter, GetRandomInteger(0, c.size() - 1));
        return *iter;
    }
#endif

private:

};


#define MT_N 624
#define MT_M 397

class MersenneTwister 
{
public:
    explicit MersenneTwister(unsigned int seed);
    unsigned int Integer();
    float PoissonInterval(float rate) 
    {
        return -logf(1.0f - Integer() * 2.3283e-10f) * rate;
    }

private:
    unsigned int buffer_[MT_N];
    int index_;
};


}

#endif