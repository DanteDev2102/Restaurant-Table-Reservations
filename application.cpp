#include "application.hpp"
#include "utils/utility.hpp"
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

void Application::updateFunction(Reservations& reservationList) {
	int tables = getQtyTables();
	int searchTable, table, peopleQty, selectionVar;
	string searchDate, name, dni, day;
	char continueVar = 's';
	Reservation* resultSearch;
	bool resultUpdate = false;
	bool updateRecordResult;

	while(continueVar == 's' || continueVar == 'S') {
		searchTable = readIntegers("Ingrese el numero de mesa asociado a la reservacion: ", 1, tables);
		searchDate = readValidDay("Ingrese el dia de la reservacion a actualizar: ");

		resultSearch = reservationList.findReservationByDate(searchTable, searchDate);
		if(resultSearch == nullptr) {
			cout << "La reservacion buscada no existe" << endl;
			cout << "¿Desea continuar? (s/n) " << endl;
			cin >> continueVar;
			cin.ignore();
			if (continueVar == 'n' || continueVar == 'N') break;
		    continue;
		}
	    break;
	}

	while(continueVar == 's' || continueVar == 'S') {
        // 1. Guardamos el ID original ANTES de modificar nada
        int oldTable = resultSearch->getTable();
        string oldDate = resultSearch->getDate();
        string oldID = std::to_string(oldTable) + "_" + toLower(oldDate);
        // ----------------------------------------------------

		cout << "\nReservacion encontrada!" << endl;
		cout << "-------------------------" << endl;
		cout << "Ingrese la actualizacion a hacer para la reserva" << endl;
		cout << "-------------------------" << endl;
		cout << "1: Dia y Mesa de la reserva" << endl;
		cout << "2: Nombre y Cedula del reservante" << endl;
		cout << "3: Cantidad de Personas de la reserva" << endl;
		cout << "4: Toda informacion de la reserva" << endl;
		cout << "Otro: Salir" << endl;
		cin >> selectionVar;
		cin.ignore();

		switch(selectionVar) {
			case 1: { // Date and Table
				table = readIntegers("Numero de mesa: ", 1, tables);
				day = readValidDay("Dia de la reserva (Lunes-Viernes): ");
				name = resultSearch->getName();
				dni = resultSearch->getDni();
				peopleQty = resultSearch->getQty();
				resultUpdate = reservationList.updateReservation(resultSearch, table, peopleQty, name, dni, day);
				if(resultUpdate) cout << "Actualizacion exitosa!" << endl;
				else cout << "Error: Ya existe una reserva para esa mesa en ese dia. "<< endl;
				break;
			}
			case 2: { // Name and DNI
				name = readAlphaString("Nombre del cliente: ");
				dni = readDNI("Cedula del cliente (8 digitos): ");
				table = resultSearch->getTable();
				peopleQty = resultSearch->getQty();
				day = resultSearch->getDate();
				resultUpdate = reservationList.updateReservation(resultSearch, table, peopleQty, name, dni, day);
				if(resultUpdate) cout << "Actualizacion exitosa!" << endl;
				else cout << "Ha surgido un error "<< endl;
				break;
			}
			case 3: { // Quantity
				table = resultSearch->getTable();
				peopleQty = readIntegers("Cantidad de personas (1-8): ", 1, 8);
				name = resultSearch->getName();
				dni = resultSearch->getDni();
				day = resultSearch->getDate();	
				resultUpdate = reservationList.updateReservation(resultSearch, table, peopleQty, name, dni, day);
				if(resultUpdate) cout << "Actualizacion exitosa!" << endl;
				else cout << "Ha surgido un error "<< endl;
				break;
			}
			case 4: { // All
				table = readIntegers("Numero de mesa: ", 1, tables);
				name = readAlphaString("Nombre del cliente: ");
				dni = readDNI("Cedula del cliente (8 digitos): ");
				day = readValidDay("Dia de la reserva (Lunes-Viernes): ");
				peopleQty = readIntegers("Cantidad de personas (1-8): ", 1, 8);
				resultUpdate = reservationList.updateReservation(resultSearch, table, peopleQty, name, dni, day);
				if(resultUpdate) cout << "Actualizacion exitosa!" << endl;
				else cout << "Error: Ya existe una reserva para esa mesa en ese dia. "<< endl;
				break;
			}
			default: { break; }
		}

		if(resultUpdate) {
            // Recogemos los datos NUEVOS (ya actualizados en memoria)
            int newTable = resultSearch->getTable();
            string newDate = resultSearch->getDate();
            string newID = std::to_string(newTable) + "_" + toLower(newDate);
            
			json reservationData = parseToJson(newTable, resultSearch->getName(), resultSearch->getDni(), newDate, resultSearch->getQty());

            // LOGICA INTELIGENTE: ¿Cambió el ID (Mesa o Fecha)?
            if (oldID != newID) {
                // Si cambió, debemos BORRAR el viejo y CREAR el nuevo
                // porque la base de datos no encuentra el ID nuevo para actualizar
                db->deleteRecord("reservations", oldID);
                updateRecordResult = db->createRecord("reservations", newID, reservationData);
            } else {
                // Si el ID es igual (solo cambió nombre/dni), hacemos update normal
			    updateRecordResult = db->updateRecord("reservations", newID, reservationData);
            }

			if(updateRecordResult) {
				cout << "Actualizacion guardada en base de datos" << endl;
			} else {
				std::cerr << "Error al momento de actualizar en base de datos" << endl;
			}
		}
	    break;
	}
}

void Application::deleteFunction(Reservations& reservationList, Reservations& cancelledList) {
	char continueVar = 's';
	int table;
	string date;
	Reservation* resultSearch;
	bool resultDelete, recordDeleteResult;
	int tables = getQtyTables();
	
	while (continueVar == 's' || continueVar == 'S') {
		table = readIntegers("Numero de mesa a actualizar: ", 1, tables);
		date = readValidDay("Dia de la mesa a actualizar: ");
		resultSearch = reservationList.findReservationByDate(table, date);
		cout << "Â¿Seguro que desea proseguir con la cancelacion? (s/n) ";
		cin >> continueVar;
	
		if (continueVar == 's' || continueVar == 'S') {
			resultDelete = reservationList.deleteReservation(resultSearch, cancelledList);
		} else {
			break;
		}
		
		if(resultDelete == false) {
			cout << "La reservacion buscada no existe, Â¿desea volver a intentar? (s/n) ";
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
	
		// Delete record from db
		std::string* id = new std::string;
		*id = std::to_string(table) + "_" + toLower(date);
	
		recordDeleteResult = db->deleteRecord("reservations", *id);
		if(recordDeleteResult) {
			cout << "Borrado exitoso en la base de datos" << endl;
		} else {
			cerr << "Ha surgido un error en el borrado en la base de datos" << endl;
		}
		delete id;

		cout << "Presione ENTER para continuar...";
		cin.ignore();
		string _tmp;
		getline(cin, _tmp);
		break;
	}
}

bool Application::deleteReservationRecord(int table, string date) { //CASE 15: COME,PAGA Y AL IRSE EL CLIENTE SE VA LA RESERVA DE LA DB
    string id = std::to_string(table) + "_" + toLower(date);
    return db->deleteRecord("reservations", id);
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

json Application::parseToJson(const int& table, const std::string& name, const std::string& dni, const std::string& date, const int& qty) {
	json reservationData;

	reservationData["table"] = table;
	reservationData["name"] = name;
	reservationData["dni"] = dni;
	reservationData["date"] = date;
	reservationData["qty"] = qty;

	return reservationData;
}

bool Application::createReservation(int table, int qty, const std::string& name, const std::string& dni, const std::string& day) {
	
	// 1- Crear objeto json 
	json reservationData;
	reservationData["table"] = table;
	reservationData["name"] = name;
	reservationData["dni"] = dni;
	reservationData["date"] = toLower(day);
	reservationData["qty"] = qty;
	
	// 2- Crear clave unica: mesa-dia
	std::string id = std::to_string(table) + "_" + toLower(day);
	
	// 3- Tratar de guardar en DB
	if(db->createRecord("reservations", id, reservationData))
	{
		// 4- Si se guardo -> retornar true
		return true;
	} 
	else 
	{
		// 5- No se guardo -> Informar error 
		return false;
	}
}

void Application::loadReservations(Reservations& reservationList) {
    
    json reservationsTable = db->readTable("reservations");
    
    if (reservationsTable.empty()) {
        std::cout << "Informacion: No hay reservas registradas en la base de datos." << std::endl;
        return;
    }
    
    try {
    	    
    	for (auto& item : reservationsTable.items()) {
        	const json& reservationData = item.value();
        
        	int table = reservationData.value("table", 0);
        	int qty = reservationData.value("qty", 0);
        	std::string name = reservationData.value("name", "");
        	std::string dni = reservationData.value("dni", "");
        	std::string date = reservationData.value("date", "");
        
            reservationList.insertAtBeginning(table, qty, name, dni, date);
    	}
    	
	} catch ( std::exception& e) {
		cout << "Error al cargar reservas: "<< e.what() << endl;
	}

    
    std::cout << "Informacion: Reservas cargadas desde la base de datos." << std::endl;
}
