//**********************************************************************
// tflite object detection utility functions
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-08		hlq		    generate            66536382@qq.com
//*********************************************************************

#include "tflite_od_utils.h"
#include "tflite_utils.h"
#include "cv_utils.h"
#include "file_utils.h"
#include "tflite_od_glue.h"

// get tflite od outputs pointers
void tflite_od_get_outputs(tflite_interpreter_pointer_type& interpreter, int& detection_count, const float*& p_locations, const float*& p_classes, const float*& p_scores)
{
    const std::vector<int>& outputs = interpreter->outputs();
    int tensor_index = 0;
    p_locations = interpreter->tensor(outputs[tensor_index++])->data.f;
    p_classes = interpreter->tensor(outputs[tensor_index++])->data.f;
    p_scores = interpreter->tensor(outputs[tensor_index++])->data.f;
    detection_count = *interpreter->tensor(outputs[tensor_index])->data.f;
}

// get normalize box of target detection object
void tflite_od_get_normalize_box(const float* p_detection_locations, int detection_index, float& x1, float& y1, float& x2, float& y2)
{
    int location_offset = 4 * detection_index;
    y1 = p_detection_locations[location_offset++];
    x1 = p_detection_locations[location_offset++];
    y2 = p_detection_locations[location_offset++];
    x2 = p_detection_locations[location_offset];
}

// get box of target detection object
void tflite_od_get_box(const float* p_detection_locations, int detection_index, int frame_width, int frame_height, int& x1, int& y1, int& x2, int& y2)
{
    //get normalize box
    float x1_, y1_, x2_, y2_;
    tflite_od_get_normalize_box(p_detection_locations, detection_index, x1_, y1_, x2_, y2_);

    //get pixel space box
    y1 = static_cast<int>(y1_ * frame_height);
    x1 = static_cast<int>(x1_ * frame_width);
    y2 = static_cast<int>(y2_ * frame_height);
    x2 = static_cast<int>(x2_ * frame_width);
}

// tflite od callback
void tflite_od_cb(tflite_interpreter_pointer_type& interpreter, cv_frame_type& frame, float detection_score, int detection_class_index, const std::string& detection_class, int x1, int y1, int x2, int y2)
{
    // get color
    unsigned char red, green, blue;
    tflite_od_get_class_color(detection_class_index, detection_class.c_str(), red, green, blue);
    auto clr = cv_color(red, green, blue);

    //draw object box
    cv_image_draw_rectangle(frame, x1, y1, x2, y2, clr);

    //draw object information
    std::stringstream ss;
    ss << detection_class << ":" << tflite_score_to_string(detection_score);
    double font_scale = 0.6;
    cv_image_draw_text(frame, ss.str(), x1, y1 - 5, clr, font_scale);
}

// tflite od outputs parse
unsigned int tflite_od_outputs_parse(tflite_interpreter_pointer_type& interpreter, const tflite_labels_type& tflite_labels, cv_frame_type& frame, const float confidence_threshold, int class_index_offset, tflite_od_cb_type handle_cb)
{
    // get frame size
    int frame_width = 0, frame_height = 0;
    cv_image_get_size(frame, frame_width, frame_height);

    // get tflite inference results
    const float* detection_locations, *detection_classes, *detection_scores;
    int num_detections;
    tflite_od_get_outputs(interpreter, num_detections, detection_locations, detection_classes, detection_scores);

    // iterate result
    unsigned int detected_objects_count = 0;
    for(int i = 0; i < num_detections; ++i)
    {
        float detection_score = detection_scores[i];
        if(detection_score > confidence_threshold)
        {
            // get box of target object
            int x1, y1, x2, y2;
            tflite_od_get_box(detection_locations, i, frame_width, frame_height, x1, y1, x2, y2);

            // get detection class information
            int detection_class_index = (int)detection_classes[i] + class_index_offset;
            const std::string& detection_class = tflite_labels[detection_class_index];

            // invoke callback
            handle_cb(interpreter, frame, detection_score, detection_class_index, detection_class, x1, y1, x2, y2);
            ++detected_objects_count;
        }
    }
    return detected_objects_count;
}

//run tflite od inference
unsigned int tflite_od(tflite_interpreter_pointer_type& interpreter, const tflite_labels_type& tflite_labels, cv_frame_type& frame, const float confidence_threshold, int class_index_offset, tflite_od_cb_type handle_cb)
{
    //run tflite inference
    tflite_run(interpreter, frame);

    // parse tflite od result
    return tflite_od_outputs_parse(interpreter, tflite_labels, frame, confidence_threshold, class_index_offset, handle_cb);
}
