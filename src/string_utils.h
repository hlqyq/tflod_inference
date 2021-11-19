//**********************************************************************
// string utility functions
// reference: https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-12		hlq		    generate            66536382@qq.com
//*********************************************************************

#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

//包含头文件
#include <string>
#include <vector>

// trim from start (in place)
void ltrim(std::string &s);

// trim from end (in place)
void rtrim(std::string &s);

// trim from both ends (in place)
void trim(std::string &s);

// trim from start (copying)
std::string ltrim_copy(std::string s);

// trim from end (copying)
std::string rtrim_copy(std::string s);

// trim from both ends (copying)
std::string trim_copy(std::string s);

// split string
unsigned int string_split(const std::string& src, const std::string& separaters, std::vector<std::string>& part_list, bool reset_result = true);

#endif  //STRING_UTILS_HPP
