#pragma once

#include <string>
#include <algorithm>

/***********************************************************************************************************************
*  isNum
*   - returns TRUE if specified character is a number
***********************************************************************************************************************/
inline bool isNum(char c)
{
  return (c >= '0' && c <= '9');
}

/***********************************************************************************************************************
*  isAlpha
*   - returns TRUE if specified character is in alphabet
***********************************************************************************************************************/
inline bool isAlpha(char c)
{
  return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

/***********************************************************************************************************************
*  isNumMod
*   - returns TRUE if specified character is a numberic modifier character
***********************************************************************************************************************/
inline bool isNumMod(char c)
{
  return ((c == '.') || (c == '-'));
}

/***********************************************************************************************************************
*  isNumHex
*   - returns TRUE if specified character is a hex character
***********************************************************************************************************************/
inline bool isNumHex(char c)
{
  return (((c >= 'a') && (c <= 'f')) || ((c >= 'A') && (c <= 'F')));
}

/***********************************************************************************************************************
*  isWS
*   - returns TRUE if specified character is white space
***********************************************************************************************************************/
inline bool isWS(char c)
{
  return ((c == ' ') || (c == ',') || (c == '\t') || (c == '\r') || (c == '\n'));
}

/***********************************************************************************************************************
*  atoh
*   - returns integer value of hex string
***********************************************************************************************************************/
inline int atoh(std::string& s)
{
  int val = 0;
  for (int i = 2; (i < (int) s.size()) && (isNum(s[i]) || isNumHex(s[i])); i++) 
  {
    val = (val*16);
    if (s[i] >= 'a')
      val += 10 + s[i] - 'a';
    else
    if (s[i] >= 'A')
      val += 10 + s[i] - 'A';
    else
      val += s[i] - '0';
  }
  return val;
}

/***********************************************************************************************************************
*  toLower
*   - makes all alpha characters in string lower case
***********************************************************************************************************************/
inline void toLower(std::string &s)
{
  if ( !s.empty() )
  {
    std::transform(s.begin(), s.end(), s.begin(), tolower); 
  }
}

/***********************************************************************************************************************
*  toUpper
*   - makes all alpha characters in string upper case
***********************************************************************************************************************/
inline void toUpper(std::string &s)
{
  if ( !s.empty() )
  {
    std::transform(s.begin(), s.end(), s.begin(), toupper); 
  }
}