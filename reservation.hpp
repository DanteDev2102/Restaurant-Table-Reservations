#pragma once

#include <iostream>
#include <string>


using namespace std;

class Reservations;

class Reservation{
	private:
		int table = 0;
		int qty = 0;
		string name = "";
		string dni = "";
		string date = "";
		Reservation* next = nullptr;
		
		
	public:		//coloque public
	    Reservation() {}  //Implementacion de metodos constructores
		Reservation(int table, int qty, string name, string dni, string date) 
             : table(table), qty(qty), name(name), dni(dni), date(date), next(nullptr) {}
	
		// Getters
	    int getTable() const { return table; }
	    int getQty() const { return qty; }
	    string getName() const { return name; }
	    string getDni() const { return dni; }
	    string getDate() const { return date; }
    
		 // Getter para avanzar en la lista
	    Reservation* getNext() const { return next; }
    
	friend class Reservations;
};

class Reservations {
	typedef Reservation* ptr;
	
	private:
		Reservation* first;
	
	public:
		Reservations();
		void setFirst(Reservation* p);
		Reservation* getFirst();
		bool full();
		bool checkReservationData(int table, int qty, string name, string dni, string date);
		bool createReservation(int table, int qty, string name, string dni, string date);
		bool updateReservation(Reservation* ptr, int table, int qty, string name, string dni, string date);
		bool deleteReservation(Reservation* ptr);
		
		// Buscar reservacion por cedula
    	 Reservation* searchReservationByDni(const string& dni, Reservation* start);
    	 Reservation* findReservationByDate(int table, string date);
    	 
    	 // Contar Reservaciones
    	  int getCount();
    	 
    	

};
