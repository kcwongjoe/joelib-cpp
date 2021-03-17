#include "math_utils.h"

namespace Utils
{
	/**
	 * @brief Convert radius to degree
	 * @param radius Radius value
	 * @return Return degree value
     * @date 2021-03-17
	*/
	double toDegree(double radius)
	{
		return radius * 180.0 / PI;
	}

	/**
	 * @brief Convert degree to radius
	 * 
	 * @param degree Degree value
	 * @return Return radius value
     * @date 2021-03-17
	 */
	double toRadius(double degree)
	{
		return degree * PI / 180.0;
	}
}