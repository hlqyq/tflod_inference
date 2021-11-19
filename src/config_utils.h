//**********************************************************************
// Config parameters utility functions
// usage:
//
//     -------------------- initialize(main.cpp) --------------------
//
//        std::string result_error, config_path_name("/home/pi/tflite_data/model.cfg");
//        if(!application_config_parameters_load(data_path, result_error))
//           std::cerr << result_error;
//
//     -------------------- query --------------------
//
//          std::string application_name = get_config_parameter("application_name");
//          std::string user_name = get_config_parameter("user_name", "guest");
//          int user_age = get_config_parameter("age", 18);
//          int user_age_0 = get_config_parameter<int>("age");
//          bool adult = get_config_parameter<bool>("age");
//          bool adult_1 = get_config_parameter("age", false);
//
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-12		hlq		    generate            66536382@qq.com
//*********************************************************************

#ifndef CONFIG_UTILS_H
#define CONFIG_UTILS_H

#include <string>
#include <sstream>
#include <map>

////////////////////////////////////////  initialize ////////////////////////////////////////

typedef std::map<std::string, std::string> config_paramter_dict_type;

// get config parameters file name
const std::string get_config_parameters_file_name(void);

// get config parameters path name
const std::string get_config_parameters_path_name(const std::string& path);

//load config parameters file
bool config_parameters_load(const std::string& config_path_name, config_paramter_dict_type& config_parameters_dict, std::string& result_error);

// application's config parameters file load
bool application_config_parameters_load(const std::string& config_path_name, std::string& result_error);

// get path of application's config parameters file
std::string& get_application_config_parameters_path(void);

//////////////////////////////////////// query ////////////////////////////////////////

//get config parameter
const std::string& get_config_parameter(const config_paramter_dict_type& config_parameters_dict, const std::string& parameter_name, const std::string& default_value);

//get application's config parameter
const std::string& get_config_parameter(const std::string& parameter_name, const std::string& default_value = "");

//get application's config parameter(template)
template<typename T> const T get_config_parameter(const std::string& parameter_name, const T& default_value = T());

//////////////////////////////////////// template implements ////////////////////////////////////////

// get config parameter(generic)
template<typename T>
inline const T get_config_parameter(const std::string& parameter_name, const T& default_value)
{
    std::string empty_value;
    std::string value = get_config_parameter(parameter_name, empty_value);
    if(value.empty())
        return default_value;
    T result = T();
    std::istringstream(value) >> result;
    return result;
}

// get config parameter(bool)
template<bool>
const bool get_config_parameter(const std::string& parameter_name, const bool default_value)
{
    std::string value = get_config_parameter(parameter_name);
    if(value.empty())
        return default_value;
    bool result;
    std::istringstream(value) >> result;
    return result;
}

// get config parameter(int)
template<int>
const int get_config_parameter(const std::string& parameter_name, const int default_value)
{
    std::string value = get_config_parameter(parameter_name);
    if(value.empty())
        return default_value;
    return std::stoi(value);
}

#endif  //CONFIG_UTILS_H