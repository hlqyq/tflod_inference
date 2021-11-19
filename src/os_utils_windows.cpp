//**********************************************************************
// OS utility functions
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-12		hlq		    generate            66536382@qq.com
//*********************************************************************

#include "os_utils.h"
#include <string>
#include <windows.h>
#include "utils.h"

//Get path name of current executable(https://stackoverflow.com/questions/143174/how-do-i-get-the-directory-that-a-program-is-running-from)
std::string os_get_current_execuable_path_name(void)
{
    char result[MAX_PATH] = "\0";
    return std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
}

//enum subdir and files in special path
void os_directory_visit(const std::string& path, os_directory_visit_type visit_cb)
{
    WIN32_FIND_DATA fd;
    std::string find_path = path + "\\*.*"
    HANDLE h_find = FindFirstFile(find_path.c_str(), &fd);
    if(h_find == INVALID_HANDLE_VALUE)
    {
        bool terminate = false;
        visit_cb(false, "", "", false, "Could not open current directory", terminate);
        return;
    }
    auto scoped_find_close = create_scoped_guard(std::bind(FindClose, h_find));

    do
    {
        if (strcmp(fd.cFileName, ".") != 0 && strcmp(fd.cFileName, "..") != 0)
        {
            bool terminate = false;
            visit_cb(true, path, fd.cFileName, fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY, error, terminate);
            if(terminate)
                break;
        }
    }while(FindNextFile(hFind, &fd));
}

// check directory exists
bool os_directory_exists(const std::string& path)
{
    auto file_type = GetFileAttributesA(path.c_str());
    if (file_type == INVALID_FILE_ATTRIBUTES)
        return false;
    if (file_type & FILE_ATTRIBUTE_DIRECTORY)
        return true;
    return false;
}
