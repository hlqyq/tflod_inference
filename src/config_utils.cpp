//**********************************************************************
// Config parameters utility functions
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-12		hlq		    generate            66536382@qq.com
//*********************************************************************

#include "config_utils.h"
#include "file_utils.h"
#include "utils.h"
#include "string_utils.h"
#include <fstream>
#include <map>
#include <algorithm>

// get config parameters file name
const std::string get_config_parameters_file_name(void)
{
    return "model.cfg";
}

// get config parameters path name
const std::string get_config_parameters_path_name(const std::string& path)
{
    return path_join(path, get_config_parameters_file_name(), "");
}

//test is a comment line
bool config_is_comment_line(const std::string& line)
{
    auto p = std::find_if(line.begin(), line.end(), [](unsigned char ch){return !std::isspace(ch);});
    return (p != line.end()) && (*p == '#');
}

//load config parameters file
bool config_parameters_load(const std::string& config_path_name, config_paramter_dict_type& config_parameters_dict, std::string& result_error)
{
    if(!file_exists(config_path_name))
    {
        STRING_STREAM(result_error) << "Config parameters file(" << config_path_name << ") is not exists or openable.";
        return false;
    }

    std::ifstream file(config_path_name.c_str());
    if (!file.is_open())
    {
        STRING_STREAM(result_error) << "Couldn't open config parameters file(\"" << config_path_name << "\") for reading.";
        return false;
    }

    std::string line;
    while(std::getline(file, line))
    {
        if(!line.empty())
        {
            if(!config_is_comment_line(line))
            {
                trim(line);
                auto pos = line.find("=");
                if((pos != std::string::npos) && (pos > 0) && (pos < (line.length() - 1)))
                {
                    auto name = line.substr(0, pos);
                    trim(name);
                    auto value = line.substr(pos + 1);
                    trim(value);
                    if(!name.empty())
                        config_parameters_dict.insert(std::make_pair(name, value));
                }
            }
        }
    }
    return true;
}

// get default config parameter dict
config_paramter_dict_type& get_default_config_parameter_dict(void)
{
    static config_paramter_dict_type s_default_config_parameter_dict;
    return s_default_config_parameter_dict;
}

// get path of application's config parameters file
std::string& get_application_config_parameters_path(void)
{
    static std::string s_application_config_parameters_path;
    return s_application_config_parameters_path;
}

//application's config parameters file load
bool application_config_parameters_load(const std::string& config_path_name, std::string& result_error)
{
    bool result = config_parameters_load(config_path_name, get_default_config_parameter_dict(), result_error);
    if(result)
    {
        std::string path, base_name, ext_name;
        split_file_path_name(config_path_name, path, base_name, ext_name);
        get_application_config_parameters_path() = path;
    }
    return result;
}

//get config parameter
const std::string& get_config_parameter(const config_paramter_dict_type& config_parameters_dict, const std::string& parameter_name, const std::string& default_value)
{
    auto p = config_parameters_dict.find(parameter_name);
    return p != config_parameters_dict.end() ? p->second : default_value;
}

//get application's config parameter
const std::string& get_config_parameter(const std::string& parameter_name, const std::string& default_value)
{
    return get_config_parameter(get_default_config_parameter_dict(), parameter_name, default_value);
}
