#include "interface.hpp"
#include "reservation.hpp"
#include "utils/utility.hpp"

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
            cout << "Error: Entrada invalida. Ingrese un numero." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            choice = -1;
            clearScreen();
            continue; 
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
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
            char continuar = 's';
            int tables = app.getQtyTables();
            int table, peopleQty;
            string name, dni, day;
            
		    while (continuar == 's' || continuar == 'S') {
		    	table = readIntegers("Numero de mesa: ", 1, tables);
		    	name = readAlphaString("Nombre del cliente: ");
		    	dni = readDNI("Cedula del cliente (8 digitos): ");
		    	day = readValidDay("Dia de la reserva (Lunes-Viernes): ");
		    	peopleQty = readIntegers("Cantidad de personas (1-8): ", 1,8);
		    	
		    	if(app.createReservation(table, peopleQty, name, dni, day))
		    	{
		    		if(list1.insertAtBeginning(table,peopleQty,name,dni,day)){
		    			cout <<"Reservacion exitosa"<<endl;
					} else{
						cout << "Error: Memoria llena"<< endl;
					}
				} else{
					cout << "Error: Ya existe una reserva para esa mesa en ese dia."<< endl;
				}
		        cout << "¿Desea agregar otra reserva? (s/n): ";
		        cin >> continuar;
		        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar buffer
		    }
		}
			break;
		case 3: {
			if (list1.isEmpty()) {
			    cout << "No hay reservaciones registradas." << endl;
			    cout << "Presione ENTER para continuar...";
			    string _tmp; getline(cin, _tmp);
			    break;
			}

			char continuar = 's';
    		while (continuar == 's' || continuar == 'S') {
	    		string dniSearch;
	    		cout << "Cedula del cliente: ";

        		getline(cin, dniSearch);

		        if (dniSearch.empty()) {
		            cout << "Cedula vacia. Intente de nuevo." << endl;
		            continue;
		        }
	
			    Reservation* startNode = list1.getFirst();
			    
				bool found = false;
	
			    while (startNode != nullptr) {
			    	
			    	Reservation* p = list1.searchReservationByDni(dniSearch, startNode);
			    	if (p == nullptr) break;
			    	
			        cout << "----------Reservacion------------" << endl;
			        cout << "Numero de mesa: " << p->getTable() << endl;
			        cout << "Nombre del cliente: " << p->getName() << endl;
			        cout << "Dia de la reserva: " << p->getDate() << endl;
			        cout << "Cantidad de personas: " << p->getQty() << endl;
			        found = true;

            		startNode = p->getNext();

			    }
			
			    if (!found) {
			        cout << "No hay reservaciones para esa cedula" << endl;
			    }
			    
			    cout << "\¿Desea buscar otra cedula? (s/n): ";
			    cin >> continuar;
			    cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
    		break;
		}
		case 4: {
			updateFunction();
			break;
		}
		case 5: {
		    if (list1.isEmpty()) {
		        cout << "No hay reservaciones registradas." << endl;
		        cout << "Presione ENTER para continuar...";
		        string _tmp; getline(cin, _tmp);
		        break;
		    }
		
		    cout << "----- Reporte de Reservas por Dia -----" << endl;
		    mostrarReservasPorDia(list1, "lunes");
		    mostrarReservasPorDia(list1, "martes");
		    mostrarReservasPorDia(list1, "miercoles");
		    mostrarReservasPorDia(list1, "jueves");
		    mostrarReservasPorDia(list1, "viernes");
		
		    cout << "Presione ENTER para continuar...";
		    string _tmp; getline(cin, _tmp);
		    break;
		}


		case 6: {
			deleteFunction();
			break;
		}
			
		case 7: {
		    if (list1.isEmpty()) {
			    cout << "No hay reservaciones registradas." << endl;
			    cout << "Presione ENTER para continuar...";
			    string _tmp; getline(cin, _tmp);
			    break;
			}

		
		    int total = list1.getCount();
		    cout << "----- Total Reservas (" << total << ") -----" << endl;
		    cout << "----- Lista de Reservas -----" << endl;
		
			Reservation* p = list1.getFirst();
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
		    break;
		}



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
    cout << "3. Consultar Reserva por Cedula" << endl;
    cout << "4. Actualizar Reserva" << endl;
    cout << "5. Reporte de Reservas" << endl;
    cout << "6. Cancelar Reserva" << endl;
    cout << "7. Listar Mesas Reservadas" << endl;
    cout << "0. Salir" << endl;
	cout << "-------------------------------" << endl;
	cout << "Ingrese su opcion" << endl;
}

void CmdInterface::clearScreen() const {
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::system("cls"); 
}

void CmdInterface::updateFunction() {
	int tables = app.getQtyTables();
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
        resultSearch = list1.findReservationByDate(searchTable, searchDate);
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
        cout << "Reservacion encontrada!" << endl << endl;
        cout << "Dia de la reserva: " << resultSearch->getDate();
        cout << "Ingrese la actualizacion a hacer para la reserva" << endl << endl;
        cout << "1: Dia y Mesa" << endl;
        cout << "2: Nombre y Cedula del Reservante" << endl;
        cout << "3: Cantidad de Personas de la Reserva" << endl;
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
		    	
		    	if(!list1.updateReservation(resultSearch, table, peopleQty, name, dni, day)) {
		    		cout << "Error: Ya existe una reserva para esa mesa en ese dia. "<< endl;
				} else {
					cout << "Actualizacion exitosa!" << endl;
				}
				break;
			}
			case 2: { // Name and DNI Update
				name = readAlphaString("Nombre del cliente: ");
		    	dni = readDNI("Cedula del cliente (8 digitos): ");
		    	table = resultSearch->getTable();
		    	peopleQty = resultSearch->getQty();
				day = resultSearch->getDate();
				
		    	if(list1.updateReservation(resultSearch, table, peopleQty, name, dni, day)) {
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
				
				if(list1.updateReservation(resultSearch, table, peopleQty, name, dni, day)) {
		    		cout << "Actualizacion exitosa!" << endl;
				} else {
					cout << "Ha surgido un error "<< endl;
				}
				break;
			}
			case 4: { // All update
				table = readIntegers("Numero de mesa: ", 1, tables);
		    	name = readAlphaString("Nombre del cliente: ");
		    	dni = readDNI("Cedula del cliente (8 digitos): ");
		    	day = readValidDay("Dia de la reserva (Lunes-Viernes): ");
		    	peopleQty = readIntegers("Cantidad de personas (1-8): ", 1, 8);
		    	
		    	if(!list1.updateReservation(resultSearch, table, peopleQty, name, dni, day)) {
		    		cout << "Error: Ya existe una reserva para esa mesa en ese dia. "<< endl;
				} else {
					cout << "Actualizacion exitosa!" << endl;
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
void CmdInterface::deleteFunction() {
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
        resultSearch = list1.findReservationByDate(mesa, dia);
        cout << "¿Seguro que desea proseguir con la cancelacion? (s/n) ";
        cin >> continueVar;
        if (continueVar == 's' || continueVar == 'S') {
        	resultDelete = list1.deleteReservation(resultSearch, cancelledList);
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
