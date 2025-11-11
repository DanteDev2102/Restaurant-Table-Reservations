#include "interface.hpp"
#include "reservation.hpp"   // para usar Reservation y Reservations
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
            cout << "Error: Entrada inválida. Ingrese un número." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            choice = -1;
            clearScreen();
            continue; 
        }
        
        // Consumir el '\n' que quedó después de >> choice para que futuros getline funcionen
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
			//CONFIGURAR MESAS
			app.configQtyTables();
			break;
		case 2:{
			// RESERVAR UNA MESA
			//  Reservations list1; Lo puse en comentario porque se e declaro en interface.hpp para que la lista1 permanezca viva en todas las opciones
			
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
		case 3: {
			// CONSULTAR RESERVA POR CEDULA
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
	    		// Lectura de cedula
        		getline(cin, dniSearch);

		        // si el usuario accidentalmente presionó Enter, volvemos a pedir
		        if (dniSearch.empty()) {
		            cout << "Cedula vacia. Intente de nuevo." << endl;
		            continue;
		        }
	
			    Reservation* startNode = list1.getFirst(); // reinicia busqueda desde el inicio de la lista
			    
				bool found = false;
	
			    while (startNode != nullptr) {
			    	
			    	Reservation* p = list1.searchReservationByDni(dniSearch, startNode);
			    	if (p == nullptr) break;
			    	
			    	// Mostrar Reservacion
			        cout << "----------Reservacion------------" << endl;
			        cout << "Numero de mesa: " << p->getTable() << endl;
			        cout << "Nombre del cliente: " << p->getName() << endl;
			        cout << "Dia de la reserva: " << p->getDate() << endl;
			        cout << "Cantidad de personas: " << p->getQty() << endl;
			        found = true;
			
			        // Avanzar al siguiente nodo después del encontrado
            		startNode = p->getNext();

			    }
			
			    if (!found) {
			        cout << "No hay reservaciones para esa cedula" << endl;
			    }
			    
			    cout << "\n¿Desea buscar otra cedula? (s/n): ";
			    cin >> continuar;
			    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // limpiar buffer antes del siguiente getline
			}
    		break;
		}
		case 4: {
			int searchTable, mesa, cantPersonas;
            string searchDate, nombre, cedula, dia;
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
                cout << "Reservacion encontrada!" << endl;
                cout << "Ingrese el nuevo numero de mesa: ";
                cin >> mesa;
                cin.ignore();
                cout << "Ingrese el nuevo día de la reservacion ";
                getline(cin, dia);
                cout << "Cedula del cliente: ";
                getline(cin, cedula);
                cout << "Nombre del cliente: ";
                getline(cin, nombre);
                cout << "Cantidad de personas para la reserva: ";
                cin >> cantPersonas;
                bool resultUpdate;
                resultUpdate = list1.updateReservation(resultSearch, mesa, cantPersonas, nombre, cedula, dia);
                if (resultUpdate == false) {
                    cout << "Surgio un error con los datos, ¿desea volver a intentar? (s/n)";
                    cin >> continueVar;
                    if (continueVar == 'n' || continueVar == 'N') {
                        break;
                    }
                    continue;
                }
                cout << "La reservacion se ha actualizado con exito!";
                break;
            }
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
                	resultDelete = list1.deleteReservation(resultSearch);
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
                break;
            }
			break;
		}
			
		case 7: {
			// LISTAR MESAS RESERVADAS
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
