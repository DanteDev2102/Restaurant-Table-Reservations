#include "application.hpp"

using namespace std;

Application::Application(int qty) {
	qtyTables = qty;
}

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
	
	int alo = getQtyTables();
	
	//cout << "this is a " << alo << endl; se modifico mensaje a espanol
	cout << "Se configuraron " << alo << " mesas en el restaurante." << endl;
}
