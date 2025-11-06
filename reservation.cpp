#include "reservation.hpp"

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
	if(table < 1 || qty < 1 || qty > 8 ){
		return false;
	}
	
	//Verificar cadenas
	if(!isAlphabetic(name) || !isAlphabetic(date) || !isNumeric(dni)) {
		return false;
	}
	
	return true;
}

bool Reservations::createReservation(int table, int qty, string name, string dni, string date){
	// 1 - Verificamos si los datos son validos 
	if(!checkReservationData(table, qty, name, dni, date))
	{
		return false;
	} 
	else 
	{
		if(!full()){
			ptr p = first;
			// 2 - Una misma mesa no puede ser reservada por dos clientes diferentes el mismo dia
			while(p != nullptr){
				if(p-> table == table && toLower(p->date)== toLower(date)){
					return false;
				}
				p = p->next;
			}
			// 3 - Creamos nueva reserva e insertamos al inicio si se pasan todas las verificaciones
			Reservation* newNode = new Reservation(table, qty, name, dni, date);
			newNode->next = first;
			first = newNode;
			return true;
		}
	}
}
