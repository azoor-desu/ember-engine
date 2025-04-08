#include "Helper/Str.h"
#include <string> // string and wide string use
#include <string_view>
#include <vector>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <stringapiset.h> // WideCharToMultiByte()

namespace TE
{

//-------------------------------------------------------------------//
//						  Wide-string conversions					 //
//-------------------------------------------------------------------//

/// <summary>
/// Converts widestring into string
/// </summary>
/// <param name="wstr">The string to convert.</param>
/// <param name="encoding">the type of encoding to be used in conversion.</param>
/// <return>converted string</return>
std::string Str_ConvertWstrToStr(std::wstring_view wstr)
{
    int count =
        WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.length(), NULL, 0, NULL, NULL);
    std::string str(count, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), -1, str.data(), count, NULL, NULL);
    return str;
}

/// <summary>
/// Converts string into widestring
/// </summary>
/// <param name="str">The string to convert.</param>
/// <param name="encoding">the type of encoding to be used in conversion.</param>
/// <return>converted string</return>
std::wstring Str_ConvertStrToWstr(std::string_view str)
{
    int count = MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.length(), NULL, 0);
    std::wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.data(), -1, wstr.data(), count);
    return wstr;
}


/// <summary>
/// replaces all instances of a certain substring "toReplace" with another substring "replaceWith" in the string "toModify".
/// </summary>
/// <param name="toModify">The string to modify.</param>
/// <param name="toReplace">what sequence of characters that needs to be replaced.</param>
/// <param name="replaceWith">a sequence of characters to be replaced with.</param>
/// <param name="maxReplacements">maximum number of replacements to perform. 0 means unlimited.</param>
/// <param name="startFromBack">Start replacing from the back to front instead of front to back.</param>
/// <return>number of times a replacement happens.</return>
size_t Str_Replace(std::string& toModify, std::string_view toReplace, std::string_view replaceWith,
                   size_t maxReplacements, bool startFromBack)
{
    size_t count = 0;
    if (!startFromBack)
    {
        for (std::string::size_type pos = 0;
             std::string::npos != (pos = toModify.find(toReplace.data(), pos, toReplace.length()));
             pos += replaceWith.length())
        {
            toModify.replace(pos, toReplace.length(), replaceWith.data(), replaceWith.length());
            count++;
            if (maxReplacements != 0 && count >= maxReplacements)
                break;
        }
    }
    else
    {
        for (std::string::size_type pos = toModify.length() - 1;
             std::string::npos != (pos = toModify.rfind(toReplace.data(), pos, toReplace.length()));
             pos -= toReplace.length())
        {
            toModify.replace(pos, toReplace.length(), replaceWith.data(), replaceWith.length());
            count++;
            if (maxReplacements != 0 && count >= maxReplacements)
                break;
        }
    }
    return count;
}

/// <summary>
/// replaces all instances of a certain character "toReplace" with another character "replaceWith" in the string "toModify".
/// </summary>
/// <param name="toModify">The string to modify.</param>
/// <param name="toReplace">what character to be replaced.</param>
/// <param name="replaceWith">character to be replaced with.</param>
/// <param name="maxReplacements">maximum number of replacements to perform. 0 means unlimited.</param>
/// <param name="startFromBack">Start iterating from the back to front instead of front to back.</param>
/// <return>number of times a replacement happened.</return>
size_t Str_Replace(std::string& toModify, const char toReplace, const char replaceWith,
                   size_t maxReplacements, bool startFromBack)
{
    size_t count = 0;

    if (!startFromBack)
    {
        for (int pos = 0; pos < toModify.length(); pos++)
        {
            if (toModify[pos] == toReplace)
            {
                toModify[pos] = replaceWith;
                count++;
                if (maxReplacements != 0 && count >= maxReplacements)
                    break;
            }
        }
    }
    else
    {
        for (int pos = (int)toModify.length() - 1; pos >= 0; pos--)
        {
            if (toModify[pos] == toReplace)
            {
                toModify[pos] = replaceWith;
                count++;
                if (maxReplacements != 0 && count >= maxReplacements)
                    break;
            }
        }
    }


    return count;
}

/// <summary>
/// removes all instances of a certain substring "toRemove" from a string.
/// </summary>
/// <param name="toModify">The string to modify.</param>
/// <param name="toRemove">what sequence of characters needs to be removed.</param>
/// <param name="maxRemoves">maximum number of removes to perform. 0 means unlimited.</param>
/// <param name="startFromBack">Start iterating from the back to front instead of front to back.</param>
/// <return>number of times a remove happened.</return>
size_t Str_Remove(std::string& toModify, std::string_view toRemove, size_t maxRemoves,
                  bool startFromBack)
{
    size_t count = 0;
    if (!startFromBack)
    {
        for (std::string::size_type pos = 0;
             std::string::npos != (pos = toModify.find(toRemove.data(), pos, toRemove.length()));)
        {
            toModify.erase(pos, toRemove.length());
            count++;
            if (maxRemoves != 0 && count >= maxRemoves)
                break;
        }
    }
    else
    {
        for (std::string::size_type pos = toModify.length() - 1;
             std::string::npos != (pos = toModify.rfind(toRemove.data(), pos, toRemove.length()));
             pos -= toRemove.length())
        {
            toModify.erase(pos, toRemove.length());
            count++;
            if (maxRemoves != 0 && count >= maxRemoves)
                break;
        }
    }
    return count;
}

/// <summary>
/// removes all instances of a certain character "toRemove" from a string.
/// </summary>
/// <param name="toModify">The string to modify.</param>
/// <param name="toRemove">what character needs to be removed.</param>
/// <param name="maxRemoves">maximum number of removes to perform. 0 means unlimited.</param>
/// <param name="startFromBack">Start iterating from the back to front instead of front to back.</param>
/// <return>number of times a remove happened.</return>
size_t Str_Remove(std::string& toModify, const char toRemove, size_t maxRemoves, bool startFromBack)
{
    size_t count = 0;
    if (!startFromBack)
    {
        for (std::string::size_type pos = 0;
             std::string::npos
             != (pos = toModify.find_first_of(*const_cast<char*>(&toRemove), pos));)
        {
            toModify.erase(pos, 1);
            count++;
            if (maxRemoves != 0 && count >= maxRemoves)
                break;
        }
    }
    else
    {
        for (std::string::size_type pos = toModify.length() - 1;
             std::string::npos != (pos = toModify.find_last_of(*const_cast<char*>(&toRemove), pos));
             pos--)
        {
            toModify.erase(pos, 1);
            count++;
            if (maxRemoves != 0 && count >= maxRemoves)
                break;
        }
    }
    return count;
}
/// <summary>
/// Removes trailing characters at the front and back of the string, specified by charsToTrim.
/// </summary>
/// <param name="str">the string to trim</param>
/// <param name="charsToTrim">the characters that are to be considered unwanted at the front/back, to be removed</param>
/// <returns>the trimmed string</returns>
std::string Str_Trim(std::string_view str, std::string_view charsToTrim)
{
    if (str.empty() || charsToTrim.empty())
        return std::string(str);

    size_t frontpos = str.find_first_not_of(charsToTrim);
    if (frontpos == std::string::npos)
        return std::string(str);
    size_t backpos = str.find_last_not_of(charsToTrim);
    return std::string(str.substr(frontpos, backpos - frontpos + 1));
}
/// <summary>
/// Removes trailing characters at the front of the string, specified by charsToTrim.
/// </summary>
/// <param name="str">the string to trim</param>
/// <param name="charsToTrim">the characters that are to be considered unwanted at the front, to be removed</param>
/// <returns>the trimmed string</returns>
std::string Str_TrimFront(std::string_view str, std::string_view charsToTrim)
{
    if (str.empty() || charsToTrim.empty())
        return std::string(str);

    size_t frontpos = str.find_first_not_of(charsToTrim);
    if (frontpos == std::string::npos)
        return std::string(str);
    return std::string(str.substr(frontpos, str.size() - frontpos + 1));
}
/// <summary>
/// Removes trailing characters at the back of the string, specified by charsToTrim.
/// </summary>
/// <param name="str">the string to trim</param>
/// <param name="charsToTrim">the characters that are to be considered unwanted at the back, to be removed</param>
/// <returns>the trimmed string</returns>
std::string Str_TrimBack(std::string_view str, std::string_view charsToTrim)
{
    if (str.empty() || charsToTrim.empty())
        return std::string(str);
    size_t backpos = str.find_last_not_of(charsToTrim);
    if (backpos == std::string::npos)
        return std::string(str);
    return std::string(str.substr(0, backpos + 1));
}

/// <summary>
/// Splits a string container into a vector of strings, using the delimiters provided.
/// Delimiters mark the spots where the string should be split.
/// Delimiter characters will not show up in the split strings.
/// </summary>
/// <param name="toSplit">the string to be split.</param>
/// <param name="delimiters">delimiter characters to use to mark where to split the string. can use multiple characters at once.</param>
/// <returns>vector of strings that have been split.</returns>
std::vector<std::string> Str_Split(std::string_view toSplit, std::string_view delimiters)
{
    std::vector<std::string> ret;
    if (toSplit.empty() || delimiters.empty())
        return ret;

    size_t currentPos = 0;
    size_t nextDelim = toSplit.find_first_of(delimiters);

    // Skip start trailing delims if any. set the start pos and next delim to the correct positions.
    if (nextDelim == 0)
    {
        currentPos = toSplit.find_first_not_of(delimiters);
        nextDelim = toSplit.find_first_of(delimiters, currentPos);
    }

    while (nextDelim != std::string::npos)
    {
        ret.emplace_back(toSplit.substr(currentPos, nextDelim - currentPos));
        currentPos = toSplit.find_first_not_of(delimiters, nextDelim);
        nextDelim = toSplit.find_first_of(delimiters, currentPos);
    }

    // Handle case where there is no trailing delim. nextDelim is npos, currentPos exists.
    // pushback currentPos to end of string.
    if (currentPos != std::string::npos && nextDelim == std::string::npos)
    {
        ret.emplace_back(toSplit.substr(currentPos, toSplit.size() - currentPos));
    }

    return ret;
}
/// <summary>
/// Converts a string to upper case.
/// </summary>
/// <param name="str">string to convert</param>
/// <returns>copy of a string that is upper-cased</returns>
std::string Str_ToUpper(std::string_view str)
{
    std::string ret(str);
    for (auto& c : ret)
        c = (char)toupper(c);
    return ret;
}
/// <summary>
/// Converts a string to lower case.
/// </summary>
/// <param name="str">string to convert</param>
/// <returns>copy of a string that is lower-cased</returns>
std::string Str_ToLower(std::string_view str)
{
    std::string ret(str);
    for (auto& c : ret)
        c = (char)tolower(c);
    return ret;
}

/// <summary>
/// Checks if a string starts with a sequence of characters
/// </summary>
/// <param name="str">String to check if sequence of characters exists</param>
/// <param name="startsWith">sequence of characters that the string should start with</param>
/// <returns>Returns true if match, false if not.</returns>
bool Str_StartsWith(std::string_view str, std::string_view startsWith)
{
    if (startsWith.empty() || str.size() < startsWith.size())
        return false;
    for (size_t i = 0; i < startsWith.size(); i++)
    {
        if (str[i] != startsWith[i])
            return false;
    }
    return true;
}
/// <summary>
/// Checks if a string ends with a sequence of characters
/// </summary>
/// <param name="str">String to check if sequence of characters exists</param>
/// <param name="endsWith">sequence of characters that the string should end with</param>
/// <returns>Returns true if match, false if not.</returns>
bool Str_EndsWith(std::string_view str, std::string_view endsWith)
{
    if (endsWith.empty() || str.size() < endsWith.size())
        return false;
    int offset = (int)(str.size() - endsWith.size());
    for (int i = (int)endsWith.size() - 1; i >= 0; i--)
    {
        if (str[(size_t)i + (size_t)offset] != endsWith[i])
            return false;
    }
    return true;
}
} // namespace TE
