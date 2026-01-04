#include "interface.hpp"
#include "reservation.hpp"
#include "utils/utility.hpp"

#include <limits>
#include <iomanip>
#include <cstdlib>
#include <chrono>    
#include <thread>
#include <iostream> 

using namespace std;

CmdInterface::CmdInterface() : app(0) {
	app.loadReservations(list1);
}

void CmdInterface::run() {
	int choice = -1;
	
	while (choice != 0) {
		displayMenu();
		
		// Lectura segura del menú
		if (!(cin >> choice)) {
            cout << "Error: Entrada invalida. Ingrese un numero." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            choice = -1;
            system("pause"); // Pausa si hay error
            clearScreen();
            continue; 
        }

        // Limpiamos el buffer justo después de leer la opción del menú
        // Esto evita que el "Enter" del menú afecte a los casos siguientes
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
			system("pause"); // Pausa al terminar
			break;
		case 2: {
            char continuar = 's';
            int tables = app.getQtyTables();
            
            if (tables == 0) {
                 cout << "Error: Configure las mesas (Opcion 1) primero." << endl;
                 system("pause");
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
			    system("pause");
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
			// Nota: updateFunction suele tener sus propios cin/cout,
			// pero por seguridad ponemos pausa al volver
			system("pause"); 
			break;
		}
		
		case 5: {
		    if (list1.isEmpty()) {
		        cout << "No hay reservaciones registradas." << endl;
		        system("pause");
		        break;
		    }
		
		    cout << "----- Reporte de Reservas por Dia -----" << endl;
		    mostrarReservasPorDia(list1, "lunes");
		    mostrarReservasPorDia(list1, "martes");
		    mostrarReservasPorDia(list1, "miercoles");
		    mostrarReservasPorDia(list1, "jueves");
		    mostrarReservasPorDia(list1, "viernes");
		
		    system("pause");
		    break;
		}

		case 6: {
			app.deleteFunction(list1, cancelledList);
			// deleteFunction maneja su flujo, pero aseguramos pausa
			system("pause"); 
			break;
		}
			
		case 7: {
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
		        clientsList.showQueue(false);
		    }
		    else {
		        waitingQueue.showQueue(true);
		    }
		
		    system("pause"); // CORREGIDO: Pausa simple y efectiva
		    break;
		}
		
		case 8: {
			app.showCancelledReservations(cancelledList);
            // Si showCancelled no tiene pausa, la ponemos aqui, si ya tiene, no importa.
			break; 
		}
		
		case 9: {
            cout << "\n--- REGISTRO DE LLEGADA (CHECK-IN) ---" << endl;
            
            string dni, name, day;
            int numMesa = 0;
            
            int maxMesas = app.getQtyTables();
            if (maxMesas == 0) {
                cout << "[!] ERROR: Configure mesas primero (Opc 1)." << endl;
                system("pause");
                break; 
            }

            dni = readDNI("Ingrese Cedula del Cliente: ");

            // 1. BUSCAR RESERVA
            Reservation* res = list1.searchReservationByDni(dni, list1.getFirst());

            if (res != nullptr) {
                name = res->getName();
                numMesa = res->getTable();
                day = res->getDate(); 
                
                cout << "\n>> ¡Reserva Encontrada!" << endl;
                cout << ">> Cliente: " << name << " | Mesa Reservada: " << numMesa << endl;
                
            } else {
                cout << "\n>> Cliente SIN reserva." << endl;
                name = readAlphaString("Ingrese Nombre: ");
                day = "HOY"; 
                
                cout << "¿Desea asignar mesa YA? (1: Si / 0: A la Cola): ";
                int opc; cin >> opc;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar buffer tras leer int
                
                if (opc == 1) {
                    numMesa = readIntegers("Mesa deseada (1-" + to_string(maxMesas) + "): ", 1, maxMesas);
                } else {
                    numMesa = 0; 
                }
            }

            // 2. VALIDACION DE DISPONIBILIDAD REAL
            bool mesaOcupada = false;
            if (numMesa > 0) {
                if (clientsList.isTableOccupied(numMesa)) {
                    mesaOcupada = true;
                    cout << "\n[!] ALERTA: La Mesa " << numMesa << " esta OCUPADA actualmente." << endl;
                }
            }

            // 3. DECISIÓN FINAL
            if (numMesa == 0 || mesaOcupada) {
                if (mesaOcupada) {
                    cout << ">> El cliente debe esperar en la COLA hasta que se libere." << endl;
                }
                Client clienteCola(dni, name, numMesa, day);
                if (waitingQueue.enqueue(clienteCola)) {
                    cout << ">> Cliente " << name << " agregado a la COLA DE ESPERA." << endl;
                } else {
                    cout << ">> Error: Cola llena." << endl;
                }
            } else {
                Client clienteMesa(dni, name, numMesa, day);
                if (clientsList.enqueue(clienteMesa)) {
                    cout << ">> EXITO: Cliente sentado en la Mesa " << numMesa << "." << endl;
                } else {
                    cout << ">> Error: Lista de mesas llena." << endl;
                }
            }
            
            system("pause"); // CORREGIDO: Pausa simple
            break;
        }
        
        case 10: {
            cout << "\n--- ASIGNAR MESA (Cola -> Mesa) ---" << endl;
            
            if (waitingQueue.isEmpty()) {
                cout << ">> No hay clientes esperando." << endl;
            } 
            else {
                Client clienteAtender;
                waitingQueue.dequeue(clienteAtender); 
                
                cout << ">> Atendiendo a: " << clienteAtender.getName() << endl;
                cout << ">> Mesa que esperaba: " << clienteAtender.getTable() << endl;
                
                int mesaDestino = clienteAtender.getTable();
                int maxMesas = app.getQtyTables();

                // 2. VALIDACIÓN ESTRICTA
                bool necesitaCambio = (mesaDestino == 0) || clientsList.isTableOccupied(mesaDestino);
                
                if (necesitaCambio) {
                    if (mesaDestino > 0) {
                        cout << "[!] La Mesa " << mesaDestino << " sigue OCUPADA por otro comensal." << endl;
                    } else {
                        cout << ">> El cliente no tiene mesa especifica asignada." << endl;
                    }
                    
                    do {
                        mesaDestino = readIntegers("Asigne una mesa LIBRE (1-" + to_string(maxMesas) + "): ", 1, maxMesas);
                        if (clientsList.isTableOccupied(mesaDestino)) {
                            cout << ">> Error: La mesa " << mesaDestino << " TAMBIEN esta ocupada." << endl;
                        }
                    } while (clientsList.isTableOccupied(mesaDestino));
                    
                    clienteAtender.setTable(mesaDestino);
                }

                if (clientsList.enqueue(clienteAtender)) {
                    cout << ">> EXITO: Cliente trasladado a la Mesa " << mesaDestino << "." << endl;
                } else {
                    cout << ">> Error critico: Memoria llena." << endl;
                }
            }
            
            system("pause"); // CORREGIDO: Pausa simple
            break;
        }

		default:
			cout << "Ingrese un item de menu valido" << endl;
			system("pause");
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
    cout << "10. Sentar Cliente (Cola -> Mesa)" << endl;
    cout << "0. Salir" << endl;
	cout << "-------------------------------" << endl;
	cout << "Ingrese su opcion" << endl;
}

void CmdInterface::clearScreen() const {
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::system("cls"); 
}
