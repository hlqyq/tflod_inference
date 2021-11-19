//**********************************************************************
// tflite utility functions
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-11		hlq		    generate            66536382@qq.com
//*********************************************************************

#include "tflite_utils.h"
#include "utils.h"
#include "cv_utils.h"
#include "string_utils.h"
#include <fstream>
#include <sstream>
#include <iomanip>

//tflite labels file load
bool tflite_labels_load(const std::string& labels_file_name, std::vector<std::string>& result_labels, std::string& result_error)
{
    std::ifstream file(labels_file_name);
    if(!file)
    {
        STRING_STREAM(result_error) << "Labels file " << labels_file_name << " not found";
        return false;
    }

    result_labels.clear();
    std::string line;
    while (std::getline(file, line))
    {
        trim(line);
        if(!line.empty())
            result_labels.push_back(line);
    }
    return true;
}

// tflite model load
void tflite_interpreter_load(const std::string& model_file_name, tflite_model_pointer_type& tflite_model, tflite_interpreter_pointer_type& interpreter, unsigned int inference_thread_count)
{
    // load model
    tflite_model = tflite::FlatBufferModel::BuildFromFile(model_file_name.c_str());

    // construct interpreter
    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder(*tflite_model.get(), resolver)(&interpreter);

    // allocate tensors
    interpreter->AllocateTensors();

    // interpreter initialize
    interpreter->SetAllowFp16PrecisionForFp32(true);
    if(inference_thread_count > 0)
        interpreter->SetNumThreads(inference_thread_count);
}

// get tflite model's input shape
void tflite_get_input_shape(tflite_interpreter_pointer_type& interpreter, int& result_width, int& result_height, int& result_channels)
{
    int input = interpreter->inputs()[0];
    TfLiteIntArray* dims = interpreter->tensor(input)->dims;
    result_height = dims->data[1];
    result_width = dims->data[2];
    result_channels = dims->data[3];
}

// get tflite model's input type string
const std::string tflite_get_input_type_string(tflite_interpreter_pointer_type& interpreter)
{
    std::string input_type_string;
    if(interpreter->inputs().size() > 0)
    {
        auto input_type = interpreter->tensor(interpreter->inputs()[0])->type;
        if(input_type == kTfLiteUInt8)
            STRING_STREAM(input_type_string) << "UInt8";
        else if(input_type == kTfLiteFloat32)
            STRING_STREAM(input_type_string)<< "Float32";
        else
            STRING_STREAM(input_type_string)<< input_type;
    }
    return input_type_string;
}

// get tflite model's input size
void tflite_get_input_size(tflite_interpreter_pointer_type& interpreter, int& result_width, int& result_height)
{
    int nChannels = 0;
    tflite_get_input_shape(interpreter, result_width, result_height, nChannels);
}

// print tflite model's summary
void tflite_print_summary(tflite_interpreter_pointer_type& interpreter, std::ostream& os, bool show_tensor_details)
{
    os << "\n-------------------- Model summary --------------------" << std::endl << std::endl;
    os << "    tensors size: " << interpreter->tensors_size() << std::endl;
    os << "    nodes size: " << interpreter->nodes_size() << std::endl;
    os << "    inputs: " << interpreter->inputs().size() << std::endl;
    int width, height, channels;
    tflite_get_input_shape(interpreter, width, height, channels);
    os << "    inputs(0) shape: " << width << "x" << height << "x" << channels << " " << tflite_get_input_type_string(interpreter) << std::endl;
    os << "    input(0) name: " << interpreter->GetInputName(0) << std::endl;

    if(show_tensor_details)
    {
        int t_size = interpreter->tensors_size();
        for (int i = 0; i < t_size; i++)
        {
            os << i << ": "
                << interpreter->tensor(i)->name << ", "
                << interpreter->tensor(i)->bytes << ", "
                << interpreter->tensor(i)->type << ", "
                << interpreter->tensor(i)->params.scale << ", "
                << interpreter->tensor(i)->params.zero_point << std::endl;
        }
    }
    os << "\n-------------------------------------------------------" << std::endl << std::endl;
}

// test model input need to be normalized
bool tflite_input_need_normalize(tflite_interpreter_pointer_type& interpreter)
{
    if(interpreter->inputs().size() > 0)
    {
        if(interpreter->tensor(interpreter->inputs()[0])->type == kTfLiteFloat32)
            return true;
    }
    return false;
}

// model input normalize
void tflite_input_normalize(cv_frame_type& input_image)
{
    cv_image_to_float(input_image, input_image);
    cv_image_bgr_to_rgb(input_image, input_image);
    cv_image_normalize(input_image);
}

// fill frame to interpreter
void tflite_set_input_image(tflite_interpreter_pointer_type& interpreter, cv_frame_type& frame)
{
    // convert image size
    cv_frame_type input_image;
    int input_width = 0, input_height = 0;
    tflite_get_input_size(interpreter, input_width, input_height);
    cv_image_resize(frame, input_width, input_height, input_image);

    // fill input image to interpreter
    if(tflite_input_need_normalize(interpreter))
    {
        tflite_input_normalize(input_image);
        memcpy(interpreter->typed_input_tensor<float>(0), input_image.data, input_image.total() * input_image.elemSize());
    }
    else
        memcpy(interpreter->typed_input_tensor<uchar>(0), input_image.data, input_image.total() * input_image.elemSize());
}

// tflite model inference
void tflite_run(tflite_interpreter_pointer_type& interpreter, cv_frame_type& frame)
{
    // fill frame to interpreter
    tflite_set_input_image(interpreter, frame);

    // tflite inference
    interpreter->Invoke();
}

//convert detection score to string
std::string tflite_score_to_string(float detection_score)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(0) << (detection_score * 100) << "%";
    return ss.str();
}
