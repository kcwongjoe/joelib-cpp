#include <opencv_utils.h>

namespace OpenCVUtils
{
	/**
	 * @brief Save Mat in async mode
	 * @param mat Image to be save
	 * @param name Path of the image
     * @date 2021-03-17
	*/
	void saveImage(std::string name, cv::Mat mat)
	{
		std::thread t([mat, name]() {
			cv::imwrite(name, mat);
			}
		);
		t.detach();
	}

	/**
	 * @brief Convert cv::mat type to string
	 * @param type cv::Mat().type()
	 * @return Return type string
     * @date 2021-03-17
	*/
	std::string type2str(int type) {
		std::string r;

		uchar depth = type & CV_MAT_DEPTH_MASK;
		uchar chans = 1 + (type >> CV_CN_SHIFT);

		switch (depth) {
		case CV_8U:  r = "8U"; break;
		case CV_8S:  r = "8S"; break;
		case CV_16U: r = "16U"; break;
		case CV_16S: r = "16S"; break;
		case CV_32S: r = "32S"; break;
		case CV_32F: r = "32F"; break;
		case CV_64F: r = "64F"; break;
		default:     r = "User"; break;
		}

		r += "C";
		r += (chans + '0');

		return r;
	}

	/**
	 * @brief Acquire RGB values
	 * @param[in] image Image
	 * @param[out] redColor Red colors. It can be NULL.
	 * @param[out] greenColor Green colors. It can be NULL.
	 * @param[out] blueColor Blue colors. It can be NULL.
	 * @param[in] ignoreBlackColor (Option) Ignore black color? Default as true
	 * @param[in] isBGR (Option) Is color foramt as BGR? Default as true. Set as false for RGB format.
     * @date 2021-03-17
	*/
	void acquireRGB(cv::Mat image, std::vector<uchar>* redColor, std::vector<uchar>* greenColor, std::vector<uchar>* blueColor, bool ignoreBlackColor, bool isBGR)
	{
		int imageWidth = image.cols;
		int imageHeight = image.rows;

		// Resize to maximum for speed up
		int maximumBufferSize = imageWidth * imageHeight;

		if (redColor) redColor->resize(maximumBufferSize);
		if (greenColor) greenColor->resize(maximumBufferSize);
		if (blueColor) blueColor->resize(maximumBufferSize);
		int pixelSize = 0;

		for (int y = 0; y < imageHeight; y++)
		{
			for (int x = 0; x < imageWidth; x++)
			{
				// Get pixel color
				uchar r = isBGR ? image.at<cv::Vec3b>(y, x)[2] : image.at<cv::Vec3b>(y, x)[0];
				uchar g = image.at<cv::Vec3b>(y, x)[1];
				uchar b = isBGR ? image.at<cv::Vec3b>(y, x)[0] : image.at<cv::Vec3b>(y, x)[2];

				// Check color, not black color
				bool addColor = true;
				if (ignoreBlackColor && r == 0 && g == 0 && b == 0) addColor = false;

				if (addColor)
				{
					if (redColor) redColor->at(pixelSize) = r;
					if (greenColor) greenColor->at(pixelSize) = g;
					if (blueColor) blueColor->at(pixelSize) = b;

					pixelSize++;
				}
			}
		}

		// Resize to fit size
		if (redColor) redColor->resize(pixelSize);
		if (greenColor) greenColor->resize(pixelSize);
		if (blueColor) blueColor->resize(pixelSize);
	}

	/**
	 * @brief Acquire HSL values
	 * @param[in] image Image
	 * @param[out] hue Hue. It can be NULL.
	 * @param[out] saturation Saturation. It can be NULL.
	 * @param[out] lightness :ightness. It can be NULL.
	 * @param[in] ignoreBlackColor (Option) Ignore black color? Default as true
	 * @param[in] isBGR (Option) Is color foramt as BGR? Default as true. Set as false for RGB format.
     * @date 2021-04-21
	*/
	void acquireHSL(cv::Mat image, std::vector<double>* hue, std::vector<double>* saturation, std::vector<double>* lightness, bool ignoreBlackColor, bool isBGR)
	{
		std::vector<uchar> redColor;
		std::vector<uchar> greenColor;
		std::vector<uchar> blueColor;

		// Get RGB
		acquireRGB(image, &redColor, &greenColor, &blueColor, ignoreBlackColor, isBGR);

		// Resize
		if (hue) hue->resize(redColor.size());
		if (saturation) saturation->resize(redColor.size());
		if (lightness) lightness->resize(redColor.size());

		// Convert to HSL
		for (int i = 0; i < redColor.size();i++) {
			double h;
			double s;
			double l;
			Utils::rgb2hsl(redColor[i], greenColor[i], blueColor[i], &h, &s, &l);
			if (hue) hue->at(i) = h;
			if (saturation) saturation->at(i) = s;
			if (lightness) lightness->at(i) = l;
		}
	}

	/**
	 * @brief Remove contour which touch the image boundary
	 * @param[in, out] processContours Contour list to be processed
	 * @param[in] imageWidth Image width
	 * @param[in] imageHeight Image height
     * @date 2021-03-17
	*/
	void removeContourTouchBoundary(contours* processContours, int imageWidth, int imageHeight)
	{
		std::vector<int> eraseIndex;
		for (int i = 0; i < processContours->size(); i++)
		{
			// Search
			bool touchBoundary = false;
			for (int j = 0; j < processContours->at(i).size(); j++)
			{
				int x = processContours->at(i)[j].x;
				int y = processContours->at(i)[j].y;
				if (x == 0 || x == imageWidth || y == 0 || y == imageHeight)
				{
					touchBoundary = true;
					break;
				}
			}

			// Erase
			if (touchBoundary)
			{
				eraseIndex.push_back(i);
			}
		}

		// Erase
		Utils::removeByIndices(processContours, eraseIndex);
	}

	/**
	 * @brief Get the index of the biggest contour
	 * @param[in] searchContours Contours list to be searched.
	 * @return Return the index of the biggest contour
     * @date 2021-03-17
	*/
	int getBiggestContourIndex(contours searchContours)
	{
		int biggestContourIndex = 0;
		double biggestContourArea = 0;
		for (int i = 0; i < searchContours.size(); i++)
		{
			double area = cv::contourArea(searchContours[i]);
			if (area > biggestContourArea)
			{
				biggestContourIndex = i;
				biggestContourArea = area;
			}
		}

		return biggestContourIndex;
	}

	/**
	 * @brief Remove spike in the contour. Point was identified as spike if the neighbor lines is shorter than maxSpikeDistance and the inner angle is smaller than maxSpikeAngle.
	 * @param[in, out] processContour The contour to be processed
	 * @param[in] maxSpikeDistance The maximum distance of the spike
	 * @param[in] maxSpikeAngle The maximum angle of the spike in degree
     * @date 2021-03-17
	*/
	void removeSpike(contour* processContour, double maxSpikeDistance, double maxSpikeAngle)
	{
		int perviousContourSize = -1;
		while (perviousContourSize != processContour->size())
		{
			// Find remove points
			std::vector<int> removePoint;
			int contourSize = processContour->size();
			for (int i=0;i< contourSize;i++)
			{
				// Get points
				cv::Point prePoint = (i == 0) ? processContour->at((size_t)(contourSize - 1)): processContour->at(i - 1);
				cv::Point currentPoint = processContour->at(i);
				cv::Point nextPoint = (i == contourSize - 1) ? processContour->at(0) : processContour->at((size_t)(i + 1));

				// Calculate distance and angle
				double distance = calculateDistance(prePoint, nextPoint);
				double angle = calculateAngle(prePoint, currentPoint, nextPoint);

				// Check remove point
				if (distance <= maxSpikeDistance && angle > 0 && angle <= maxSpikeAngle)
				{
					removePoint.push_back(i);
				}
			}

			// Remove points
			perviousContourSize = contourSize;
			Utils::removeByIndices(processContour, removePoint);
		}
	}

	/**
	 * @brief Calculate angle of vector1 (point2, point1) and vector2 (point2, point3)
	 * @param[in] point1 Point 1
	 * @param[in] point2 Point 2 
	 * @param[in] point3 Point 3
	 * @return Return angle in degree
     * @date 2021-03-17
	*/
	double calculateAngle(cv::Point point1, cv::Point point2, cv::Point point3)
	{
		cv::Point vector1 = point2 - point1;
		cv::Point vector2 = point2 - point3;

		vector1 /= cv::norm(vector1);
		vector2 /= cv::norm(vector2);

		double dotProduct = vector1.ddot(vector2);
		double angle = Utils::toDegree(acos(dotProduct));

		return angle;
	}

	/**
	 * @brief Calculate distance between two points
	 * @param[in] point1 Point 1
	 * @param[in] point2 Point 2
	 * @return Return distance between two points
     * @date 2021-03-17
	*/
	double calculateDistance(cv::Point point1, cv::Point point2)
	{
		cv::Point vector = point1 - point2;
		double distance = sqrt(vector.x * vector.x + vector.y * vector.y);

		return distance;
	}
}