#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void helper(std::string& cur, int index, std::string floating,
            const std::set<std::string>& dict, std::set<std::string>& results);

// Definition of primary wordle function
std::set<std::string> wordle(const std::string& in, const std::string& floating, const std::set<std::string>& dict) {
    std::set<std::string> results;
    std::string current = in;
    helper(current, 0, floating, dict, results);
    return results;
}

// Define any helper functions here
void helper(std::string& cur, int index, std::string floating,
            const std::set<std::string>& dict, std::set<std::string>& results) {
    
    if (index == cur.size()) {
        if (floating.empty() && dict.find(cur) != dict.end()) {
            results.insert(cur);
        }
        return;
    }

    if (cur[index] != '-') {
        helper(cur, index + 1, floating, dict, results);
    } else {
        // if there are floating letters try placing them first
        std::set<char> tried;

        for (int i = 0; i < floating.size(); ++i) {
            char ch = floating[i];
            if (tried.count(ch)) continue;  // avoid duplicate recursive calls
            tried.insert(ch);

            cur[index] = ch;
            std::string newFloating = floating.substr(0, i) + floating.substr(i + 1);
            helper(cur, index + 1, newFloating, dict, results);
            cur[index] = '-';
        }

        // if more blanks left than floating letters, try a-z
        if (floating.size() < (cur.size() - index)) {
            for (char ch = 'a'; ch <= 'z'; ++ch) {
                // skip already tried floating letters
                if (tried.count(ch)) continue;

                cur[index] = ch;
                helper(cur, index + 1, floating, dict, results);
                cur[index] = '-';
            }
        }
    }
}