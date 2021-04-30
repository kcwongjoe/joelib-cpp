#include <color_utils.h>

namespace Utils
{
	namespace   // anonymous namespace for private function
    {
		/**
		 * @brief Private function for hsl2rgb()
		 * 
		 * @param q1 q1
		 * @param q2 q2
		 * @param hue hue
		 * @return double 
		 * @date 2021-04-21
		 */
		double qqhToRGBValue(double q1, double q2, double hue)
        {
            if (hue > 360) hue -= 360;
            else if (hue < 0) hue += 360;

            if (hue < 60) return q1 + (q2 - q1) * hue / 60;
            if (hue < 180) return q2;
            if (hue < 240) return q1 + (q2 - q1) * (240 - hue) / 60;
            return q1;
        }
	}

	/**
	 * @brief Convert RGB to HSL. 
     * Reference: http://csharphelper.com/blog/2016/08/convert-between-rgb-and-hls-color-models-in-c/
	 * 
	 * @param[in] r Red
	 * @param[in] g Green
	 * @param[in] b Blue
	 * @param[out] h Hue
	 * @param[out] s Saturation
	 * @param[out] l Lightness
	 * @date 2021-04-21
	 */
	void rgb2hsl(unsigned char r, unsigned char g, unsigned char b, double* h, double* s, double* l) {

		// Convert RGB to a 0.0 to 1.0 range.
		double double_r = r / 255.0;
		double double_g = g / 255.0;
		double double_b = b / 255.0;

		// Get the maximum and minimum RGB components.
		double max = double_r;
		if (max < double_g) max = double_g;
		if (max < double_b) max = double_b;

		double min = double_r;
		if (min > double_g) min = double_g;
		if (min > double_b) min = double_b;

		double diff = max - min;
		*l = (max + min) / 2;
		if (abs(diff) < 0.00001)
		{
			*s = 0;
			*h = 0;  // H is really undefined.
		}
		else
		{
			if (*l <= 0.5) *s = diff / (max + min);
			else *s = diff / (2 - max - min);

			double r_dist = (max - double_r) / diff;
			double g_dist = (max - double_g) / diff;
			double b_dist = (max - double_b) / diff;

			if (double_r == max) *h = b_dist - g_dist;
			else if (double_g == max) *h = 2 + r_dist - b_dist;
			else *h = 4 + g_dist - r_dist;

			*h = *h * 60;
			if (*h < 0) *h += 360;
		}
	}

	/**
	 * @brief Convert HSL to RGB
	 * 
	 * Reference: http://csharphelper.com/blog/2016/08/convert-between-rgb-and-hls-color-models-in-c/
	 * 
	 * @param[in] h Hue
	 * @param[in] s Saturation
	 * @param[in] l Lightness
	 * @param[out] r Red
	 * @param[out] g Green
	 * @param[out] b Blue
	 * @date 2021-04-21
	 */
	void hsl2rgb(double h, double s, double l, unsigned char* r, unsigned char* g, unsigned char* b) {

			// Calculate p2
			double p2;
            if (l <= 0.5) p2 = l * (1 + s);
            else p2 = l + s - l * s;

			// Calculate p1
            double p1 = 2 * l - p2;

			// Convert
            double double_r, double_g, double_b;
            if (s == 0)
            {
                double_r = l;
                double_g = l;
                double_b = l;
            }
            else
            {
                double_r = qqhToRGBValue(p1, p2, h + 120);
                double_g = qqhToRGBValue(p1, p2, h);
                double_b = qqhToRGBValue(p1, p2, h - 120);
            }

            // Convert RGB to the 0 to 255 range.
            *r = (int)(double_r * 255.0);
            *g = (int)(double_g * 255.0);
            *b = (int)(double_b * 255.0);
	}


}