/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 泛型AC自动机，用于高效率匹配多个关键字 AC自动机算法详见：http://www.cppblog.com/mythit/archive/2009/04/21/80633.html
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef __AC_AUTO_MATON_H__
#define __AC_AUTO_MATON_H__ 

#include <vector>
#include <set>
#include <map>
#include <queue>
#include <stdint.h>

namespace base_utils
{

template<typename CharType, bool STORE_STATE_ROUTE = false, size_t MAX_ROUTE_CACHE = 32>
class AC_Automaton_General
{
private:
    struct Node
    {
        uint32_t father; //父节点编号
        int32_t fail; //fail节点编号
        size_t depth; //距离根节点的深度
        CharType ch; //节点的字符
        bool is_keyword; //是否是一个关键字的匹配节点
        std::set<size_t> keyword_lens; //能匹配到的所有关键字的长度
        std::map<CharType, uint32_t> childs; //各个子节点
        std::map<CharType, uint32_t> route; //保存的快速跳转 (STORE_STATE_ROUTE = true 时才使用)

        Node() : father(0), fail(-1), depth(0), is_keyword(false) {}
    };

public:
    AC_Automaton_General() : _curr_pos(0), _fail_built(false) {_tree.resize(1);}

    void ReserveTreeNodes(size_t size)
    {
        _tree.reserve(size);
    }

    void Reset()
    {
        _curr_pos = 0;
        _fail_built = false;
        _tree.clear();
        _tree.resize(1);
    }

    //往字典里添加一个关键词
    template<typename StringType>
    void AddKeyword(const StringType& keyword)
    {
        AddKeyword(keyword.begin(), keyword.end());
    }

    //往字典里添加一个关键词
    template<typename IterType>
    void AddKeyword(IterType begin, IterType end)
    {
        if (begin == end)
        {
            return;
        }

        _fail_built = false;
        uint32_t pos = 0;
        size_t len = 0;
        for (IterType it = begin; it != end; ++it)
        {
            ++len;
            Node& curr_node = _tree[pos];
            std::map<CharType, uint32_t>::const_iterator it_child = curr_node.childs.find(*it);
            if (it_child != curr_node.childs.end())
            {
                pos = it_child->second;
                continue;
            }
            _tree.resize(_tree.size() + 1);
            Node& child = _tree.back();
            child.father = pos;
            child.fail = 0;
            child.depth = len;
            child.ch = *it;
            _tree[pos].childs[*it] = _tree.size() - 1;
            pos = _tree.size() - 1;
        }
        _tree[pos].is_keyword = true;
    }

    //对指定的文本内容进行匹配
    //回调应该满足这样的形式：
    //  int callback_func(size_t parsed_chars, size_t keyword_len)
    //    回调入参分别是，当前处理了多少个字符，以及当前匹配到的关键字的长度
    //    回调函数若返回0，则匹配过程结束，返回非0则继续匹配
    //匹配算法的时间复杂度是 O(文本内容长度 + 所有关键字总长度)
    template<typename StringType, typename Callback>
    void Match(const StringType& text, Callback cb)
    {
        Match(text.begin(), text.end(), cb);
    }

    template<typename IterType, typename Callback>
    void Match(IterType begin, IterType end, Callback cb)
    {
        _curr_pos = 0;
        BuildFail();
        size_t parsed_chars = 0;
        for (IterType it = begin; it != end; ++it)
        {
            PutChar(*it);
            ++parsed_chars;
            Node& node = _tree[_curr_pos];
            for (std::set<size_t>::const_iterator it_k = node.keyword_lens.begin(); it_k != node.keyword_lens.end(); ++it_k)
            {
                if (cb(parsed_chars, *it_k) == 0)
                {
                    return;
                }
            }
        }
    }

private:
    void BuildFail()
    {
        if (_fail_built)
        {
            return;
        }

        std::queue<uint32_t> node_queue;
        node_queue.push(0);

        while (node_queue.size())
        {
            uint32_t node_idx = node_queue.front();
            node_queue.pop();
            Node& node = _tree[node_idx];

            node.keyword_lens.clear();
            if (node.is_keyword)
            {
                node.keyword_lens.insert(node.depth);
            }
            if (STORE_STATE_ROUTE)
            {
                node.route.clear();
            }

            if (node_idx) //仅仅对非root节点构建fail链
            {
                int32_t fail = _tree[node.father].fail; //从father.fail开始，沿着已经生成的fail链进行寻找

                while (fail >= 0)
                {
                    Node& father_fail_node = _tree[fail];
                    std::map<CharType, uint32_t>::const_iterator it_child = father_fail_node.childs.find(node.ch);
                    if (it_child != father_fail_node.childs.end()) //如果父节点的fail链上有一个节点，含有与当前结点相同字符的子节点
                    {
                        node.fail = it_child->second;
                        Node& fail_node = _tree[node.fail];
                        node.keyword_lens.insert(fail_node.keyword_lens.begin(), fail_node.keyword_lens.end());
                        break;
                    }
                    fail = father_fail_node.fail;
                }

                if (fail < 0)
                {
                    node.fail = 0;
                }
            }

            FOR_EACH(it, node.childs)
            {
                node_queue.push(it->second);
            }
        }

        _fail_built = true;
    }

    void PutChar(const CharType& ch)
    {
        Node& node = _tree[_curr_pos];
        if (STORE_STATE_ROUTE)
        {
            std::map<CharType, uint32_t>::const_iterator it_route = node.route.find(ch);
            if (it_route != node.route.end())
            {
                _curr_pos = it_route->second;
                return;
            }
        }

        int32_t pos = _curr_pos;

        while (pos >= 0) //检查当前结点是否有包含ch的子节点，如果没有，就沿着fail链进行尝试
        {
            Node& curr_node = _tree[pos];
            std::map<CharType, uint32_t>::const_iterator it_child = curr_node.childs.find(ch);
            if (it_child != curr_node.childs.end())
            {
                _curr_pos = it_child->second;
                break;
            }
            pos = curr_node.fail;
        }

        if (pos < 0) //如果在fail链上找不到任何包含ch的子节点，则状态归位到root节点
        {
            _curr_pos = 0;
        }

        if (STORE_STATE_ROUTE)
        {
            node.route[ch] = _curr_pos;
            if (node.route.size() > MAX_ROUTE_CACHE)
            {
                for(std::map<CharType, uint32_t>::const_iterator it = node.route.begin(), it != node.route.end(); ++it)
                {
                    if (it->first != ch)
                    {
                        node.route.erase(it);
                        break;
                    }
                }
            }
        }
    }

private:
    uint32_t _curr_pos;
    bool _fail_built;
    std::vector<Node> _tree;
};

typedef AC_Automaton_General<char, true> AC_Automaton_String;

}

#endif