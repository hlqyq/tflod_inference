//**********************************************************************
// tflite object detection utility functions
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-08		hlq		    generate            66536382@qq.com
//*********************************************************************

#ifndef TFLITE_OD_UTILS_H
#define TFLITE_OD_UTILS_H

#include <functional>
#include "cv_utils.h"
#include "tflite_utils.h"

typedef std::function<void(tflite_interpreter_pointer_type& interpreter, cv_frame_type& frame, float detection_score, int detection_class_index, const std::string& detection_class, int x1, int y1, int x2, int y2)> tflite_od_cb_type;

//tflite od callback
void tflite_od_cb(tflite_interpreter_pointer_type& interpreter, cv_frame_type& frame, float detection_score, int detection_class_index, const std::string& detection_class, int x1, int y1, int x2, int y2);

//run tflite od inference
unsigned int tflite_od(tflite_interpreter_pointer_type& interpreter, const tflite_labels_type& tflite_labels, cv_frame_type& frame, const float confidence_threshold, int class_index_offset, tflite_od_cb_type handle_cb = tflite_od_cb);

#endif  //TFLITE_OD_UTILS_H