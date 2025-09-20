#pragma once

#include "defines.h"
#include "types.h"
#include <string>
#include <string_view>
#include <vector>

EMB_NAMESPACE_START

// strings and stuff
using embStr = std::string;
using embWstr = std::wstring;
using embStrView = std::string_view;

//-------------------------------------------------------------------//
//						  String manipulation						 //
//-------------------------------------------------------------------//

constexpr std::string_view WHITESPACE_CHARS = " \f\n\r\t\v";
/// <summary>
/// Converts widestring into string
/// </summary>
/// <param name="wstr">The string to convert.</param>
/// <param name="encoding">the type of encoding to be used in conversion.</param>
/// <return>converted string</return>
//std::string Str_ConvertWstrToStr(std::wstring_view wstr);

/// <summary>
/// Converts string into widestring
/// </summary>
/// <param name="str">The string to convert.</param>
/// <param name="encoding">the type of encoding to be used in conversion.</param>
/// <return>converted string</return>
//std::wstring Str_ConvertStrToWstr(std::string_view str);

/// <summary>
/// replaces all instances of a certain substring "toReplace" with another substring "replaceWith" in the string "toModify".
/// </summary>
/// <param name="toModify">The string to modify.</param>
/// <param name="toReplace">what sequence of characters that needs to be replaced.</param>
/// <param name="replaceWith">a sequence of characters to be replaced with.</param>
/// <param name="maxReplacements">maximum number of replacements to perform. 0 means unlimited.</param>
/// <param name="startFromBack">Start replacing from the back to front instead of front to back.</param>
/// <return>number of times a replacement happens.</return>
size_t StrReplace(std::string& toModify, std::string_view toReplace, std::string_view replaceWith,
                  size_t maxReplacements = 0, bool startFromBack = false);

/// <summary>
/// replaces all instances of a certain character "toReplace" with another character "replaceWith" in the string "toModify".
/// </summary>
/// <param name="toModify">The string to modify.</param>
/// <param name="toReplace">what character to be replaced.</param>
/// <param name="replaceWith">character to be replaced with.</param>
/// <param name="maxReplacements">maximum number of replacements to perform. 0 means unlimited.</param>
/// <param name="startFromBack">Start iterating from the back to front instead of front to back.</param>
/// <return>number of times a replacement happened.</return>
size_t StrReplace(std::string& toModify, const char toReplace, const char replaceWith,
                  size_t maxReplacements = 0, bool startFromBack = false);

/// <summary>
/// removes all instances of a certain substring "toRemove" from a string.
/// </summary>
/// <param name="toModify">The string to modify.</param>
/// <param name="toRemove">what sequence of characters needs to be removed.</param>
/// <param name="maxRemoves">maximum number of removes to perform. 0 means unlimited.</param>
/// <param name="startFromBack">Start iterating from the back to front instead of front to back.</param>
/// <return>number of times a remove happened.</return>
size_t StrRemove(std::string& toModify, std::string_view toRemove, size_t maxRemoves = 0,
                 bool startFromBack = false);

/// <summary>
/// removes all instances of a certain character "toRemove" from a string.
/// </summary>
/// <param name="toModify">The string to modify.</param>
/// <param name="toRemove">what character needs to be removed.</param>
/// <param name="maxRemoves">maximum number of removes to perform. 0 means unlimited.</param>
/// <param name="startFromBack">Start iterating from the back to front instead of front to back.</param>
/// <return>number of times a remove happened.</return>
size_t StrRemove(std::string& toModify, const char toRemove, size_t maxRemoves = 0,
                 bool startFromBack = false);
/// <summary>
/// Removes trailing characters at the front and back of the string, specified by charsToTrim.
/// </summary>
/// <param name="str">the string to trim</param>
/// <param name="charsToTrim">the characters that are to be considered unwanted at the front/back, to be removed</param>
/// <returns>the trimmed string</returns>
std::string StrTrim(std::string_view str, std::string_view charsToTrim = WHITESPACE_CHARS);
/// <summary>
/// Removes trailing characters at the front of the string, specified by charsToTrim.
/// </summary>
/// <param name="str">the string to trim</param>
/// <param name="charsToTrim">the characters that are to be considered unwanted at the front, to be removed</param>
/// <returns>the trimmed string</returns>
std::string StrTrimFront(std::string_view str, std::string_view charsToTrim = WHITESPACE_CHARS);
/// <summary>
/// Removes trailing characters at the back of the string, specified by charsToTrim.
/// </summary>
/// <param name="str">the string to trim</param>
/// <param name="charsToTrim">the characters that are to be considered unwanted at the back, to be removed</param>
/// <returns>the trimmed string</returns>
std::string StrTrimBack(std::string_view str, std::string_view charsToTrim = WHITESPACE_CHARS);

/// <summary>
/// Splits a string container into a vector of strings, using the delimiters provided.
/// Delimiters mark the spots where the string should be split.
/// Delimiter characters will not show up in the split strings.
/// </summary>
/// <param name="toSplit">the string to be split.</param>
/// <param name="delimiters">delimiter characters to use to mark where to split the string. can use multiple characters at once.</param>
/// <returns>vector of strings that have been split.</returns>
std::vector<std::string> StrSplit(std::string_view toSplit, std::string_view delimiters = " ");

/// <summary>
/// Converts a string to upper case.
/// </summary>
/// <param name="str">string to convert</param>
/// <returns>copy of a string that is upper-cased</returns>
std::string StrToUpper(std::string_view str);
/// <summary>
/// Converts a string to lower case.
/// </summary>
/// <param name="str">string to convert</param>
/// <returns>copy of a string that is lower-cased</returns>
std::string StrToLower(std::string_view str);

/// <summary>
/// Checks if a string starts with a sequence of characters
/// </summary>
/// <param name="str">String to check if sequence of characters exists</param>
/// <param name="startsWith">sequence of characters that the string should start with</param>
/// <returns>Returns true if match, false if not.</returns>
bool StrStartsWith(std::string_view str, std::string_view startsWith);
/// <summary>
/// Checks if a string ends with a sequence of characters
/// </summary>
/// <param name="str">String to check if sequence of characters exists</param>
/// <param name="endsWith">sequence of characters that the string should end with</param>
/// <returns>Returns true if match, false if not.</returns>
bool StrEndsWith(std::string_view str, std::string_view endsWith);

/// <summary>
/// Concatenates a bunch of strings together.
/// </summary>
/// <param name="args">string types to concatenate. (string, string_view, c-strings)</param>
/// <returns>Returns a concatenated string.</returns>
template <typename... T>
std::string StrConcat(const T&... args)
{
    std::string ret;
    std::string_view views[]{args...};
    std::string::size_type full_size = 0;
    for (const auto& sub_view : views)
        full_size += sub_view.size();
    ret.reserve(full_size);
    for (const auto& sub_view : views)
    {
        if (sub_view.empty())
            continue;
        ret.append(sub_view);
    }
    return ret;
}

EMB_NAMESPACE_END
