#ifndef NUMBER_UTIL
#define NUMBER_UTIL

/** Constrains a value to be withing a specified range.
    If value < min then value = min
    If value > max then value = max
    otherwise value is returned.*/
void clamp(int &value, int min, int max);

/** Checks if a value is within a specified range.
    Returns true if min <= value <= max. */
bool valueInRange(int value, int min, int max);

#endif // NUMBER_UTIL
