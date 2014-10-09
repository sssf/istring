#include "istring.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>



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

    // dont forget the '\0'
    size_t length = strlen(str);
    size_t alloc_size = length + 1;

    // allocate memory and clear it
    char *istr = malloc(SIZE_OF_SIZE + (alloc_size*sizeof(char)));
    memset(istr, 0, SIZE_OF_SIZE + (alloc_size*sizeof(char)));

    if (istr == NULL) {
        return NULL;
    }

    istr = STRING(istr);

    istring_set_length(istr, length);
    strcpy(istr, str);
    istr[length] = '\0';

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
// TODO: add tests for this!!!
size_t istrfixlen(char *istr) {

    assert(istr != NULL);

    // TODO: indices might be wrong here!
    int length = istring_get_length(istr);
    int first_non_printable = length;

    for (int n=0; n<length; ++n) {

        if (istr[n] < (char)32 && n < first_non_printable) {
            //printf("\nfirst non printable:%d\n", n);
            first_non_printable = n;
        }
        if (istr[n] == '\0') {
            //puts("found");
            istring_set_length(istr, n);
            return n;
        }
    }

    istr[first_non_printable] = '\0';
    istring_set_length(istr, first_non_printable);

    return first_non_printable;
}


/*
 * Sets the length of an istring and inserts a corresponding '\0'
 * character. If the length argument is out of bounds for the string,
 * a new istring should be returned with the contents of the original
 * string. The last character of the original string will be repeated
 * to fill the string to its given length.
 * NOTE: if istr is NULL return NULL
 */
char* istrslen(char *istr, size_t length) {

    if (istr == NULL) {
        return NULL;
    }

    size_t old_length = istring_get_length(istr);

    // new length <= old length
    if (length <= old_length) {
        istring_set_length(istr, length);
        istr[length] = '\0';
        return istr;
    }

    // new length > old length
    char new_string[length+1];
    for (int n=0; n < length; ++n) {
        if (n < old_length) {
            new_string[n] = istr[n];
        }
        else {
            new_string[n] = istr[old_length-1];
        }
    }
    // NULL terminate the string
    new_string[length] = '\0';

    return istring_mk(new_string);
}

/*
 * For definitions, see the manual page of respective function on the
 * solaris system and the assignment text.
 *
 * Think about whether having O(1) access to the length can be used to
 * improve the implementations if the string.h equivalents and use
 * that to guide your implementations!
 */
char *istrchr(const char *istr, int c) {
    assert(istr != NULL);

    int n = 0;
    int length = istring_get_length(istr);
    while (n <= length) {
        if (istr[n] == c) {
            return (char*)&istr[n];
        }
        ++n;
    }
    return NULL;
}
// TODO: YES O(1) access to the length helps
char *istrrchr(const char *istr, int c) {
    assert(istr != NULL);

    int n = istring_get_length(istr);
    while (n >= 0) {
        if (istr[n] == c) {
            return (char*)&istr[n];
        }
        --n;
    }
    return NULL;
}

// THE O(1) access to length doesn't help here
int istrcmp(const char *istr1, const char *istr2) {
    assert(istr1 != NULL);
    assert(istr2 != NULL);

    return strcmp(istr1, istr2);
}

int istrncmp(const char *istr1, const char *istr2, size_t n) {
    assert(istr1 != NULL);
    assert(istr2 != NULL);

    return strncmp(istr1, istr2, n);
}

/* TODO: fix documentation
 * istrlen - calculate the length of a string
 * The function calculates the length of the string istr, excluding the terminating null byte ('\0').
 * returns the number of bytes in the string istr.
 */
size_t istrlen(const char *istr) {
    assert(istr != NULL);
    return istring_get_length(istr);
}

/*
 * I nedanstående funktioner är dst en pekare till en vanlig
 * sträng och inte en istring. Däremot skall minnesutrymmet
 * "konverteras" till en istring av funktionerna, d.v.s. efter att
 * t.ex. istrcpy anropats bör man vid anropsplatsen göra dst =
 * STRING(dst) för att hoppa över längd-delen av strängen.
 */
char *istrcpy(char *dst, const char *src) {

    char *istr = STRING(dst);
    strcpy(istr, src);
    istring_set_length(istr, strlen(src));
    //istrfixlen(istr); // NOTE: didn't work because istrfixlen used the strings length witch was 0;

    return istr;
}

char *istrncpy(char *dst, const char *src, size_t n) {

    char *istr = STRING(dst);
    strncpy(istr, src, n);
    istring_set_length(istr, strlen(src));

    return istr;
}


char *istrcat(char *dst, const char *src) { return NULL; }
char *istrncat(char *dst, const char *src, size_t n) { return NULL; }





// TODO: #undef our macros
