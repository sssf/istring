#ifndef __ISTRING_REP_H__
#define __ISTRING_REP_H__

#include <stdint.h>

/**
 * \brief Data representation for istrings
 */
typedef struct IString {
    uint32_t length;    /**< length of the istring */
    char string[];      /**< C99 flexable arrays for holding the string data */
} IString;


#define START(string)   ((IString*)(((uint32_t*)string) - 1))
#define STRING(istring) ((char*)(((uint32_t*)istring) + 1))


#endif // __ISTRING_REP_H__
