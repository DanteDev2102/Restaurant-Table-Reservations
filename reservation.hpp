#pragma once

#include <iostream>
#include <string>
#include "utils/utility.cpp"

using namespace std;

class Reservations;

class Reservation {
	private:
		int table = 0;
		int qty = 0;
		string name = "";
		string dni = "";
		string date = "";
		Reservation* next = nullptr;
			
		Reservation() {}  //Implementacion de metodos constructores
		Reservation(int table, int qty, string name, string dni, string date) 
             : table(table), qty(qty), name(name), dni(dni), date(date), next(nullptr) {}
	
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
};
