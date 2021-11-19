//**********************************************************************
// opencv utility functions
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-08		hlq		    generate            66536382@qq.com
//*********************************************************************

#include "cv_utils.h"
#include "string_utils.h"

// construct color
cv_color_type cv_color(unsigned char red, unsigned char green, unsigned char blue)
{
    return cv_color_type(blue, green, red);
}

// construct color
cv_color_type cv_color_from_string(const std::string& color)
{
    std::vector<std::string> clr_part_list;
    string_split(color, ",;", clr_part_list);
    unsigned char red = 0, green = 0, blue = 0;
    if(clr_part_list.size() == 3)
    {
        red = static_cast<unsigned char>(std::stoi(clr_part_list[0]));
        green = static_cast<unsigned char>(std::stoi(clr_part_list[1]));
        blue = static_cast<unsigned char>(std::stoi(clr_part_list[2]));
    }
    return cv_color(red, green, blue);
}

// get image size
void cv_image_get_size(cv_frame_type& frame, int& result_width, int& result_height)
{
    result_width = frame.cols;
    result_height = frame.rows;
}

// image resize
void cv_image_resize(cv_frame_type& frame, int width, int height, cv_frame_type& result_frame)
{
    cv::resize(frame, result_frame, cv::Size(width, height));
}

// draw text
void cv_image_draw_text(cv_frame_type& frame, const std::string& text, int x, int y, const cv_color_type& color, double font_scale)
{
    putText(frame, text.c_str(), cv::Point(x, y), cv::FONT_HERSHEY_SIMPLEX, font_scale, color);
}

// draw rectangle
void cv_image_draw_rectangle(cv_frame_type& frame, int left, int top, int right, int bottom, const cv_color_type& color)
{
    cv::Rect rec(left, top, right - left, bottom - top);
    cv::rectangle(frame, rec, color);
}

// show image
void cv_image_show(const std::string& title, cv_frame_type& frame)
{
    cv::imshow(title.c_str(), frame);
}

// convert image type to float
void cv_image_to_float(const cv_frame_type& frame, cv_frame_type& float_image)
{
    frame.convertTo(float_image, CV_32FC3);
}

// convert image BGR to RGB
void cv_image_bgr_to_rgb(const cv_frame_type& frame, cv_frame_type& float_image)
{
    cv::cvtColor(frame, float_image, cv::COLOR_BGR2RGB);
}

// part value of pixel normalize
void cv_pixel_value_normalize(float& pixel_value, float input_std = 255.0, float normalize_min = -1, float normalize_max = 1)
{
    pixel_value = ((pixel_value / input_std) - 0.5) * (normalize_max - normalize_min);
}

// pixel normalize
template<typename PixelType>
void cv_pixel_normalize(PixelType& pixel, float input_std = 255.0, float normalize_min = -1, float normalize_max = 1)
{
    cv_pixel_value_normalize(pixel.x, input_std, normalize_min, normalize_max);
    cv_pixel_value_normalize(pixel.y, input_std, normalize_min, normalize_max);
    cv_pixel_value_normalize(pixel.z, input_std, normalize_min, normalize_max);
}

// image normalize
void cv_image_normalize(cv_frame_type& float_image)
{
    typedef cv::Point3_<float> pixel_type;
    for (auto p = float_image.ptr<pixel_type>(0, 0), p_end = p + float_image.cols * float_image.rows; p != p_end; ++p)
        cv_pixel_normalize(*p);
}
