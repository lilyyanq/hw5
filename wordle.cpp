#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include <vector>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here

//helper to fill any remaining blanks with all possible letters from a-z
void fill_remaining(string& current, size_t blank_idx, const vector<size_t>& blanks,
                    int blanks_left, const string& fixed, set<string>& result, const set<string>& dict);

//uses place_floating to try all combinations of placing char d and n in blank positions
void place_floating(string& current, size_t floating_idx, const vector<size_t>& blanks, 
                    const string& floating, vector<bool>& used_blanks, int floating_to_place, 
                    int total_blanks, int used_count, const string& fixed, set<string>& result, 
                    const set<string>& dict);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    set<string> result;
    vector<size_t> blanks;
    for(size_t i = 0; i < in.length(); ++i)
    {
      if(in[i] == '-')
      {
        blanks.push_back(i);
      }
    }
    string adjusted_floating = floating;
    //adjust floating characters
    for(char c : in)
    {
      if(c != '-')
      {
        size_t pos = adjusted_floating.find(c);
        if(pos != string::npos)
        {
          adjusted_floating.erase(pos, 1);
        }
      }
    }
    if(adjusted_floating.length() > blanks.size())
    {
      return result;
    }
    string current = in;
    vector<bool> used_blanks(blanks.size(), false);
    place_floating(current, 0, blanks, adjusted_floating, used_blanks, adjusted_floating.length(), blanks.size(), 0, in, result, dict);
    return result; 
}

// Define any helper functions here
void fill_remaining(string& current, size_t blank_idx, const vector<size_t>& blanks, int blanks_left, const string& fixed, set<string>& result, const set<string>& dict)
{
  if(blanks_left == 0)
  {
    if(dict.count(current))//real word, put in result set
    {
      result.insert(current);
    }
    return;
  }
  //find next not filled blank position
  while(blank_idx < blanks.size() && current[blanks[blank_idx]] != '-')
  {
    blank_idx++;
  }
  //if run out of blank positions, return
  if(blank_idx >= blanks.size())
  {
    return;
  }
  size_t pos = blanks[blank_idx];
  for(char c = 'a'; c <='z'; ++c)
  {
    current[pos] = c;
    fill_remaining(current, blank_idx + 1, blanks, blanks_left - 1, fixed, result, dict);
  }
  current[pos] = '-';//backtrack
}

void place_floating(string& current, size_t floating_idx, const vector<size_t>& blanks, const string& floating, vector<bool>& used_blanks, int floating_to_place, int total_blanks, int used_count, const string& fixed, set<string>& result, const set<string>& dict)
{
  if(floating_to_place == 0)
  {
    int remaining_blanks = total_blanks - used_count;
    fill_remaining(current, 0, blanks, remaining_blanks, fixed, result, dict);
    return;
  }
  char f = floating[floating_idx];
  //try placing curr floating character
  for(size_t i = 0; i < blanks.size(); ++i)
  {
    if(!used_blanks[i])
    {
      size_t pos = blanks[i];
      current[pos] = f;
      used_blanks[i] = true;
      place_floating(current, floating_idx + 1, blanks, floating, used_blanks, floating_to_place - 1, total_blanks, used_count + 1, fixed, result, dict);
      used_blanks[i] = false; 
      current[pos] = '-';//backtrack
    }
  }
}