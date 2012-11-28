#ifndef NUMBER_UTIL
#define NUMBER_UTIL

#include<math.h>

#define AS_DEG(radian) (radian * (180.0f / M_PI))
#define AS_RAD(degree) (degree * (M_PI / 180.0f))

/** Clamps a value to within a given range.
  * @param value A reference to the value to clamp.
  * @param min value is set to min if it is less than min.
  * @param max value is set to max if it is greater than max.
  */
template<class T>
void clamp(T &value, T min, T max)
{
    // Ensure the min value is less than the max value
    if(min > max)
    {
        int temp = min;
        min = max;
        max = temp;
    }

    if(value < min)
        value = min;
    else if(value > max)
        value = max;
}

/** Checks if a value is withing a certain range.
  * @param value The value to check.
  * @param min The minimum value of the range.
  * @param max The maximum value of the range.
  * @return True if min <= value <= max, false otherwise.
  */
bool valueInRange(int value, int min, int max);

#endif // NUMBER_UTIL
