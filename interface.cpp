#include "interface.hpp"
#include "reservation.cpp"
#include <limits>
#include <iomanip>
#include <cstdlib>
#include <chrono>    
#include <thread>

using namespace std;

CmdInterface::CmdInterface() : app(30) {}

void CmdInterface::run() {
	int choice = -1;
	
	while (choice != 0) {
		displayMenu();
		
		if (!(cin >> choice)) {
            cout << "Error: Entrada inválida. Ingrese un número." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            choice = -1;
            clearScreen();
            continue; 
        }
        
        processChoice(choice);
        
        clearScreen();
	}
}

void CmdInterface::processChoice(int choice) {
	switch (choice) {
		case 0:
			break;
		case 1:
			app.configQtyTables();
			break;
		case 2:{
			Reservations list1;
            char continuar = 's';

		    while (continuar == 's' || continuar == 'S') {
		        int mesa, cantPersonas;
		        string nombre, cedula, dia;
		
		        cout << "\n--- Crear nueva reserva ---" << endl;
		        cout << "Numero de mesa: ";
		        cin >> mesa;
		        cin.ignore();                    // Limpiar buffer antes de getline
		        cout << "Nombre del cliente: ";
		        getline(cin, nombre);
		        cout << "Cedula del cliente: ";
		        getline(cin, cedula);
		        cout << "Dia de la reserva: ";
		        getline(cin, dia);
		        cout << "Cantidad de personas: ";
		        cin >> cantPersonas;

		        list1.createReservation(mesa, cantPersonas, nombre, cedula, dia);
		
		        cout << "\n¿Desea agregar otra reserva? (s/n): ";
		        cin >> continuar;
		        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar buffer
		    }
		}
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		default:
			cout << "Ingrese un item de menu valido" << endl;
			break;
	}
}

void CmdInterface::displayMenu() const {
	cout << "\n===============================" << endl;
    cout << "          MENU PRINCIPAL         " << endl;
    cout << "===============================" << endl;
    cout << "1. Configurar Cantidad de Mesas" << endl;
    cout << "2. Reservar una mesa" << endl;
    cout << "3. Listas Mesas Reservadas" << endl;
    cout << "4. Actualizar Reserva" << endl;
    cout << "5. Reporte de Reservas" << endl;
    cout << "6. Cancelar Reserva" << endl;
    cout << "0. Salir" << endl;
	cout << "-------------------------------" << endl;
	cout << "Ingrese su opción" << endl;
}

void CmdInterface::clearScreen() const {
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std:system("cls");
}
