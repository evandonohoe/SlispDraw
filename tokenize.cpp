#include "tokenize.hpp"
#include <cctype>

#include <iostream>

using namespace std;

vector<string> isolateParentheses(const string& str) {
    vector<string> tokens;
    string token;
    for (const auto& c : str) {
        if (c == OPEN || c == CLOSE) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(string(1, c));
        }
        else {
            token += c;
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}

TokenSequenceType tokenize(std::istream& seq) {
    TokenSequenceType input_tokens;
    TokenSequenceType tokens;

    string token_string;
    while (seq >> token_string) {
        input_tokens.push_back(token_string);
    }


    for (const auto& s : input_tokens) {

        vector<string> temp_tokens = isolateParentheses(s);
        for (const auto& t_s : temp_tokens) {
            if (t_s != "\"") {
                tokens.push_back(t_s);
            }
        }
    }

    return tokens;
}
