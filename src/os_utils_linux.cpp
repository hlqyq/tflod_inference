//**********************************************************************
// OS utility functions
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-12		hlq		    generate            66536382@qq.com
//*********************************************************************

#include "os_utils.h"
#include "utils.h"
#include <string>
#include <limits.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

//Get path name of current executable (https://stackoverflow.com/questions/143174/how-do-i-get-the-directory-that-a-program-is-running-from)
std::string os_get_current_execuable_path_name(void)
{
    char result[PATH_MAX] = "\0";
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    return std::string(result, (count > 0) ? count : 0);
}

//enum subdir and files in special path
void os_directory_visit(const std::string& path, os_directory_visit_type visit_cb)
{
    //open directory
    DIR* p_dir = opendir(path.c_str());
    if (p_dir == nullptr)
    {
        bool terminate = false;
        visit_cb(false, "", "", false, "Could not open current directory", terminate);
        return;
    }
    auto scoped_close_dir = create_scoped_guard(std::bind(closedir, p_dir));

    // read directory
    std::string error;
    struct dirent* p_de = nullptr;  // Pointer for directory entry
    while ((p_de = readdir(p_dir)) != nullptr)
    {
        if (strcmp(p_de->d_name, ".") != 0 && strcmp(p_de->d_name, "..") != 0)
        {
            bool terminate = false;
            visit_cb(true, path, p_de->d_name, p_de->d_type == DT_DIR, error, terminate);
            if(terminate)
                break;
        }
    }
}

// check directory exists
bool os_directory_exists(const std::string& path)
{
    struct stat st;
    if(stat(path.c_str(), &st) == 0)
    {
        if((st.st_mode & S_IFDIR) != 0)
            return true;
    }
    return false;
}
