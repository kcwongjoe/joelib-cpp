#pragma once
#ifndef JW_OPENCV_UTILS_H
#define JW_OPENCV_UTILS_H

//************Content************

#include <string>
#include <math_utils.h>
#include <general_utils.h>

#include <opencv2/opencv.hpp>

namespace OpenCVUtils
{
	/**
	 * @brief List of Contour
	 */
	typedef std::vector<std::vector<cv::Point>> contours;

	/**
	 * @brief Contour, List of Points
	 */
	typedef std::vector<cv::Point> contour;

	void saveImage(std::string name, cv::Mat mat);
	std::string type2str(int type);
	void acquireRGB(cv::Mat image, std::vector<uchar>* r, std::vector<uchar>* g, std::vector<uchar>* b, bool ignoreBlackColor = true, bool isBGR = true);

	// Contours
	void removeContourTouchBoundary(contours* inputContours, int imageWidth, int imageHeight);
	int getBiggestContourIndex(contours searchContours);
	void removeSpike(contour* processContour, double maxSpikeDistance, double maxSpikeAngle);

	// Points
	double calculateAngle(cv::Point point1, cv::Point point2, cv::Point point3);
	double calculateDistance(cv::Point point1, cv::Point point2);
}


//*******************************

#endif