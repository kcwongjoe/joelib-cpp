#pragma once
#ifndef JW_MATH_UTILS_H
#define JW_MATH_UTILS_H

//************Content************
#include <vector>
#include <math.h>

constexpr double PI = 3.1415926535897932384626433;
namespace Utils
{
	
#ifndef JW_UTILS_IS_NUMERICAL_H
#define JW_UTILS_IS_NUMERICAL_H
	/**
	 * @brief Check whether typename is numerical type
	 * @tparam T Type to be checked.
     * @date 2021-04-09
	*/
	template <typename T>
	constexpr bool is_numerical = !std::is_same_v<T, bool> && std::is_arithmetic_v<T>;
#endif

	/**
	 * @brief Average
	 * 
     * @code{.cpp}
     * std::vector<int> values;
	 * double result = average<double>(values);
     * @endcode
	 *
	 * @tparam R Return Numerical type.
	 * @tparam T Input Numerical type.
	 * @param values Values to be averaged.
	 * @return T Return the average value.
     * @date 2021-03-17
	 */
	template <typename R, typename T>
	R average(std::vector<T> values)
	{
		// Exception
		if constexpr (!is_numerical<R> || !is_numerical<T>)
			throw "This funciton only support numerical type.";

		// Get Size
		size_t size = values.size();
		if (size == 0) return 0;

		// Average
		double average = 0.0;
		for (int i = 0; i < size; i++)
		{
			average += values[i];
		}
		average = average / (double)size;

		return static_cast<R>(average);
	};

	/**
	 * @brief Standard deviation
	 *
     * @code{.cpp}
     * std::vector<int> values;
	 * double result = stdev<double>(values);
     * @endcode
	 * 
	 * @tparam R Return Numerical type.
	 * @tparam T Input Numerical type.
	 * @param values Values to be applied Standard deviation.
	 * @return T Return the standard deviation value.
     * @date 2021-03-17
	 */
	template <typename R, typename T>
	R stdev(std::vector<T> values)
	{
		// Exception
		if constexpr (!is_numerical<R> || !is_numerical<T>)
			throw "This funciton only support numerical type.";

		// Get size
		size_t size = values.size();
		if (size == 0) return 0;

		// Calculate average
		double mean = average<double>(values);

		// Calculate stdev
		double sum = 0;
		for (int i = 0; i < size; i++)
		{
			double diff = (values[i] - mean);
			sum += diff * diff;
		}

		double stdev = std::sqrt(sum / (size - 1));

		return static_cast<R>(stdev);
	};

	/**
	 * @brief Median
	 *
     * @code{.cpp}
     * std::vector<int> values;
	 * double result = median<double>(values);
     * @endcode
	 * 
	 * @tparam R Return Numerical type.
	 * @tparam T Input Numerical type.
	 * @param values Values to be calculated median.
	 * @return T Return median
     * @date 2021-03-17
	 */
	template <typename T>
	T median(std::vector<T> values)
	{
		// Exception
		if constexpr (!is_numerical<T>)
			throw "This funciton only support numerical type.";

		// Get size
		size_t size = values.size();
		if (size == 0) return 0;  // Undefined, really.

		// Calculate
		sort(values.begin(), values.end());
		if (size % 2 == 0)
		{
			return (values[size / 2 - 1] + values[size / 2]) / 2;
		}
		else
		{
			return values[size / 2];
		}
	};

	// Math Operator

	/**
	 * @brief Values1 + Values2.
	 * 
     * @code{.cpp}
     * std::vector<int> values1;
     * std::vector<long> values2;
	 * double result = addition<double>(values1, values2);
     * @endcode
	 * 
	 * @tparam R Return Numerical type.
	 * @tparam T1 Input Numerical type 1.
	 * @tparam T2 Input Numerical type 2.
	 * @param[in] values1 Values 1
	 * @param[in] values2 Values 2
	 * @return Return Values1 + Values2.
     * @date 2021-03-17
	*/
	template <typename R, typename T1, typename T2>
	std::vector<R> addition(std::vector<T1> values1, std::vector<T2> values2)
	{
		// Exception
		if constexpr (!is_numerical<R> || !is_numerical<T1> || !is_numerical<T2>)
			throw "This funciton only support numerical type.";

		// Get size
		size_t size = values1.size();
		if (values1.size() != values2.size()) return std::vector<R>();

		// Calculate
		std::vector<R> result(size);
		for (int i = 0; i < size; i++)
		{
			// Addition
			result[i] = static_cast<R>((double)values1[i] + (double)values2[i]);
		}

		return result;
	}

	/**
	 * @brief Values1 - Values2.
	 * 
     * @code{.cpp}
     * std::vector<int> values1;
     * std::vector<long> values2;
	 * double result = subtraction<double>(values1, values2);
     * @endcode
	 * 
	 * @tparam R Return Numerical type.
	 * @tparam T1 Input Numerical type 1.
	 * @tparam T2 Input Numerical type 2.
	 * @param[in] values1 Values 1
	 * @param[in] values2 Values 2
	 * @return Return Values1 - Values2.
     * @date 2021-03-17
	*/
	template <typename R, typename T1, typename T2 >
	std::vector<R> subtraction(std::vector<T1> values1, std::vector<T2> values2)
	{
		// Exception
		if constexpr (!is_numerical<R> || !is_numerical<T1> || !is_numerical<T2>)
			throw "This funciton only support numerical type.";

		// Get size
		size_t size = values1.size();
		if (values1.size() != values2.size()) return std::vector<R>();

		// Calculate
		std::vector<R> result(size);
		for (int i = 0; i < size; i++)
		{
			// Subtraction
			result[i] = static_cast<R>((double)values1[i] - (double)values2[i]);
		}

		return result;
	}

	/**
	 * @brief Values1 * Values2.
	 * 
     * @code{.cpp}
     * std::vector<int> values1;
     * std::vector<long> values2;
	 * double result = multiple<double>(values1, values2);
     * @endcode
	 * 
	 * @tparam R Return Numerical type.
	 * @tparam T1 Input Numerical type 1.
	 * @tparam T2 Input Numerical type 2.
	 * @param[in] values1 Values 1
	 * @param[in] values2 Values 2
	 * @return Return Values1 * Values2.
     * @date 2021-03-17
	*/
	template <typename R, typename T1, typename T2 >
	std::vector<R> multiple(std::vector<T1> values1, std::vector<T2> values2)
	{
		// Exception
		if constexpr (!is_numerical<R> || !is_numerical<T1> || !is_numerical<T2>)
			throw "This funciton only support numerical type.";

		// Get size
		size_t size = values1.size();
		if (values1.size() != values2.size()) return std::vector<R>();

		// Calculate
		std::vector<R> result(size);
		for (int i = 0; i < size; i++)
		{
			// Multiple
			result[i] = static_cast<R>((double)values1[i] * (double)values2[i]);
		}

		return result;
	}

	/**
	 * @brief Values1 / Values2. If Values2[i] == 0, it will be skipped.
	 * 
     * @code{.cpp}
     * std::vector<int> values1;
     * std::vector<long> values2;
	 * double result = divideBy<double>(values1, values2);
     * @endcode
	 * 
	 * @tparam R Return Numerical type.
	 * @tparam T1 Input Numerical type 1.
	 * @tparam T2 Input Numerical type 2.
	 * @param[in] values1 Values 1
	 * @param[in] values2 Values 2
	 * @param[out] zeroIndices (Option) Return the index of values 2 which is 0.
	 * @return Return Values1 / Values2.
     * @date 2021-03-17
	*/
	template <typename R, typename T1, typename T2 >
	std::vector<R> divideBy(std::vector<T1> values1, std::vector<T2> values2, std::vector<int>* zeroIndices = NULL)
	{
		// Exception
		if constexpr (!is_numerical<R> || !is_numerical<T1> || !is_numerical<T2>)
			throw "This funciton only support numerical type.";

		// Get size
		size_t size = values1.size();
		if (values1.size() != values2.size()) return std::vector<R>();
		
		// Calculate
		std::vector<R> result(size);
		size_t pushedCount = 0;
		for (int i = 0; i < size; i++)
		{
			if (values2[i] != 0)
			{
				// Divide
				result[pushedCount] = static_cast<R>((double)values1[i] / (double)values2[i]);

				// Push zero
				if (zeroIndices) zeroIndices->push_back(i);

				// Move data point
				pushedCount++;
			}				
		}

		// Resize
		if (pushedCount != size) result.resize(pushedCount);

		return result;
	}

	// Degree/Radius convertion
	double toDegree(double radius);
	double toRadius(double degree);
}


//*******************************

#endif