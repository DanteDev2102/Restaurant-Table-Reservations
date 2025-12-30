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
		        cout << "Nombre del Cliente: " << p->getName() << endl;
		        cout << "Cedula del Cliente: " << p->getDni() << endl;
		        cout << "Dia de la reserva: " << p->getDate() << endl;
		        cout << "Cantidad de personas: " << p->getQty() << endl;
		        p = p->getNext();
		    }
		
		    cout << "------------------------------" << endl;
		    cout<<"PERSONAS EN COLA DE ESPERA:";
		    colaEspera.mostrarCola();
		    
		    cout << "Presione ENTER para continuar...";
		    string _tmp;
		    getline(cin, _tmp);
		    break;
		}
		
		case 8: {
			app.showCancelledReservations(cancelledList);
			break;
		}
		
		
        case 9: {
            cout << "\n--- REGISTRO DE CLIENTE EN PUERTA ---" << endl;

            // 1. VALIDACIÓN: ¿Existen mesas configuradas?
            int maxMesas = app.getQtyTables();
            if (maxMesas == 0) {
                cout << "[!] ERROR: No se han configurado las mesas." << endl;
                cout << "    Configure las mesas en la opcion 1 primero." << endl;
                cout << "Presione ENTER para continuar...";
                string _tmp; getline(cin, _tmp);
                break; 
            }

            // 2. Variables para los datos
            string dni, name;
            int numMesa = 0;
            
            // 3. Pregunta Inicial
            int hayMesa = readIntegers("¿Hay mesa disponible ahora mismo? (1: Si / 0: No): ", 0, 1);

            // Pedimos datos básicos (DNI y Nombre)
            dni = readDNI("Ingrese Cedula (Solo numeros): ");
            name = readAlphaString("Ingrese Nombre: ");

            if (hayMesa == 1) {
                // --- CAMINO A: SE VA A SENTAR (Ahora mismo) ---
                
                // Validamos la mesa
                numMesa = readIntegers("Asigne numero de mesa (1 - " + to_string(maxMesas) + "): ", 1, maxMesas);
                
                // Creamos el objeto para el Líder (Clients.cpp)
                // Nota: Al sentarse YA, el día es "Hoy" (o puedes pedirlo si prefieres)
                Client clienteNuevo(dni, name, numMesa, "Hoy");

                if (clientsList.enqueue(clienteNuevo)) {
                    cout << ">> EXITO: Cliente registrado y sentado en la Mesa " << numMesa << "." << endl;
                } else {
                    cout << ">> Error: Memoria llena en lista de clientes." << endl;
                }

            } else {
                // --- CAMINO B: SE VA A LA COLA (Lista de Espera) ---
                cout << "\n>> Restaurante Lleno. Ingrese datos para la LISTA DE ESPERA:" << endl;
                
                // Pedimos los datos específicos de la espera
                cout << "--- Detalles de la Espera ---" << endl;
                
                // 1. ¿Qué mesa quiere?
                int mesaEsperada = readIntegers("¿Que mesa espera? (1-" + to_string(maxMesas) + "): ", 1, maxMesas);
                
                // 2. ¿Para qué día? (Usamos readValidDay para validar mayusculas/minusculas)
                string diaEsperado = readValidDay("¿Para que dia es la espera? (Lunes-Viernes): ");
                
                // 3. ¿Cuántos son?
                int personas = readIntegers("Cantidad de personas (1-8): ", 1, 8);

                // Llenamos el paquete con la estructura NUEVA
                DatosCola waitingClient;
                waitingClient.dni = dni;
                waitingClient.name = name;
                waitingClient.table = mesaEsperada; // Ahora sí existe en el struct
                waitingClient.day = diaEsperado;    // Ahora sí existe en el struct
                waitingClient.qty = personas;       // Ahora sí existe en el struct
                
                colaEspera.insertar(waitingClient);
                cout << ">> Cliente agregado a la COLA DE ESPERA para el " << diaEsperado << "." << endl;
            }
            
            cout << "Presione ENTER para continuar...";
            string _tmp; getline(cin, _tmp);
            break;
        }
		default:
            // CORRECCION 2: El default ahora está DENTRO del switch
			cout << "Ingrese un item de menu valido" << endl;
			break;
	} // Fin del switch
} // Fin de processChoice

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
    cout << "8. Listar Reservas Canceladas" << endl;
    cout << "9. Registrar llegada de cliente (Mesa/Cola)" << endl;
    cout << "0. Salir" << endl;
	cout << "-------------------------------" << endl;
	cout << "Ingrese su opcion" << endl;
}

void CmdInterface::clearScreen() const {
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::system("cls"); 
}
