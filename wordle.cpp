#ifndef RECCHECK
#include <iostream>
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// Add prototype of helper function
void helper(std::string& in, int pos, std::string floating,
            const std::set<std::string>& dict, std::set<std::string>& results);

// Definition of primary wordle function
std::set<std::string> wordle(const std::string& in, const std::string& floating, const std::set<std::string>& dict) {
    std::set<std::string> results;
    std::string current = in;
    
    // Count blanks to ensure we have enough for floating letters
    int blanks = 0;
    for(char c : current) {
        if(c == '-') blanks++;
    }
    
    // If we don't have enough blanks for floating letters, return empty set
    if(blanks < floating.size()) {
        return results;
    }
    
    helper(current, 0, floating, dict, results);
    return results;
}


void helper(std::string& in, int pos, std::string floating,
            const std::set<std::string>& dict, std::set<std::string>& results) {
    // base case: if we've processed the entire string
    if(pos == in.size()) {
        // if we've used all floating letters and the word is in dictionary
        if(floating.empty() && dict.find(in) != dict.end()) {
            results.insert(in);
        }
        return;
    }
    
    // if this position is already filled, move to next position
    if(in[pos] != '-') {
        helper(in, pos + 1, floating, dict, results);
        return;
    }
    
    // calculate remaining blanks
    int remaining_blanks = 0;
    for(size_t i = pos; i < in.size(); i++) {
        if(in[i] == '-') remaining_blanks++;
    }
    
    // if we must use floating letters (not enough blanks left)
    bool must_use_floating = (remaining_blanks <= floating.size());
    
    // try floating letters first
    std::set<char> tried;
    for(size_t i = 0; i < floating.size(); i++) {
        char c = floating[i];
        if(tried.find(c) != tried.end()) continue; // skip duplicates
        
        tried.insert(c);
        in[pos] = c;
        
        // remove this char from floating and continue
        std::string new_floating = floating;
        new_floating.erase(i, 1);
        
        helper(in, pos + 1, new_floating, dict, results);
        in[pos] = '-'; // restore
    }
    
    // if we have extra blanks, try all alphabet letters (not in floating)
    if(!must_use_floating) {
        for(char c = 'a'; c <= 'z'; c++) {
            if(tried.find(c) != tried.end()) continue; // skip if already tried
            in[pos] = c;
            helper(in, pos + 1, floating, dict, results);
            in[pos] = '-'; // restore
        }
    }
}