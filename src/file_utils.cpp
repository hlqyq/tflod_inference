//**********************************************************************
// File utility functions
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-12		hlq		    generate            66536382@qq.com
//*********************************************************************

#include "file_utils.h"
#include "os_utils.h"
#include <fstream>
#include <sstream>
#include <algorithm>

//split path and file name
void split_path_and_file_name(const std::string& path_name, std::string& result_path, std::string& result_file_name)
{
    auto p = std::find_if(path_name.rbegin(), path_name.rend(), [](unsigned char ch) { return ch == '/' || ch == '\\';}).base(), p_begin = path_name.begin();
    if(p != p_begin)
    {
        result_path = path_name.substr(0, p - p_begin - 1);
        result_file_name = path_name.substr(p - p_begin);
    }
    else
    {
        if(!result_path.empty())
            result_path.clear();
        result_file_name = path_name;
    }
}

//split file base name and ext name
void split_base_name_and_ext_name(const std::string& file_name, std::string& base_name, std::string& ext_name)
{
   auto p = std::find_if(file_name.rbegin(), file_name.rend(), [](unsigned char ch) { return ch == '.';}).base(), p_begin = file_name.begin();
    if(p != p_begin)
    {
        base_name = file_name.substr(0, p - p_begin - 1);
        ext_name = file_name.substr(p - p_begin - 1);
    }
    else
    {
        if(!ext_name.empty())
            ext_name.clear();
        base_name = file_name;
    }
}

//split path name to (path, base_name, ext name)
void split_file_path_name(const std::string& path_name, std::string& path, std::string& base_name, std::string& ext_name)
{
    std::string file_name;
    split_path_and_file_name(path_name, path, file_name);
    split_base_name_and_ext_name(file_name, base_name, ext_name);
}

// get path
const std::string get_path(const std::string& path_name)
{
    std::string path, file_name;
    split_path_and_file_name(path_name, path, file_name);
    return path;
}

// get file name
const std::string get_file_name(const std::string& path_name)
{
    std::string path, file_name;
    split_path_and_file_name(path_name, path, file_name);
    return file_name;
}

// get file base name
const std::string get_file_base_name(const std::string& path_name)
{
    std::string path, base_name, ext_name;
    split_file_path_name(path_name, path, base_name, ext_name);
    return base_name;
}

// get file ext name
const std::string get_file_ext_name(const std::string& path_name)
{
    std::string base_name, ext_name;
    split_base_name_and_ext_name(path_name, base_name, ext_name);
    return ext_name;
}

//get path name of current executable
const std::string get_current_execuable_path_name(void)
{
    return os_get_current_execuable_path_name();
}

//get path of current executable
const std::string get_current_execuable_path(void)
{
    return get_path(get_current_execuable_path_name());
}

//get file name of current executable
const std::string get_current_execuable_file_name(void)
{
    return get_file_name(get_current_execuable_path_name());
}

//get base name of current executable
const std::string get_current_execuable_base_name(void)
{
    return get_file_base_name(get_current_execuable_path_name());
}

//get ext name of current executable
const std::string get_current_execuable_ext_name(void)
{
    return get_file_ext_name(get_current_execuable_path_name());
}

//get separator of path
unsigned char get_path_separator(void)
{
    std::string path_name = get_current_execuable_path_name();
    auto p = std::find_if(path_name.rbegin(), path_name.rend(), [](unsigned char ch) { return ch == '/' || ch == '\\';}).base(), p_begin = path_name.begin();
    return (p != p_begin) ? *(--p) : '/';
}

// path join
std::string path_join(const std::string& path, const std::string& base_name, const std::string& ext_name)
{
    auto separator = get_path_separator();
    std::stringstream ss;
    auto path_empty = path.empty();
    if(!path_empty)
        ss << path;
    if(!base_name.empty())
    {
        if(!path_empty)
            ss << separator;
        ss << base_name;
    }
    if(!ext_name.empty())
        ss << ext_name;
    return ss.str();
}

//test file can be opened
bool file_openable(const std::string& path_name)
{
    std::fstream fs(path_name, std::ios::in);
    return fs.is_open();
}

//test file exists(https://linyunwen.github.io/2021/10/20/c-c-file-exist/)
bool file_exists(const std::string& path_name)
{
    return file_openable(path_name);
}

// test directory exists
bool directory_exists(const std::string& path)
{
    return os_directory_exists(path);
}

// get subdirectory list
std::vector<std::string> get_directory_list(const std::string& path)
{
    std::vector<std::string> result_list;
    os_directory_visit(path, [&result_list](bool success, const std::string& result_path, const std::string& result_name, bool is_dir, const std::string& result_error, bool& terminate)
    {
        if(success && is_dir)
        {
            result_list.push_back(path_join(result_path, result_name, ""));
        }
    });
    return result_list;
}

// get file list
std::vector<std::string> get_file_list(const std::string& path)
{
    std::vector<std::string> result_list;
    os_directory_visit(path, [&result_list](bool success, const std::string& result_path, const std::string& result_name, bool is_dir, const std::string& result_error, bool& terminate)
    {
        if(success && !is_dir)
        {
            result_list.push_back(path_join(result_path, result_name, ""));
        }
    });
    return result_list;
}
