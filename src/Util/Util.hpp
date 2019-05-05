#ifndef UTIL_HPP
#define UTIL_HPP

/* Check if input is in valid range from 0 to an upper range. */
inline bool valid_input(int selection_index, int upper_range)
{
    bool valid = (selection_index >= 0 && selection_index < upper_range);
    return valid;
}

#endif
