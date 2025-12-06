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
