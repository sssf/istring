#include "istring.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>




#define SIZE_OF_SIZE sizeof(uint32_t)
#define START(ptr)  (ptr-SIZE_OF_SIZE)
#define STRING(ptr) (ptr+SIZE_OF_SIZE)
//#define STRING_SIZE(str) (*(uint32_t*)START(str))

typedef char istring;

void istring_set_length(const istring *istr, uint32_t length) {
    *(uint32_t*)START(istr) = length;
}
uint32_t istring_get_length(const istring *istr) {
    return *(uint32_t*)START(istr);
}

/*
 * Returns a new istring instance created from supplied
 * string. Returns NULL if out of memory. Returns NULL string if
 * argument str is NULL.
 */
char *istring_mk(const char* str) {
    if (str == NULL) {
        return NULL;
    }
    size_t size = strlen(str);
    // TODO: add NULL AT END
    // allocate memory and clear it
    char *istr = malloc(SIZE_OF_SIZE + (size*sizeof(char)));
    memset(istr, 0, SIZE_OF_SIZE + (size*sizeof(char)));

    if (istr == NULL) {
        return NULL;
    }
    istr = STRING(istr);

    istring_set_length(istr, size);
    strcpy(istr, str);

    return istr;
}

/*
 * Deallocates the supplied istring.
 */
void istring_rm(char *istr) {
    if (istr == NULL) {
        return;
    }
    free(START(istr));
}

/*
 * Returns a standard null terminated char* representation of the
 * supplied istring. Returns NULL if out of memory.
 */
char *istring_to_string(const char *istr) {
    if (istr == NULL) {
        return NULL;
    }

    size_t size = (istring_get_length(istr) + 1) * sizeof(char);

    char *cstr = malloc(size);

    if (cstr == NULL) {
        return NULL;
    }

    memset(cstr, 0, size);
    strcpy(cstr, istr);

    return cstr;
}

/*
 * Inspects the length of the string from str and if it differs from
 * length, updates length accordingly. If str does not contain a \0
 * token, it will be inserted at the first element which is not a
 * printable ascii character, and length updated accordingly. This
 * function must be as efficient as possible and not rescan the string
 * if it can be avoided.
 *
 * This function is useful when istrings have been manipulated as
 * regular C strings, to reestablish the length invariant.
 */
size_t istrfixlen(char *istr) {
    if (istr == NULL) {
        return -1;
    }

    uint32_t length = istring_get_length(istr);




    for (int n=0; n<length; ++n) {
        if (istr[n] == '\0') {
            istring_set_length(istr, n);
            return;
        }
    }

}




