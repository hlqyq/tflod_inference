//**********************************************************************
// main.cpp
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-08		hlq		    generate            66536382@qq.com
//*********************************************************************

#include "utils.h"
#include "config_utils.h"
#include "file_utils.h"
#include "cv_utils.h"
#include "fps_utils.h"
#include "video_capture_utils.h"
#include "tflite_utils.h"
#include "tflite_od_utils.h"
#include "tflite_od_glue.h"
#include "tflite_data.h"

// video capture callback
void video_capture_cb(tflite_interpreter_pointer_type& interpreter, const tflite_labels_type& tflite_labels, float confidence_threshold, int class_index_offset, const std::string application_title, cv_color_type title_color, bool success, video_frame_type& video_frame, std::string& result_error)
{
    // error handle
    if(!success)
    {
        std::cerr << result_error << std::endl;
        return;
    }

    // fps monitor
    auto scoped_fps = create_scoped_fps();

    //invoke object detection
    auto detected_objects_count = tflite_od(interpreter, tflite_labels, video_frame, confidence_threshold, class_index_offset);

    //show fps
    auto fps = get_fps();
    std::string message;
    STRING_STREAM(message) << "FPS: " << fps_to_string(fps, 1)  << " Inference time: " << fps_to_string(1000/fps, 0) << "ms  Objects: " << detected_objects_count << " threshold: " << confidence_threshold;
    int x = 10, y = 20;
    double font_scale = 0.6;
    cv_image_draw_text(video_frame, message, x, y, title_color, font_scale);

    //show frame
    cv_image_show(application_title, video_frame);
}

int main(int argc,char ** argv)
{
    // select model data directory
    std::cout << "\n-------------------- Welcome to tflite object detection --------------------" << std::endl << std::endl;
    std::string result_error, model_data_path;
    if(!select_model_data_path(get_model_data_root_path(), model_data_path, result_error))
    {
        LOG(ERROR) << result_error;
        return -1;
    }

    //load application's config parameters
    auto config_parameters_path_name = get_config_parameters_path_name(model_data_path);
    LOG(INFO) << "Load application's config parameters(\"" << get_file_name(config_parameters_path_name) << "\")...";
    if(!application_config_parameters_load(config_parameters_path_name, result_error))
    {
        LOG(ERROR) << result_error;
        return -1;
    }

    //tflite labels load
    LOG(INFO) << "Load labels file(\"" << get_file_name(tflite_get_labels_file()) << "\")...";
    tflite_labels_type tflite_labels;
    if(!tflite_labels_load(tflite_get_labels_file(), tflite_labels, result_error))
    {
        std::cerr << "loading labels failed: " << result_error << std::endl;
        return -1;
    }

    //tflite model load
    LOG(INFO) << "Load model file(\"" << get_file_name(tflite_get_model_file()) << "\")...";
    tflite_model_pointer_type tflite_model;
    tflite_interpreter_pointer_type interpreter;
    tflite_interpreter_load(tflite_get_model_file(), tflite_model, interpreter);
    tflite_print_summary(interpreter, std::cout);

    // get config parameters
    auto confidence_threshold = get_config_parameter("confidence_threshold", 0.45f);
    auto application_title = get_config_parameter("application_name", "RPi 4 - 1,9 GHz - 2 Mb RAM");
    auto camera_info = get_config_parameter("camera_index_or_file", "");    // 0 or "James.mp4"
    auto class_index_offset = get_config_parameter("class_index_offset", 0); // 1 if class("unlabeled") exists else 0
    auto title_color = cv_color_from_string(get_config_parameter("title_color", "0,0,255"));
    auto is_video = camera_info.find('.') != std::string::npos;

    // start video capture loop
    auto _video_capture_cb = std::bind(video_capture_cb, std::ref(interpreter), std::ref(tflite_labels), confidence_threshold, class_index_offset, application_title, title_color, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    if(is_video)
    {
        LOG(INFO) << "Start grabbing(\"" << camera_info << "\")... \npress ESC on Live window to terminate...";
        video_capture_loop(path_join(get_application_config_parameters_path(), camera_info, ""), VK_ESCAPE, _video_capture_cb);
    }
    else
    {
        auto camera_index = camera_info.empty() ? 0 : std::stol(camera_info);
        LOG(INFO) << "Start grabbing(camera: " << camera_index << ")... \npress ESC on Live window to terminate...";
        video_capture_loop(camera_index, VK_ESCAPE, _video_capture_cb);
    }

    // release resource
    if(is_video)
        LOG(INFO) << "Closing the video...";
    else
        LOG(INFO) << "Closing the camera...";
    LOG(INFO) << "Bye!";
    return 0;
}
