//**********************************************************************
// tflite model data utility functions
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-15		hlq		    generate            66536382@qq.com
//*********************************************************************

#ifndef TFLITE_DATA_UTILS_H
#define TFLITE_DATA_UTILS_H

#include <string>

// get model data root directory
const std::string& get_model_data_root_path(void);

// select model data directory
bool select_model_data_path(const std::string& model_data_root_path, std::string& result_model_data_path ,std::string& result_error);

#endif  //TFLITE_DATA_UTILS_H