#pragma once

#include <iostream>
#include <string>

#include "application.hpp"
#include "reservation.hpp"
#include "cola.hpp"   
#include "clients.hpp"
using namespace std;

structu DatosCola{
	string dni;
	string name;
	string phone;
};

class CmdInterface {
	private:
		Application app;
		Reservations list1;
		Reservations cancelledList;
		Clients clientsList;
		Cola<DatosCola> colaEspera;
		void displayMenu() const;
		void processChoice(int choice);
		void clearScreen() const;
		
	public:
		void run ();
		CmdInterface();

};

