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
