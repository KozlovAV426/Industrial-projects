#pragma once

#include<iostream>
#include<stdio.h>
#include<algorithm>
#include<vector>
#include<ctype.h>
#include<cassert>

/**
 * @file
 */


/**
 * @brief Structure that allows you to use itself as a string without
 * storing a buffer.
 *
 */

struct OpenedString {
    char* ptr_;
    size_t len_ = 0;
};

void PrintString(const OpenedString* string, FILE* output);

/**
 * @brief Shifts the string pointer to the last character of the string.
 *
 * @param string is a pointer on a string, which pointer we want to shift.
 *
 */

void ShiftToEnd(OpenedString* string);

void ShiftToEndInArray(OpenedString* strings, size_t len);

/**
 * @brief Similar to ShiftToEnd(), but shifts the pointer to the first character.
 *
 * @param string is a pointer on a string, which pointer we want to shift.
 *
 */

void ShiftToBegin(OpenedString* string);

void ShiftToBeginInArray(OpenedString* strings, size_t len);

void PrintBuffer(const char* buffer, size_t size, FILE* output);

/**
 *
 * @brief Sorts strings from input file and writes sorted strings to output file.
 * Sorts from the beginning of lines firstly, then from the end.
 *
 * @param input is a pointer on input file.
 *
 * @param output is a pointer on output file.
 *
 */

void SortStringsInOrder(FILE* input, FILE* output);

/**
 * @brief Writes to file an array of strings.
 *
 * @param strings it is a pointer on an array of strings.
 *
 * @param size is an amount of strings.
 *
 * @param output is a pointer on file to write.
 *
 */

void WriteToFile(const OpenedString* strings, size_t size, FILE* output);

int Cmp(const void* farg, const void* sarg);

/*!
 * @brief Returns the sign of a number
 *
 * @param[in] number a number, which sign we want to know
 *
 * @return 1 or -1
 */
int GetSign(const int& number);

size_t CountStrings(char* buffer, size_t* len);

size_t CountFileSize(FILE* file_ptr);


/**
 * @brief Reads strings from buffer.
 *
 * @param buffer it is a pointer on buffer.
 *
 * @param len is an amount of symbols.
 *
 * @return Array of strings.
 *
 */

OpenedString* ReadStrings(char* buffer, size_t* len);

bool IsLetter(const char* symbol);

/**
 * @brief Reads buffer from file.
 *
 * @param file_ptr is a pointer on a file.
 *
 * @param len is an amount of symbols.
 *
 * @return Array of symbols.
 *
 */

char* ReadBuffer(FILE* file_ptr, size_t* len);
