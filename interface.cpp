#include "interface.hpp"
#include "reservation.hpp"
#include "utils/utility.hpp"

#include <limits>
#include <iomanip>
#include <cstdlib>
#include <chrono>    
#include <thread>

using namespace std;

CmdInterface::CmdInterface() : app(0) {
	app.loadReservations(list1);
}

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
		case 2: {
            char continuar = 's';
            int tables = app.getQtyTables();
            
            if (tables == 0) {
                 cout << "Error: Configure las mesas (Opcion 1) primero." << endl;
                 break;
            }

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
		        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
		    }
		    break;
		}
			
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
			app.updateFunction(list1);
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
			app.deleteFunction(list1, cancelledList);
			break;
		}
			
		case 7: {
            // CORREGIDO: Submenú para ver Listas o Reservas
		    cout << "\n--- REPORTES Y LISTADOS ---" << endl;
		    cout << "1. Ver Reservas (Base de Datos)" << endl;
		    cout << "2. Ver Clientes Sentados (Comiendo)" << endl;
		    cout << "3. Ver Cola de Espera" << endl;
		    
		    int subOpc = readIntegers("Seleccione una opcion: ", 1, 3);

		    if (subOpc == 1) {
                if (list1.isEmpty()) {
                    cout << "No hay reservaciones registradas." << endl;
                } else {
                    int total = list1.getCount();
                    cout << "----- Total Reservas (" << total << ") -----" << endl;
                    cout << "----- Lista de Reservas -----" << endl;
                
                    Reservation* p = list1.getFirst();
                    while (p != nullptr) {
                        cout << "----------Reservacion------------" << endl;
                        cout << "Numero de mesa: " << p->getTable() << endl;
                        cout << "Nombre del Cliente: " << p->getName() << endl;
                        cout << "Cedula del Cliente: " << p->getDni() << endl;
                        cout << "Dia de la reserva: " << p->getDate() << endl;
                        cout << "Cantidad de personas: " << p->getQty() << endl;
                        p = p->getNext();
                    }
                    cout << "------------------------------" << endl;
                }
		    }
		    else if (subOpc == 2) {
		        // CORREGIDO: Usamos la lista nueva clientsList
		        clientsList.showQueue(false);
		    }
		    else {
		        // CORREGIDO: Usamos la lista nueva waitingQueue
		        waitingQueue.showQueue(true);
		    }
		
		    cout << "Presione ENTER para continuar...";
		    string _tmp; getline(cin, _tmp);
		    break;
		}
		
		case 8: {
			app.showCancelledReservations(cancelledList);
			break;
		}
		
		case 9: {
            cout << "\n--- REGISTRO DE CLIENTE EN PUERTA ---" << endl;

            int maxMesas = app.getQtyTables();
            if (maxMesas == 0) {
                cout << "[!] ERROR: Configure las mesas (Opcion 1) primero." << endl;
                cout << "Presione ENTER para salir...";
                string _tmp; getline(cin, _tmp);
                break; 
            }

            string dni = readDNI("Ingrese Cedula: ");
            string name = readAlphaString("Ingrese Nombre: ");
            
            int hayMesa = readIntegers("¿Hay mesa disponible YA? (1: Si / 0: No): ", 0, 1);

            if (hayMesa == 1) {
                // --- CAMINO A: SE SIENTA ---
                int numMesa = readIntegers("Asigne Mesa (1-" + to_string(maxMesas) + "): ", 1, maxMesas);
                
                // Cliente normal (Mesa asignada, Dia "HOY")
                Client nuevoCliente(dni, name, numMesa, "HOY");
                
                if (clientsList.enqueue(nuevoCliente)) {
                    cout << ">> Exito: Cliente sentado en Mesa " << numMesa << "." << endl;
                } else {
                    cout << ">> Error: Lista de clientes llena." << endl;
                }

            } else {
                // --- CAMINO B: A LA COLA DE ESPERA ---
                cout << ">> Restaurante lleno. Ingresando a Lista de Espera..." << endl;
                string dia = readValidDay("¿Para que dia espera? (Ej: Viernes): ");
                
                // CORREGIDO: Ya no usamos struct. Creamos un CLIENTE con mesa 0.
                Client clienteEnEspera(dni, name, 0, dia);
                
                if (waitingQueue.enqueue(clienteEnEspera)) {
                     cout << ">> Exito: Cliente agregado a la COLA DE ESPERA." << endl;
                } else {
                     cout << ">> Error: Cola de espera llena." << endl;
                }
            }
            cout << "Presione ENTER para continuar...";
            string _tmp; getline(cin, _tmp);
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
    cout << "7. Listar Mesas / Cola de Espera" << endl;
    cout << "8. Listar Reservas Canceladas" << endl;
    cout << "9. Registrar llegada (Mesa/Cola)" << endl;
    cout << "0. Salir" << endl;
	cout << "-------------------------------" << endl;
	cout << "Ingrese su opcion" << endl;
}

void CmdInterface::clearScreen() const {
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::system("cls"); 
}
