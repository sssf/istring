#ifndef __ISTRING_REP_H__
#define __ISTRING_REP_H__


typedef struct istring {
    uint32_t length;
    char string[];      // C99 flexable arrays
} istring;


#endif // __ISTRING_REP_H__
