//**********************************************************************
// opencv utility functions
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-08		hlq		    generate            66536382@qq.com
//*********************************************************************

#ifndef CV_UTILS_H
#define CV_UTILS_H

#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>

typedef cv::Mat                         cv_frame_type;
typedef class cv::Scalar_<double>       cv_color_type;

// construct color
cv_color_type cv_color(unsigned char red, unsigned char green, unsigned char blue);

// construct color
cv_color_type cv_color_from_string(const std::string& color);

// get image size
void cv_image_get_size(cv_frame_type& frame, int& result_width, int& result_height);

// image resize
void cv_image_resize(cv_frame_type& frame, int width, int height, cv_frame_type& result_frame);

// draw text
void cv_image_draw_text(cv_frame_type& frame, const std::string& text, int x, int y, const cv_color_type& color, double font_scale = 1);

// draw rectangle
void cv_image_draw_rectangle(cv_frame_type& frame, int left, int top, int right, int bottom, const cv_color_type& color);

// show image
void cv_image_show(const std::string& title, cv_frame_type& frame);

// convert image type to float
void cv_image_to_float(const cv_frame_type& frame, cv_frame_type& float_image);

// convert image BGR to RGB
void cv_image_bgr_to_rgb(const cv_frame_type& frame, cv_frame_type& float_image);

// image normalize
void cv_image_normalize(cv_frame_type& float_image);

#endif  //CV_UTILS_H
