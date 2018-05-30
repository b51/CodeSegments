#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>

static cv::Mat matInit;
static cv::Mat matSrc;
static cv::Mat matDst;

static cv::Point   ptOrigin    = cv::Point(-1, -1);
static cv::Point   ptCurrent   = cv::Point(-1, -1);

static std::string windowName = "choose_line";
static bool first_set = false;
static bool second_set = false;

void onMouse(int event, int x, int y, int flags, void *param)
{
    if (matSrc.data == NULL)
      return;

    if (x > matSrc.cols || y > matSrc.rows)
      return;

    int     fontFace    = CV_FONT_HERSHEY_COMPLEX_SMALL;
    double  fontScale   = 1;
    int     thickness   = 1;
    int     lineType    = CV_AA;

    char    strTemp[16];

    if (event == CV_EVENT_LBUTTONDOWN)  {

        matInit.copyTo(matSrc);
        matInit.copyTo(matDst);

        sprintf(strTemp, "(%d,%d)",x,y);
        ptOrigin = cv::Point(x, y);

        cv::putText(matSrc, strTemp, ptOrigin, fontFace, fontScale,
            cv::Scalar(0, 0, 0));
        cv::circle(matSrc, ptOrigin, 3, cv::Scalar(0xFF, 0, 0),
            CV_FILLED, lineType);

        cv::imshow(windowName, matSrc);
        matSrc.copyTo(matDst);
        first_set = true;

    }else if (event == CV_EVENT_MOUSEMOVE &&
        (flags & CV_EVENT_FLAG_LBUTTON))    {

            matDst.copyTo(matSrc);

            sprintf(strTemp, "(%d,%d)",x,y);
            ptCurrent = cv::Point(x, y);

            cv::putText(matSrc, strTemp, ptCurrent, fontFace, fontScale,
                cv::Scalar(0, 0, 0));
            cv::line(matSrc, ptOrigin, ptCurrent, cv::Scalar(0, 0xFF, 0),
                thickness, lineType);
            cv::imshow(windowName, matSrc);

    }else if (event == CV_EVENT_LBUTTONUP)  {

        sprintf(strTemp, "(%d,%d)",x,y);
        ptCurrent = cv::Point(x, y);

        cv::putText(matSrc, strTemp, ptCurrent, fontFace, fontScale,
            cv::Scalar(0, 0, 0));
        cv::circle(matSrc, ptCurrent, 3, cv::Scalar(0xFF, 0, 0),
            CV_FILLED, lineType);
        cv::line(matSrc, ptOrigin, ptCurrent, cv::Scalar(0, 0xFF, 0),
            thickness, lineType);
        cv::imshow(windowName, matSrc);
        second_set = true;

    }

}

void get_line_points(const cv::Mat & img, cv::Point & start, cv::Point & end)
{
    matInit = img.clone();
    matSrc = matInit.clone();
    matDst = matInit.clone();
    cv::namedWindow(windowName);
    cv::setMouseCallback(windowName, onMouse, NULL);
    cv::imshow(windowName, matSrc);

    bool success = false;
    while(!success) {
        first_set = false;
        second_set = false;


        while(!second_set) cv::waitKey(10);

        if(ptOrigin.x == ptCurrent.x && ptOrigin.y == ptCurrent.y) success = false;
        else success = true;
    }
    cv::setMouseCallback(windowName, NULL);
    cv::destroyWindow(windowName);
    matInit.release();
    matSrc.release();
    matDst.release();

    start = ptOrigin;
    end = ptCurrent;
}

void onRectMouse(int event, int x, int y, int flags, void *param)
{
    if (matSrc.data == NULL)
      return;

    if (x > matSrc.cols || y > matSrc.rows)
      return;

    int     fontFace    = CV_FONT_HERSHEY_COMPLEX_SMALL;
    double  fontScale   = 1;
    int     thickness   = 1;
    int     lineType    = CV_AA;

    char    strTemp[16];

    if (event == CV_EVENT_LBUTTONDOWN)  {

        matInit.copyTo(matSrc);
        matInit.copyTo(matDst);

        sprintf(strTemp, "(%d,%d)",x,y);
        ptOrigin = cv::Point(x, y);

        cv::putText(matSrc, strTemp, ptOrigin, fontFace, fontScale,
            cv::Scalar(0, 0, 0));
        cv::circle(matSrc, ptOrigin, 3, cv::Scalar(0xFF, 0, 0),
            CV_FILLED, lineType);

        cv::imshow(windowName, matSrc);
        matSrc.copyTo(matDst);
        first_set = true;

    }else if (event == CV_EVENT_MOUSEMOVE &&
        (flags & CV_EVENT_FLAG_LBUTTON))    {

            matDst.copyTo(matSrc);

            sprintf(strTemp, "(%d,%d)",x,y);
            ptCurrent = cv::Point(x, y);

            cv::putText(matSrc, strTemp, ptCurrent, fontFace, fontScale,
                cv::Scalar(0, 0, 0));
            cv::rectangle(matSrc, ptOrigin, ptCurrent, cv::Scalar(0, 0xFF, 0),
                thickness, lineType);
            cv::imshow(windowName, matSrc);

    }else if (event == CV_EVENT_LBUTTONUP)  {

        sprintf(strTemp, "(%d,%d)",x,y);
        ptCurrent = cv::Point(x, y);

        cv::putText(matSrc, strTemp, ptCurrent, fontFace, fontScale,
            cv::Scalar(0, 0, 0));
        cv::circle(matSrc, ptCurrent, 3, cv::Scalar(0xFF, 0, 0),
            CV_FILLED, lineType);
        cv::rectangle(matSrc, ptOrigin, ptCurrent, cv::Scalar(0, 0xFF, 0),
            thickness, lineType);
        cv::imshow(windowName, matSrc);
        second_set = true;
    }
}

void get_rect_points(const cv::Mat& img, cv::Point& leftUp, cv::Point& rightDown)
{
  matInit = img.clone();
  matSrc = matInit.clone();
  matDst = matInit.clone();
  cv::namedWindow(windowName);
  cv::setMouseCallback(windowName, onRectMouse, NULL);
  cv::imshow(windowName, matSrc);

  bool success = false;
  while(!success) {
      first_set = false;
      second_set = false;

      while(!second_set) cv::waitKey(10);

      if(ptOrigin.x == ptCurrent.x && ptOrigin.y == ptCurrent.y) success = false;
      else success = true;
  }
  cv::setMouseCallback(windowName, NULL);
  cv::destroyWindow(windowName);
  matInit.release();
  matSrc.release();
  matDst.release();

  leftUp = ptOrigin;
  rightDown = ptCurrent;
}
