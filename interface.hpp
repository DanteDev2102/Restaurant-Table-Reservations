#pragma once

#include <iostream>
#include <string>

// Inclusiones de tus librerías
#include "application.hpp"
#include "reservation.hpp"
#include "clients.hpp" // <--- IMPORTANTE: Incluir la librería de tu equipo

using namespace std;

class CmdInterface {
	private:
		Application app;
		Reservations list1;
		Reservations cancelledList;
		
		// --- AQUÍ DECLARAMOS LAS LISTAS ---
		// Si no están aquí, el .cpp no las puede usar
		Clients clientsList;   // Lista para gente en mesas
		Clients waitingQueue;  // Lista para gente en espera
		// ----------------------------------

		void displayMenu() const;
		void processChoice(int choice);
		void clearScreen() const;
		
	public:
		CmdInterface();
		void run();
};
