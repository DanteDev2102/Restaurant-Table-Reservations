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

// Convierte un String la primera letra Mayuscula y el resto minuscula
string capitalize(const string& s) {
    if (s.empty()) return s;
    string out = s;
    out[0] = toupper(out[0]);
    for (size_t i = 1; i < out.size(); ++i)
        out[i] = tolower(out[i]);
    return out;
}

