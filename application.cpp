#include "application.hpp"

using namespace std;

Application::Application(int qty): qtyTables(qty), db(new Database("db.json")) {}

void Application::setQtyTables(int qty) {
	qtyTables = qty;
}

int Application::getQtyTables() {
	return qtyTables;
}

void Application::configQtyTables() {
	int qty = 0;
	
	cout << "Ingrese cuantas mesas tiene su restaurante" << endl;
	
	if (!(cin >> qty)) {
		cout << "Ingrese un valor numerico" << endl;
		return;
	}
	
	setQtyTables(qty);
	
	for (int i = 0; i < qtyTables; i++) {
		int tableNumber = i + 1;
		
		json tableData;
        tableData["number"] = std::to_string(tableNumber);
        tableData["available"] = true; 
        
		db->createRecord("tables", std::to_string(tableNumber), tableData);
	}
	
	cout << "Se configuraron " << qty << " mesas en el restaurante." << endl;
}

void Application::updateFunction(Reservations& reservationList){
	int tables = getQtyTables();
	int searchTable, table, peopleQty, selectionVar;
	string searchDate, name, dni, day;
	char continueVar = 's';
	Reservation* resultSearch;
	bool resultUpdate;

	while(continueVar == 's' || continueVar == 'S') {
	cout << "Ingrese la mesa reservacion a actualizar: ";
	cin >> searchTable;
	cin.ignore();
	cout << "\nIngrese el dia de la reservacion a actualizar: ";
	getline(cin, searchDate);
	resultSearch = reservationList.findReservationByDate(searchTable, searchDate);
	if(resultSearch == nullptr) {
	cout << "La reservacion buscada no existe" << endl;
	cout << "¿Desea continuar? (s/n) " << endl;
	cin >> continueVar;
	cin.ignore();
	if (continueVar == 'n' || continueVar == 'N') {
	    break;
	}
	    continue;
	}
	break;
	}

	while(continueVar == 's' || continueVar == 'S') {
		cout << "\nReservacion encontrada!" << endl;
		cout << "-------------------------" << endl;
		cout << "Ingrese la actualizacion a hacer para la reserva" << endl;
		cout << "1: Dia y Mesa de la reserva" << endl;
		cout << "2: Nombre y Cedula del reservante" << endl;
		cout << "3: Cantidad de Personas de la reserva" << endl;
		cout << "4: Toda informacion de la reserva" << endl;
		cout << "Otro: Salir" << endl;
		cin >> selectionVar;
		cin.ignore();

		switch(selectionVar) {
			case 1: { // Date and Table Update
				table = readIntegers("Numero de mesa: ", 1, tables);
				day = readValidDay("Dia de la reserva (Lunes-Viernes): ");
				name = resultSearch->getName();
				dni = resultSearch->getDni();
				peopleQty = resultSearch->getQty();
				resultUpdate = reservationList.updateReservation(resultSearch, table, peopleQty, name, dni, day);	
				if(resultUpdate) {
					cout << "Actualizacion exitosa!" << endl;
				} else {
					cout << "Error: Ya existe una reserva para esa mesa en ese dia. "<< endl;
				}
				break;
			}
			case 2: { // Name and DNI Update
				name = readAlphaString("Nombre del cliente: ");
				dni = readDNI("Cedula del cliente (8 digitos): ");
				table = resultSearch->getTable();
				peopleQty = resultSearch->getQty();
				day = resultSearch->getDate();
				resultUpdate = reservationList.updateReservation(resultSearch, table, peopleQty, name, dni, day);	
				if(resultUpdate) {
					cout << "Actualizacion exitosa!" << endl;
				} else {
					cout << "Ha surgido un error "<< endl;
				}
				break;
			}
			case 3: { // Quantity of People Attending Update
				table = resultSearch->getTable();
				peopleQty = readIntegers("Cantidad de personas (1-8): ", 1, 8);
				name = resultSearch->getName();
				dni = resultSearch->getDni();
				day = resultSearch->getDate();	
				resultUpdate = reservationList.updateReservation(resultSearch, table, peopleQty, name, dni, day);	
				if(resultUpdate) {
					cout << "Actualizacion exitosa!" << endl;
				} else {
					cout << "Ha surgido un error "<< endl;
				}
				break;
			}
			case 4: { // Update all
				table = readIntegers("Numero de mesa: ", 1, tables);
				name = readAlphaString("Nombre del cliente: ");
				dni = readDNI("Cedula del cliente (8 digitos): ");
				day = readValidDay("Dia de la reserva (Lunes-Viernes): ");
				peopleQty = readIntegers("Cantidad de personas (1-8): ", 1, 8);
				resultUpdate = reservationList.updateReservation(resultSearch, table, peopleQty, name, dni, day);	
				if(resultUpdate) {
					cout << "Actualizacion exitosa!" << endl;
				} else {
					cout << "Error: Ya existe una reserva para esa mesa en ese dia. "<< endl;
				}
				break;
			}
			default: { // Anything else than 1 to 4
				break;
			}
		}
	break;
	}
}

void Application::deleteFunction(Reservations& reservationList, Reservations& cancelledList) {
	char continueVar = 's';
	int mesa;
	string dia;
	Reservation* resultSearch;
	bool resultDelete;
	
	while (continueVar == 's' || continueVar == 'S') {
	cout << "Ingrese la mesa de la reservacion: ";
	cin >> mesa;
	cin.ignore();
	cout << "Ingrese el dia de la reservacion: ";
	getline(cin, dia);
	resultSearch = reservationList.findReservationByDate(mesa, dia);
	cout << "¿Seguro que desea proseguir con la cancelacion? (s/n) ";
	cin >> continueVar;

	if (continueVar == 's' || continueVar == 'S') {
		resultDelete = reservationList.deleteReservation(resultSearch, cancelledList);
		} else {
			break;
		}
	if(resultDelete == false) {
		cout << "La reservacion buscada no existe, ¿desea volver a intentar? (s/n) ";
		cin >> continueVar;
		if(continueVar == 'n' || continueVar == 'N') {
			break;
		}
		continue;
		}

	cout << "Reservacion cancelada exitosamente" << endl;
	Reservation* puntero = cancelledList.getFirst();
	cout << "Reservacion cancelada: mesa: [" << puntero->getTable() << "]. Del dia: " << puntero->getDate() << endl;
	cout << "------------------------------" << endl;
	cout << "Presione ENTER para continuar...";
	cin.ignore();
	string _tmp;
	getline(cin, _tmp);
	break;
	}
}
void Application::showCancelledReservations(Reservations& cancelledList) {
	if (!cancelledList.isEmpty()) {
		int total = cancelledList.getCount();
		cout << "----- Total Reservas Canceladas (" << total << ") -----" << endl;
		cout << "----- Lista de Reservas Canceladas -----" << endl;
		
		Reservation* p = cancelledList.getFirst();
		while (p != nullptr) {
			cout << "----------Reservacion------------" << endl;
			cout << "Numero de mesa: " << p->getTable() << endl;
			cout << "Nombre del cliente: " << p->getName() << endl;
			cout << "Cedula del cliente: " << p->getDni() << endl;
			cout << "Dia de la reserva: " << p->getDate() << endl;
			cout << "Cantidad de personas: " << p->getQty() << endl;
			p = p->getNext();
		}
		
		cout << "------------------------------" << endl;
		cout << "Presione ENTER para continuar...";
		string _tmp;
		getline(cin, _tmp);
	} else {
		cout << "No hay reservaciones canceladas." << endl;
		cout << "Presione ENTER para continuar...";
		string _tmp; getline(cin, _tmp);
	}
}
