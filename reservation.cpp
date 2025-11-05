#include "reservation.hpp"

// Auxiliary functions
bool isAlphabetic(const string &cadena){
	for(int i = 0; i < cadena.length(); i++){
		if(!isalpha(cadena[i])) return false;
	}
	return !cadena.empty();
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
