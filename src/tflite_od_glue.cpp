//**********************************************************************
// tflite object detection glue utility functions
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-12		hlq		    generate            66536382@qq.com
//*********************************************************************

#include "tflite_od_glue.h"
#include "config_utils.h"
#include "file_utils.h"
#include "string_utils.h"
#include "utils.h"
#include <random>
#include <map>

//get tflite model file(exists in application's folder)
const std::string tflite_get_model_file(void)
{
    std::string config_path = get_application_config_parameters_path();
    auto file_name = get_config_parameter("model_file");
    if(file_name.empty())
        file_name = get_file_name(config_path) + ".tflite";
    return path_join(config_path, file_name, "");
}

//get tflite labels file(exists in application's folder)
const std::string tflite_get_labels_file(void)
{
    std::string config_path = get_application_config_parameters_path();
    auto file_name = get_config_parameter("labels_file");
    if(file_name.empty())
        file_name = get_file_name(config_path) + "_labels.txt";
    return path_join(config_path, file_name, "");
}

//get color of detection class
void tflite_od_get_class_color_impl(int detection_class_index, const std::string& detection_class_name, unsigned char& result_red, unsigned char& result_green, unsigned char& result_blue)
{
    // get parameter name
    std::string parameter_name;
    STRING_STREAM(parameter_name) << "class_" << detection_class_index << "_color";
    auto colors = get_config_parameter(parameter_name);
    if(!colors.empty())
    {
        std::vector<std::string> clr_part_list;
        string_split(colors, ",;", clr_part_list);
        if(clr_part_list.size() == 3)
        {
            result_red = static_cast<unsigned char>(std::stoi(clr_part_list[0]));
            result_green = static_cast<unsigned char>(std::stoi(clr_part_list[1]));
            result_blue = static_cast<unsigned char>(std::stoi(clr_part_list[2]));
            return;
        }
    }

    result_red = result_green = result_blue = 0;
    if(detection_class_index == 0)  // normal class
        result_green = 255;
    else
    {
        result_red = rand() % 256;
        result_green = rand() % 256;
        result_blue = rand() % 256;
    }
}

//get color of detection class
void tflite_od_get_class_color(int detection_class_index, const std::string& detection_class_name, unsigned char& result_red, unsigned char& result_green, unsigned char& result_blue)
{
    typedef std::map<int, std::tuple<unsigned char, unsigned char, unsigned char> > color_dict_type;
    static color_dict_type s_cache_dict;
    auto p = s_cache_dict.find(detection_class_index);
    if(p == s_cache_dict.end())
    {
        tflite_od_get_class_color_impl(detection_class_index, detection_class_name, result_red, result_green, result_blue);
        s_cache_dict.insert(std::make_pair(detection_class_index, std::make_tuple(result_red, result_green, result_blue)));
    }
    else
        std::tie(result_red, result_green, result_blue) = p->second;
}

