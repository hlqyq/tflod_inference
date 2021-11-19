//**********************************************************************
// tflite utility functions
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-11		hlq		    generate            66536382@qq.com
//*********************************************************************

#ifndef TFLITE_UTILS_H
#define TFLITE_UTILS_H

#include <tensorflow/lite/model.h>
#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/kernels/register.h>
#include <iostream>
#include "cv_utils.h"

typedef std::vector<std::string>                                tflite_labels_type;
typedef tflite::FlatBufferModel                                 tflite_model_type;
typedef std::unique_ptr<tflite_model_type>                      tflite_model_pointer_type;
typedef tflite::Interpreter                                     tflite_interpreter_type;
typedef std::unique_ptr<tflite_interpreter_type>                tflite_interpreter_pointer_type;
enum { prpRaspberryPI4BCoreCount = 4};

//tflite labels file load
bool tflite_labels_load(const std::string& labels_file_name, std::vector<std::string>& result_labels, std::string& result_error);

// tflite model load
void tflite_interpreter_load(const std::string& model_file_name, tflite_model_pointer_type& tflite_model, tflite_interpreter_pointer_type& interpreter, unsigned int inference_thread_count = prpRaspberryPI4BCoreCount);

// get tflite model's input shape
void tflite_get_input_shape(tflite_interpreter_pointer_type& interpreter, int& result_width, int& result_height, int& result_channels);

// get tflite model's input size
void tflite_get_input_size(tflite_interpreter_pointer_type& interpreter, int& result_width, int& result_height);

// print tflite model's summary
void tflite_print_summary(tflite_interpreter_pointer_type& interpreter, std::ostream& os, bool show_tensor_details = false);

// tflite model inference
void tflite_run(tflite_interpreter_pointer_type& interpreter, cv_frame_type& frame);

//convert detection score to string
std::string tflite_score_to_string(float detection_score);

#endif  //TFLITE_UTILS_H
