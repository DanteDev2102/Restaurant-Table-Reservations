#pragma once

#include <iostream>
#include <string>

#include "application.hpp"
#include "reservation.hpp"
#include "cola.hpp"   
#include "clients.hpp"
using namespace std;

class ClienteEspera{
private:
        string dni;
        string name;
        string day;
        int table;
        int qty;

    public:
        // 1. Constructor Vacio (Necesario para la Cola)
        ClienteEspera() {} 

        // 2. Constructor con Datos (Para llenarlo rápido)
        ClienteEspera(string d, string n, string dia, int t, int q) {
            dni = d;
            name = n;
            day = dia;
            table = t;
            qty = q;
        }

        // 3. Métodos "Getters" (Para leer los datos)
        string getDni() { return dni; }
        string getName() { return name; }
        string getDay() { return day; }
        int getTable() { return table; }
        int getQty() { return qty; }
        
        // 4. Métodos "Setters" (Por si necesitas modificar algo)
        void setDni(string d) { dni = d; }
     
};

class CmdInterface {
	private:
		Application app;
		Reservations list1;
		Reservations cancelledList;
		Clients clientsList;
		Cola<ClienteEspera> colaEspera;
		void displayMenu() const;
		void processChoice(int choice);
		void clearScreen() const;
		
	public:
		void run ();
		CmdInterface();

};

