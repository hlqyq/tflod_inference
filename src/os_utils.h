//**********************************************************************
// OS utility functions
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-12		hlq		    generate            66536382@qq.com
//*********************************************************************

#ifndef OS_UTILS_H
#define OS_UTILS_H

#include <string>
#include <functional>

//Get path name of current executable
std::string os_get_current_execuable_path_name(void);

//enum subdir and files in special path
typedef std::function<void(bool success, const std::string& result_path, const std::string& result_name, bool is_dir, const std::string& result_error, bool& terminate)> os_directory_visit_type;
void os_directory_visit(const std::string& path, os_directory_visit_type visit_cb);

// check directory exists
bool os_directory_exists(const std::string& path);

#endif  //FILE_UTILS_H