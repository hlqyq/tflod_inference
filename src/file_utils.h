//**********************************************************************
// File utility functions
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-12		hlq		    generate            66536382@qq.com
//*********************************************************************

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <vector>

//split path and file name
void split_path_and_file_name(const std::string& path_name, std::string& result_path, std::string& result_file_name);

//split file base name and ext name
void split_base_name_and_ext_name(const std::string& file_name, std::string& base_name, std::string& ext_name);

//split path name to (path, base_name, ext name)
void split_file_path_name(const std::string& path_name, std::string& path, std::string& base_name, std::string& ext_name);

// get path
const std::string get_path(const std::string& path_name);

// get file name
const std::string get_file_name(const std::string& path_name);

// get file base name
const std::string get_file_base_name(const std::string& path_name);

//get path name of current executable
const std::string get_current_execuable_path_name(void);

//get path of current executable
const std::string get_current_execuable_path(void);

//get file name of current executable
const std::string get_current_execuable_file_name(void);

//get base name of current executable
const std::string get_current_execuable_base_name(void);

//get ext name of current executable
const std::string get_current_execuable_ext_name(void);

//get separator of path
unsigned char get_path_separator(void);

// path join
std::string path_join(const std::string& path, const std::string& base_name, const std::string& ext_name);

// test file exists
bool file_exists(const std::string& path_name);

// test directory exists
bool directory_exists(const std::string& path);

// get subdirectory list
std::vector<std::string> get_directory_list(const std::string& path);

// get file list
std::vector<std::string> get_file_list(const std::string& path);

#endif  //FILE_UTILS_H