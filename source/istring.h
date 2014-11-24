#ifndef __ISTRING_H__
#define __ISTRING_H__

#include <string.h>

/**
 * @brief Create new istring instance
 *
 * The caller is responsible for deallocate the returned istring using the istring_rm function
 *
 * @param *cstring standard c string to create istring from
 * @return new istring instance created from supplied
 *         string. Returns NULL if out of memory.
 *         Returns NULL string if argument str is NULL.
 */
char *istring_mk(const char* cstring);

/**
 * @brief Deallocates the supplied istring
 *
 * @param *string istring to deallocate
 */
void istring_rm(char *string);

/**
 * @brief Create standard c string from istring
 *
 * @param *string to create c string from
 * @return Returns a standard null terminated char* representation of the
 *         supplied istring. Returns NULL if out of memory.
 */
char *istring_to_string(const char *string);

/**
 * @brief Fix length of istring
 *
 * Inspects the length of the string from string and if it differs from
 * length, updates length accordingly. If string does not contain a \0
 * token, it will be inserted at the first element which is not a
 * printable ascii character, and length updated accordingly. This
 * function must be as efficient as possible and not rescan the string
 * if it can be avoided.
 *
 * This function is useful when istrings have been manipulated as
 * regular C strings, to reestablish the length invariant.
 *
 * @param *string fix the length of this istring
 * @return The new updated length of istring
 *
 * Returns a standard null terminated char* representation of the
 *         supplied istring. Returns NULL if out of memory.
 */
size_t istrfixlen(char *string);


/**
 * @brief Set the length of an istring.
 *
 * Sets the length of an istring and inserts a corresponding '\0'
 * character. If the length argument is out of bounds for the string,
 * a new istring should be returned with the contents of the original
 * string. The last character of the original string will be repeated
 * to fill the string to its given length.
 *
 * @param **string pointer-pointer to the string to set a new length on.
 * @param length new length to be set
 *
 * @return string with length updated to length
 */
char* istrslen(char **string, size_t length);

/*
 * For definitions, see the manual page of respective function on the
 * solaris system and the assignment text.
 *
 * Think about whether having O(1) access to the length can be used to
 * improve the implementations if the string.h equivalents and use
 * that to guide your implementations!
 */


/**
 * @brief Returns a pointer to the first occurrence of the character c in string.
 *
 * @param *string istring to search in
 * @param c character to search for
 *
 * @return A pointer to the first occurrence of the character c in string.
 */
char *istrchr(const char *string, int c);

/**
 * @brief Returns a pointer to the last occurrence of the character c in string.
 *
 * @param *string istring to search in
 * @param c character to search for
 *
 * @return A pointer to the last occurrence of the character c in string.
 */
char *istrrchr(const char *string, int c);

/**
 * @brief Compares two istrings
 *
 * @param *string1 istring
 * @param *string2 istring
 *
 * @return An integer less than, equal to, or greater than zero if string1 is found,
 *         respectively, to beless than, to match, or be greater than string2.
 */
int istrcmp(const char *string1, const char *string2);

/**
 * @brief Compares two istrings
 *
 * @param *string1 istring
 * @param *string2 istring
 * @param n max number of bytes to compare
 *
 * @return An integer less than, equal to, or greater than zero if string1 is found,
 *         respectively, to beless than, to match, or be greater than string2.
 *         It compares the only first (at most) n bytes of string1 and string2.
 */
int istrncmp(const char *string1, const char *string2, size_t n);

/**
 * @brief Length of a istring
 *
 * @param *string istring to get length of
 * @return length of supplied istring excluding the null byte
 */
size_t istrlen(const char *string);


// TODO: make destination into a double pointer!

/**
 * @brief Copy a istring
 *
 * The strings may not overlap, destination must be null terminated. Beware of buffer overruns!
 *
 * @param *destination block of memory large enough to receive the copy + 4 bytes  (breaks the encapsulation)
 * @param *source istring to create copy. Must be null terminated.
 * @return destination converted to istring copy of source.
 */
char *istrcpy(char **destination, const char *source);

/**
 * @brief Copy a istring
 *
 * The strings may not overlap. Beware of buffer overruns! 
 * @warning If there is no null byte amoung the first n bytes of source, the string placed in dest will not be null-terminated.
 *
 * @param *destination block of memory large enough to receive the copy + 4 bytes  (breaks the encapsulation)
 * @param *source istring to create copy. Must be null terminated.
 * @param n number of bytes to copy from source
 * @return destination converted to istring with at most n bytes copied from source into it.
 */
char *istrncpy(char **destination, const char *source, size_t n);

/**
 * @brief Concatenate two strings
 *
 * The strings may not overlap, source and destination must be null terminated. Beware of buffer overruns!
 *
 * @param *destination block of memory large enough to receive the result + 4 bytes  (breaks the encapsulation)
 * @param *source istring to append to destination
 * @return destination converted to istring with source appended to it
 */
char *istrcat(char **destination, const char *source);

/**
 * @brief Concatenate two strings
 *
 * The strings may not overlap, destination must be null terminated.
 * @warning If destination is not large enough, the program behavior is unpredictable.
 *
 * @param *destination block of memory large enough to receive the result + 4 bytes  (breaks the encapsulation).
 * @param *source istring to append to destination.
 * @param n number of bytes to copy from source
 * @return destination converted to istring with at most n bytes of source appended to it.
 */
char *istrncat(char **destination, const char *source, size_t n);

#endif // __ISTRING_H__

