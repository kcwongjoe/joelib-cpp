#pragma once
#ifndef JW_QT_UTILS_H
#define JW_QT_UTILS_H

// If OpenCV is not used, comment the following lines
#ifndef HAVE_OPENCV
#define HAVE_OPENCV
#endif


//************Content************
#include <QMainWindow>

// ------Utils for OpenCV and QT------
#ifdef HAVE_OPENCV
#include <opencv2/opencv.hpp>

// Convert Mat to QImage
QImage MatToQImage(const cv::Mat& mat);

#endif
// ------------------------------------

//*******************************

#endif