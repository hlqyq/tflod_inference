//**********************************************************************
// string utility functions
// reference: https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-12		hlq		    generate            66536382@qq.com
//*********************************************************************

#include "string_utils.h"
#include <algorithm>
#include <cctype>
#include <locale>

// trim from start (in place)
void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
    {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
    {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
void trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
std::string ltrim_copy(std::string s)
{
    ltrim(s);
    return s;
}

// trim from end (copying)
std::string rtrim_copy(std::string s)
{
    rtrim(s);
    return s;
}

// trim from both ends (copying)
std::string trim_copy(std::string s)
{
    trim(s);
    return s;
}

// split string
unsigned int string_split(const std::string& src, const std::string& separaters, std::vector<std::string>& part_list, bool reset_result)
{
    if(reset_result && (!part_list.empty()))
        part_list.clear();
    auto p = src.begin(), p_prev = p, p_end = src.end();
    for(; p != p_end;++p)
    {
        if(separaters.find(*p) != std::string::npos)
        {
            if(p != p_prev)
            {
                auto part = std::string(p_prev, p);
                trim(part);
                if(!part.empty())
                    part_list.push_back(part);
                p_prev = p + 1;
            }
        }
    }
    if(p != p_prev)
    {
        auto part = std::string(p_prev, p);
        trim(part);
        if(!part.empty())
            part_list.push_back(part);
        p_prev = p + 1;
    }
    return part_list.size();
}
