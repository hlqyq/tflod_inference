//**********************************************************************
// Video capture utility functions
// usage:
//
//      auto camera_index_or_video_file = 0; // "James.mp4"
//      video_capture_loop(camera_index_or_video_file, VK_ESCAPE, [](bool success, video_frame_type& frame, std::string& result_error)
//      {
//          if(!success)
//              cv::imshow(frame);
//          else
//              std::cerr << result_error << std::endl;
//      }]);
//
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-08		hlq		    generate            66536382@qq.com
//*********************************************************************

#ifndef VIDEO_CAPTURE_UTILS_H
#define VIDEO_CAPTURE_UTILS_H

#ifndef VK_ESCAPE
    #define VK_ESCAPE 0x1B
#endif

#include <string>
#include <functional>

namespace cv{class Mat;}
typedef cv::Mat video_frame_type;
typedef std::function<void(bool success, video_frame_type& frame, std::string& result_error)> video_capture_cb_type;
typedef std::function<void(unsigned char key_code, bool& terminate_loop)> key_press_cb_type;

//video capture loop(by camera index)
void video_capture_loop(unsigned int camera_index, unsigned char terminate_key, video_capture_cb_type video_capture_cb);

//video capture loop(by video file)
void video_capture_loop(const std::string& video_file, unsigned char terminate_key, video_capture_cb_type video_capture_cb);

//video capture loop(by camera index)
void video_capture_loop(unsigned int camera_index, key_press_cb_type key_press_cb, video_capture_cb_type video_capture_cb);

//video capture loop(by video file)
void video_capture_loop(const std::string& video_file, key_press_cb_type key_press_cb, video_capture_cb_type video_capture_cb);

#endif  //VIDEO_CAPTURE_UTILS_H
