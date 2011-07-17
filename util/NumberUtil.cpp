#include "NumberUtil.hpp"

void clamp(int &value, int min, int max)
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

bool valueInRange(int value, int min, int max)
{
    return (value >= min) && (value <= max);
}
