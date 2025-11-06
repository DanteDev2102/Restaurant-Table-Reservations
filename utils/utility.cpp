#include "utility.hpp"
#include <algorithm>
#include <cctype>

// Auxiliary functions
bool isAlphabetic(const string &cadena){
	for(int i = 0; i < cadena.length(); i++){
		if(!isalpha(cadena[i])) return false;
	}
	return !cadena.empty();
}

bool isNumeric(const string &cadena){
	for (int i = 0; i < cadena.length(); i++){
		if (!isdigit(cadena[i])) return false;
	}
	return !cadena.empty();
}

string toLower(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}
