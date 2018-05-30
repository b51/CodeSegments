#ifndef CHOOSE_LINE_H_H
#define CHOOSE_LINE_H_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
void get_line_points(const cv::Mat & img, cv::Point & start, cv::Point & end);

void get_rect_points(const cv::Mat& img, cv::Point& leftup, cv::Point& rightdown);

#endif
