#pragma once

#include <iostream>
#include <string>

#include "application.hpp"
#include "reservation.hpp"


class CmdInterface {
	private:
		Application app;
		Reservations list1;   // lista Exista en todas las opciones
		Reservations cancelledList;
		void displayMenu() const;
		void processChoice(int choice);
		void clearScreen() const;
		
	public:
		void run ();
		CmdInterface();
};
