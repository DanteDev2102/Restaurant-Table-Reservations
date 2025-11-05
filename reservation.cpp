#include "reservation.hpp"

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
