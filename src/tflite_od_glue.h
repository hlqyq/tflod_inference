//**********************************************************************
// tflite object detection glue utility functions
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-12		hlq		    generate            66536382@qq.com
//*********************************************************************

#ifndef TFLITE_OD_GLUE_H
#define TFLITE_OD_GLUE_H

#include <string>

//get tflite model file(exists in application's folder)
const std::string tflite_get_model_file(void);

//get tflite labels file(exists in application's folder)
const std::string tflite_get_labels_file(void);

//get color of detection class
void tflite_od_get_class_color(int detection_class_index, const std::string& detection_class_name, unsigned char& result_red, unsigned char& result_green, unsigned char& result_blue);

#endif  //TFLITE_OD_GLUE_H
