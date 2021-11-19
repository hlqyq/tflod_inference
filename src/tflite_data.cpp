//**********************************************************************
// tflite model data utility functions
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-15		hlq		    generate            66536382@qq.com
//*********************************************************************

#include "tflite_data.h"
#include "file_utils.h"
#include "config_utils.h"
#include "utils.h"
#include <iostream>
#include <algorithm>

// get model data root directory
const std::string& get_model_data_root_path(void)
{
    static std::string s_model_data_root_path = path_join(get_current_execuable_path(), "model_datas", "");
    return s_model_data_root_path;
}

// test path is a model data path
bool is_model_data_path(const std::string& path)
{
    if(directory_exists(path))
    {
        if(file_exists(get_config_parameters_path_name(path)))
            return true;
    }
    return false;
}

// select model data directory
bool select_model_data_path(const std::string& model_data_root_path, std::string& result_model_data_path ,std::string& result_error)
{
    // check root path exists
    if(!directory_exists(model_data_root_path))
    {
        STRING_STREAM(result_error) << "model data root path(\"" << model_data_root_path << "\") do not exists.";
        return false;
    }

    // find model data path
    auto directory_list = get_directory_list(model_data_root_path);
    std::vector<std::pair<std::string, std::string> > model_pair_list;
    for(auto path: directory_list)
    {
        if(is_model_data_path(path))
        {
            config_paramter_dict_type config_parameters_dict;
            if(!config_parameters_load(get_config_parameters_path_name(path), config_parameters_dict, result_error))
                return false;
            auto application_name = get_config_parameter(config_parameters_dict, "application_name", get_file_name(path));
            model_pair_list.push_back(std::make_pair(path, application_name));
        }
    }
    if(model_pair_list.empty())
    {
        STRING_STREAM(result_error) << "model data root path(\"" << model_data_root_path << "\") can't find valid model data path.";
        return false;
    }
    std::sort(model_pair_list.begin(), model_pair_list.end());

    // select active data path
    std::cout << "Valid model data list:" << std::endl;
    unsigned int i = 0;
    unsigned int start_pos = get_model_data_root_path().size();
    for(auto model_pair: model_pair_list)
    {
        std::cout << "    " << ++i << ". " << model_pair.second << "(" << model_pair.first.substr(start_pos) << ")" << std::endl;
    }
    std::cout << "\nPlease select a model data path: ";
    int select_index_1 = 0;
    std::cin >> select_index_1;
    if((select_index_1 < 1) || (select_index_1 > model_pair_list.size()))
    {
        STRING_STREAM(result_error) << "Invalid model data index(" << select_index_1 << "), application will exit.";
        return false;
    }
    result_model_data_path = model_pair_list[select_index_1 - 1].first;
    std::cout << "    Selected model data directory:" << result_model_data_path.substr(get_current_execuable_path().size()) << std::endl << std::endl;
    return true;
}
