#include "reservation.hpp"

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

//Constructor
Reservations::Reservations(){
	first = nullptr;
}

//Constructor
Reservations::Reservations(){
	first = nullptr;
}

// SET y GET
void Reservations::setFirst(Reservation* p){
	first = p;
}

bool Reservations::full(){
	Reservation* p;
    p=new Reservation();
	
	if(p==NULL)
	{
		return true;
	}else
	{
		delete p;
		return false;
	}
}

bool Reservations::checkReservationData(int table, int qty, string name, string dni, string date){
	// numero de mesa o cantidad de personas negativo o mayor a 8
	if(table < 0 || qty < 0 || qty > 8 ){
		return false;
	}
	
	//Verificar cadenas
	if(!isAlphabetic(name) || !isAlphabetic(date) || !isNumeric(dni)) {
		return false;
	}
	
	return true;
}
