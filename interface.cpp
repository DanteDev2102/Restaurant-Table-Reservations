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
	
	// Configuración inicial del día de trabajo
    cout << "=== CONFIGURACION INICIAL DEL SISTEMA ===" << endl;
    currentSystemDay = readValidDay("Ingrese el DIA DE OPERACION actual (ej: Lunes): ");
    cout << ">> Sistema configurado para trabajar el dia: " << currentSystemDay << endl;
    system("pause");
    system("cls");
	
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
            cout << "\n--- RECEPCION (DIA: " << currentSystemDay << ") ---" << endl;
            
            string dni, name, day;
            int numMesa = 0;
            
            int maxMesas = app.getQtyTables();
            if (maxMesas == 0) {
                cout << "[!] ERROR: Configure mesas primero (Opc 1)." << endl;
                system("pause");
                break; 
            }

            dni = readDNI("Ingrese Cedula del Cliente: ");

            // Validar si ya está adentro (Igual que antes)
            if (waitingQueue.isClientInList(dni)) {
                cout << "\n[!] EL CLIENTE YA HIZO CHECK-IN." << endl;
                system("pause"); break;
            }
            if (clientsList.isClientInList(dni)) {
                cout << "\n[!] EL CLIENTE YA ESTA COMIENDO." << endl;
                system("pause"); break;
            }

            // 1. BUSCAR RESERVA
            Reservation* res = list1.searchReservationByDni(dni, list1.getFirst());

            if (res == nullptr) {
                cout << "\n[!] ACCESO DENEGADO." << endl;
                cout << ">> Cliente sin reserva registrada." << endl;
                system("pause"); break; 
            }

            // 2. FILTRO DE DIA (ESTO ES LO NUEVO QUE TE PIDIERON)
            // Comparamos el día de la reserva con el día del sistema
            
            string diaReserva = res->getDate();
            
            // Nota: toLower ayuda a que "Viernes" sea igual a "viernes"
            // (Asumo que tienes una funcion toLower en utils, si no, compara directo)
            if (toLower(diaReserva) != toLower(currentSystemDay)) {
                
                cout << "\n[!] RESERVA INVALIDA PARA HOY." << endl;
                cout << "------------------------------" << endl;
                cout << ">> Hoy es: " << currentSystemDay << endl;
                cout << ">> La reserva del cliente es para: " << diaReserva << endl;
                cout << "------------------------------" << endl;
                cout << ">> No se puede aceptar al cliente hoy." << endl;
                
                system("pause");
                break; // Lo sacamos, no entra a la cola
            }

            // --- SI PASA EL FILTRO, TODO IGUAL QUE ANTES ---
            name = res->getName();
            numMesa = res->getTable();
            day = res->getDate(); 
            
            cout << "\n>> [OK] Reserva Confirmada para HOY." << endl;
            cout << ">> Cliente: " << name << " | Mesa: " << numMesa << endl;

            if (clientsList.isTableOccupied(numMesa)) {
                cout << "\n[i] AVISO: La mesa " << numMesa << " aun esta ocupada." << endl;
            } else {
                cout << "\n[i] La mesa " << numMesa << " esta libre." << endl;
            }

            cout << ">> Registrando check-in..." << endl;
            Client clienteLlegando(dni, name, numMesa, day);

            if (waitingQueue.enqueue(clienteLlegando)) {
                cout << ">> EXITO: Cliente ingresado a Restaurante." << endl;
            } else {
                cout << ">> Error:Restaurante lleno con personas en espera." << endl;
            }
            
            system("pause");
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
        
        case 11: {
            cout << "\n--- CIERRE DEL DIA Y CAMBIO DE FECHA ---" << endl;
            cout << "Dia actual: " << currentSystemDay << endl;
            
            // 1. Advertencia
            cout << "\n[!] ADVERTENCIA: Al cambiar el dia, se vaciara el restaurante." << endl;
            cout << "    - Se eliminaran los clientes en espera." << endl;
            cout << "    - Se levantaran los clientes de las mesas." << endl;
            cout << "¿Seguro desea continuar? (1: Si / 0: No): ";
            int opc; 
			cin >> opc;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (opc != 1) {
                cout << ">> Operacion cancelada." << endl;
                system("pause"); break;
            }
            
            // 2. Pedir nuevo día
            string nuevoDia = readValidDay("Ingrese el nuevo dia de operacion: ");
            
            // 3. LIMPIEZA TOTAL (El "Cierre de Caja")
            cout << "\n>> Realizando cierre del dia " << currentSystemDay << "..." << endl;
            
            Client basura; // Variable temporal para sacar gente
            
            // Vaciamos la Cola de Espera
            int contCola = 0;
            while (!waitingQueue.isEmpty()) {
                waitingQueue.dequeue(basura);
                contCola++;
            }
            
            // Vaciamos las Mesas (Clientes comiendo)
            int contMesas = 0;
            while (!clientsList.isEmpty()) {
                clientsList.dequeue(basura);
                contMesas++;
            }
            
            // 4. Actualizar Variable Global
            currentSystemDay = nuevoDia;
            
            cout << ">> Limpieza completada." << endl;
            cout << "   - " << contCola << " personas retiradas de la cola." << endl;
            cout << "   - " << contMesas << " mesas liberadas." << endl;
            cout << "---------------------------------------------" << endl;
            cout << ">> EXITO: SISTEMA INICIADO PARA EL DIA: " << currentSystemDay << endl;
            
            system("pause");
            break;
        }
        
        case 12: {
			cout << "\n--- TOMAR PEDIDOS ---" << endl;
		
		    if (clientsList.isEmpty()) {
		        cout << "No hay clientes en mesas." << endl;
		        system("pause");
		        break;
		    }
		
		    int maxMesas = app.getQtyTables();
		    int tableSearch = readIntegers("Digite la mesa del cliente: ", 1, maxMesas); 
		
		    ClientNode* aux = clientsList.getFront();
		    bool found = false;
		
		    while (aux != nullptr) {
		        Client& cliente = clientsList.getInfo(aux); 
		        if (cliente.getTable() == tableSearch) {
		            found = true;
		
		            char continuar = 's';
		            while (continuar == 's' || continuar == 'S') {
		                int dishCode = readIntegers("Codigo del platillo (1-5): ", 1, 5);
		
		                string notes;
		                cout << "Notas del pedido (preferencias): ";
		                getline(cin, notes);
		
		                if (cliente.getOrders().push(dishCode, notes)) {
		                    cout << "Pedido agregado exitosamente." << endl;
		                    cliente.setTotal(cliente.getOrders().totalPrice());
		                } else {
		                    cout << "Error: Platillo no encontrado." << endl;
		                }
		
		                cout << "¿Desea agregar otro pedido para este cliente? (s/n): ";
		                cin >> continuar;
		                cin.ignore(numeric_limits<streamsize>::max(), '\n');
		            }		
		            break; // salir del bucle, ya se atendió la mesa
		        }
		        aux = clientsList.getNext(aux);
		    }
		    if (!found) {
		        cout << "Cliente no encontrado en mesas." << endl;
		    }
		    system("pause");
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
    cout << "-------------------------------" << endl;
    cout << "	RESERVAS" << endl;
    cout << "-------------------------------" << endl;
    cout << "1. Configurar Cantidad de Mesas" << endl;
    cout << "2. Reservar una mesa" << endl;
    cout << "3. Consultar Reserva por Cedula" << endl;
    cout << "4. Actualizar Reserva" << endl;
    cout << "5. Reporte de Reservas" << endl;
    cout << "6. Cancelar Reserva" << endl;
    cout << "7. Listar Mesas / Cola de Espera" << endl;
    cout << "8. Listar Reservas Canceladas" << endl;
    cout << "-------------------------------" << endl;
    cout << "	CLIENTES" << endl;
    cout << "-------------------------------" << endl;
    cout << "9. Registrar llegada (Mesa/Cola)" << endl;
    cout << "10. Sentar Cliente (Cola -> Mesa)" << endl;
    cout << "11. Cambiar Dia" << endl;
    cout << "-------------------------------" << endl;
    cout << "	PEDIDOS" << endl;
    cout << "-------------------------------" << endl;
    cout << "12. Tomar pedidos" << endl;
	cout << "-------------------------------" << endl;
	cout << "0. Salir" << endl;
	cout << "Ingrese su opcion" << endl;
}

void CmdInterface::clearScreen() const {
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::system("cls"); 
}
