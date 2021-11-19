//**********************************************************************
// Video capture utility functions
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-08		hlq		    generate            66536382@qq.com
//*********************************************************************

#include "video_capture_utils.h"
#include <opencv2/opencv.hpp>
#include "utils.h"

//video capture loop
void video_capture_loop(cv::VideoCapture& cap, key_press_cb_type key_press_cb, video_capture_cb_type video_capture_cb)
{
    video_frame_type frame;
    std::string result_error;
    bool terminate_loop = false;
    if (!cap.isOpened())
    {
        STRING_STREAM(result_error) << "ERROR: Unable to open the camera";
        video_capture_cb(false, frame, result_error);
        return;
    }

    //start video capture loop
	while(true)
    {
        // capture video frame
        cap >> frame;
        if (frame.empty())
        {
            STRING_STREAM(result_error) << "ERROR: Unable to grab from the camera";
            video_capture_cb(false, frame, result_error);
            break;
        }

        // video frame callback
        video_capture_cb(true, frame, result_error);

        // handle key terminate event
        terminate_loop = false;
        key_press_cb(static_cast<unsigned char>(cv::waitKey(5)), terminate_loop);
        if(terminate_loop)
            break;
    }

    //release resource
    cv::destroyAllWindows();
}

//video capture loop(by vcamera index)
void video_capture_loop(unsigned int camera_index, key_press_cb_type key_press_cb, video_capture_cb_type video_capture_cb)
{
    cv::VideoCapture cap(camera_index);
    video_capture_loop(cap, key_press_cb, video_capture_cb);
}

//video capture loop(by video file)
void video_capture_loop(const std::string& video_file, key_press_cb_type key_press_cb, video_capture_cb_type video_capture_cb)
{
    cv::VideoCapture cap(video_file);
    video_capture_loop(cap, key_press_cb, video_capture_cb);
}

//video capture loop(by camera index)
void video_capture_loop(unsigned int camera_index, unsigned char terminate_key, video_capture_cb_type video_capture_cb)
{
    auto key_press_cb = [&terminate_key](unsigned char key_code, bool& terminate_loop)
    {
        if(key_code == terminate_key)
            terminate_loop = true;
    };
    video_capture_loop(camera_index, key_press_cb, video_capture_cb);
}

//video capture loop(by video file)
void video_capture_loop(const std::string& video_file, unsigned char terminate_key, video_capture_cb_type video_capture_cb)
{
    auto key_press_cb = [&terminate_key](unsigned char key_code, bool& terminate_loop)
    {
        if(key_code == terminate_key)
            terminate_loop = true;
    };
    video_capture_loop(video_file, key_press_cb, video_capture_cb);
}
