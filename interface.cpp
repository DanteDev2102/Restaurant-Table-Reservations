#include "interface.hpp"
#include "reservation.hpp"
#include "utils/utility.hpp"
#include "orders.hpp"
#include "menu.hpp"
#include "clients.hpp"
#include "invoices.hpp"

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
		
		if (!(cin >> choice)) {
            cout << "Error: Entrada invalida. Ingrese un numero." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            choice = -1;
            system("pause"); 
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
		case 0: break;
		case 1: app.configQtyTables(); system("pause"); break;
		
		case 2: {
            char continuar = 's';
            int tables = app.getQtyTables();
            if (tables == 0) {
                 cout << "Error: Configure las mesas (Opcion 1) primero." << endl;
                 system("pause"); break;
            }
            int table, peopleQty;
            string name, dni, day;
		    while (continuar == 's' || continuar == 'S') {
		    	table = readIntegers("Numero de mesa: ", 1, tables);
		    	name = readAlphaString("Nombre del cliente: ");
		    	dni = readDNI("Cedula del cliente (8 digitos): ");
		    	day = readValidDay("Dia de la reserva (Lunes-Viernes): ");
		    	peopleQty = readIntegers("Cantidad de personas (1-8): ", 1,8);
		    	
		    	if(app.createReservation(table, peopleQty, name, dni, day)) {
		    		if(list1.insertAtBeginning(table,peopleQty,name,dni,day)){
		    			cout <<"Reservacion exitosa"<<endl;
					} else {
						cout << "Error: Memoria llena"<< endl;
					}
				} else {
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
			    system("pause"); break;
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
			    if (!found) cout << "No hay reservaciones para esa cedula" << endl;
			    cout << "\¿Desea buscar otra cedula? (s/n): ";
			    cin >> continuar;
			    cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
    		break;
		}
		
		case 4: app.updateFunction(list1); system("pause"); break;
		
		case 5: {
		    if (list1.isEmpty()) {
		        cout << "No hay reservaciones registradas." << endl;
		        system("pause"); break;
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

		case 6: app.deleteFunction(list1, cancelledList); system("pause"); break;
			
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
		    } else if (subOpc == 2) {
		        clientsList.showQueue(false);
		    } else {
		        waitingQueue.showQueue(true);
		    }
		    system("pause"); 
		    break;
		}
		
		case 8: app.showCancelledReservations(cancelledList); break; 
		
		case 9: {
            cout << "\n--- RECEPCION (DIA: " << currentSystemDay << ") ---" << endl;
            
            string dni, name, day;
            int numMesa = 0;
            int maxMesas = app.getQtyTables();
            if (maxMesas == 0) {
                cout << "[!] ERROR: Configure mesas primero (Opc 1)." << endl;
                system("pause"); break; 
            }

            dni = readDNI("Ingrese Cedula del Cliente: ");

            // Validar si ya está adentro 
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

            // 2. FILTRO DE DIA
            string diaReserva = res->getDate();
            if (toLower(diaReserva) != toLower(currentSystemDay)) {
                cout << "\n[!] RESERVA INVALIDA PARA HOY." << endl;
                cout << ">> Hoy es: " << currentSystemDay << endl;
                cout << ">> La reserva del cliente es para: " << diaReserva << endl;
                system("pause"); break; 
            }

            // --- SI PASA EL FILTRO ---
            name = res->getName();
            numMesa = res->getTable();
            day = res->getDate(); 
            
            cout << "\n>> [OK] Reserva Confirmada para HOY." << endl;
            cout << ">> Cliente: " << name << " | Mesa: " << numMesa << endl;

            if (clientsList.isTableOccupied(numMesa)) 
                cout << "\n[i] AVISO: La mesa " << numMesa << " aun esta ocupada." << endl;
            else 
                cout << "\n[i] La mesa " << numMesa << " esta libre." << endl;

            cout << ">> Registrando check-in..." << endl;
            Client clienteLlegando(dni, name, numMesa, day);

            if (waitingQueue.enqueue(clienteLlegando)) {
                cout << ">> EXITO: Cliente ingresado a Restaurante." << endl;
                // NOTA: NO borramos la reserva aquí. Se borra al pagar (Case 15).
            } else {
                cout << ">> Error: Restaurante lleno." << endl;
            }
            system("pause");
            break;
        }
        
        case 10: {
            cout << "\n--- ASIGNAR MESA (Cola -> Mesa) ---" << endl;
            if (waitingQueue.isEmpty()) {
                cout << ">> No hay clientes esperando." << endl;
            } else {
                Client clienteAtender;
                waitingQueue.dequeue(clienteAtender); 
                cout << ">> Atendiendo a: " << clienteAtender.getName() << endl;
                cout << ">> Mesa que esperaba: " << clienteAtender.getTable() << endl;
                
                int mesaDestino = clienteAtender.getTable();
                int maxMesas = app.getQtyTables();
                bool necesitaCambio = (mesaDestino == 0) || clientsList.isTableOccupied(mesaDestino);
                
                if (necesitaCambio) {
                    if (mesaDestino > 0) cout << "[!] La Mesa " << mesaDestino << " sigue OCUPADA." << endl;
                    else cout << ">> El cliente no tiene mesa especifica." << endl;
                    
                    do {
                        mesaDestino = readIntegers("Asigne una mesa LIBRE (1-" + to_string(maxMesas) + "): ", 1, maxMesas);
                        if (clientsList.isTableOccupied(mesaDestino)) cout << ">> Error: Ocupada." << endl;
                    } while (clientsList.isTableOccupied(mesaDestino));
                    
                    clienteAtender.setTable(mesaDestino);
                }

                if (clientsList.enqueue(clienteAtender)) {
                    cout << ">> EXITO: Cliente trasladado a la Mesa " << mesaDestino << "." << endl;
                } else {
                    cout << ">> Error critico: Memoria llena." << endl;
                }
            }
            system("pause"); 
            break;
        }
        
        case 11: {
            cout << "\n--- CIERRE DEL DIA Y CAMBIO DE FECHA ---" << endl;
            cout << "Dia actual: " << currentSystemDay << endl;
            
            // 1. REPORTE Z
            cout << "\nGenerating Z-Cut Report (Cierre de Caja)..." << endl;
            cout << "---------------------------------------------" << endl;
            if (invoices.isEmpty()) {
                cout << ">> No se generaron facturas hoy." << endl;
            } else {
                double granTotal = 0.0;
                int countFacturas = 0;
                auto nodoFactura = invoices.getFirst(); 
                cout << left << setw(10) << "MESA" << setw(20) << "CLIENTE" << "MONTO" << endl;
                cout << "---------------------------------------------" << endl;
                while (nodoFactura != nullptr) {
                    Invoice info = invoices.getInfo(nodoFactura);
                    cout << left << setw(10) << info.getTable() 
                         << setw(20) << info.getClientName() 
                         << "Bs. " << fixed << setprecision(2) << info.getTotal() << endl;
                    granTotal += info.getTotal();
                    countFacturas++;
                    nodoFactura = invoices.getNext(nodoFactura);
                }
                cout << "---------------------------------------------" << endl;
                cout << "TOTAL DEL DIA (" << currentSystemDay << "): Bs. " << granTotal << endl;
            }
            cout << "\n[!] ADVERTENCIA: Se eliminaran todos los clientes (Mesas/Cola)." << endl;
            cout << "¿Confirmar cierre? (1: Si / 0: No): ";
            int opc; cin >> opc;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (opc != 1) {
                cout << ">> Cancelado." << endl;
                system("pause"); break;
            }
            string nuevoDia = readValidDay("Ingrese el nuevo dia: ");
            
            // 3. LIMPIEZA TOTAL
            Client basura; Invoice basuraInv;
            while (!waitingQueue.isEmpty()) waitingQueue.dequeue(basura);
            while (!clientsList.isEmpty()) clientsList.dequeue(basura);
            while (!invoices.isEmpty()) invoices.removeFromBeginning(basuraInv);
            
            currentSystemDay = nuevoDia;
            cout << ">> SISTEMA REINICIADO PARA: " << currentSystemDay << endl;
            system("pause");
            break;
        }
        
        case 12: {
			cout << "\n--- TOMAR PEDIDOS ---" << endl;
		    if (clientsList.isEmpty()) {
		        cout << "No hay clientes en mesas." << endl;
		        system("pause"); break;
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
						const MenuItem* item = findMenuItem(dishCode);
                		string nombre = (item != nullptr) ? item->getName() : "Desconocido";
                		double price = (item != nullptr) ? item->getPrice() : 0.00;
                		cout << "Platillo: "<< nombre <<" || " << "Precio: "<<price<<" Bs."<<endl;
		                string notes;
		                cout << "Notas del pedido: ";
		                getline(cin, notes);
		                if (cliente.getOrders().push(dishCode, notes)) {
		                    cout << "Pedido agregado exitosamente." << endl;
		                    cliente.setTotal(cliente.getOrders().totalPrice());
		                } else cout << "Error: Platillo no encontrado." << endl;
		                cout << "¿Desea agregar otro pedido? (s/n): ";
		                cin >> continuar;
		                cin.ignore(numeric_limits<streamsize>::max(), '\n');
		            }		
		            break; 
		        }
		        aux = clientsList.getNext(aux);
		    }
		    if (!found) cout << "Cliente no encontrado en mesas." << endl;
		    system("pause");
		    break;
		}
		
		case 13: {
		    cout << "\n--- MODIFICAR PEDIDO ---" << endl;
		    if (clientsList.isEmpty()) {
		        cout << "No hay clientes en mesas." << endl;
		        system("pause"); break;
		    }
		    int maxMesas = app.getQtyTables();
		    int tableSearch = readIntegers("Indique la mesa del cliente: ", 1, maxMesas);
		    ClientNode* aux = clientsList.getFront();
		    bool foundClient = false;
		    while (aux != nullptr) {
		        Client& cliente = clientsList.getInfo(aux);
		        if (cliente.getTable() == tableSearch) {
		            foundClient = true;
		            int oldCodeDish = readIntegers("Codigo del plato a modificar: ", 1, MENU_SIZE);
		            int newCodeDish = readIntegers("Nuevo codigo del plato: ", 1, MENU_SIZE);
		            if (!cliente.getOrders().containsDish(oldCodeDish)) {
		                cout << ">> Error: El cliente no tiene ese plato." << endl;
		                break;
		            }
		            string newNotes;
		            cout << "Nuevas notas del pedido: ";
		            getline(cin, newNotes);
		            if (cliente.getOrders().modifyDish(oldCodeDish, newCodeDish, newNotes)) {
		                cliente.setTotal(cliente.getOrders().totalPrice());
		                cout << ">> Pedido modificado exitosamente." << endl;
		            } else cout << ">> Error al modificar." << endl;
		            break;
		        }
		        aux = clientsList.getNext(aux);
		    }
		    if (!foundClient) cout << "Cliente no encontrado." << endl;
		    system("pause");
		    break;
		}

		case 14: {
		    cout << "\n--- REPORTE DE PEDIDOS ---" << endl;
		    if (clientsList.isEmpty()) {
		        cout << "No hay clientes en mesas." << endl;
		        system("pause"); break;
		    }
		    PtrClient aux = clientsList.getFront();
		    while (aux != nullptr) {
		        Client& cliente = clientsList.getInfo(aux);
		        Orders& pilaOriginal = cliente.getOrders();
		        if (!pilaOriginal.isEmpty()) {
		            cout << "\nMesa: " << cliente.getTable() << " (Total: " << cliente.getTotal() << ")" << endl;
		        }
		        aux = clientsList.getNext(aux);
		    }
		    system("pause");
		    break;
		}

		case 15: {
			    cout << "\n--- SERVIR PLATILLOS, GENERAR FACTURA Y LIBERAR MESA ---" << endl;
			    if (clientsList.isEmpty()) {
			        cout << "No hay clientes en mesas." << endl;
			        system("pause"); break;
			    }
			    int maxMesas = app.getQtyTables();
			    int tableSearch = readIntegers("Digite la mesa del cliente: ", 1, maxMesas);
			
			    if (!clientsList.isTableOccupied(tableSearch)) {
			         cout << ">> Error: La mesa " << tableSearch << " esta vacia." << endl;
                     system("pause"); break;
			    }

			    ClientNode* aux = clientsList.getFront();
			    bool found = false;
			    while (aux != nullptr) {
			        Client& cliente = clientsList.getInfo(aux);
			        if (cliente.getTable() == tableSearch) {
			            found = true;
			            Orders& pedidos = cliente.getOrders();
			            if (pedidos.isEmpty()) {
			                cout << "El cliente no tiene pedidos registrados." << endl;
			                break;
			            }
			            cout << "\n>> Sirviendo platillos para cliente: " 
			                 << cliente.getName() << " (Mesa " << cliente.getTable() << ")" << endl;
			            
			            Orders copiaPedidos = pedidos; 
			            cout << "\n--- PLATILLOS SERVIDOS ---" << endl;
			            Order servido;
			            int i = 1;
			            while (pedidos.pop(servido)) {
			                const MenuItem* item = findMenuItem(servido.getDishCode());
			                string nombre = (item != nullptr) ? item->getName() : "Desconocido";
			                cout << i++ << ". " << nombre << " (Bs. " << servido.getPrice() << ")" << endl;
			            }
			            double montoFinal = cliente.getTotal();
			            cout << "Total a pagar: Bs. " << fixed << setprecision(2) << montoFinal << endl;
			
			            Invoice factura(cliente.getDni(), cliente.getName(), cliente.getTable(), cliente.getDay(), copiaPedidos, montoFinal);
			
			            if (invoices.insertAtBeginning(factura)) {
			                cout << ">> Factura generada y almacenada exitosamente." << endl;
			                
			                // === 1. BORRAR DE MEMORIA (RAM) ===
			                Reservation* res = list1.findReservationByDate(cliente.getTable(), cliente.getDay());
			                if (res != nullptr) list1.consumeReservation(res);

                            // === 2. BORRAR DE BASE DE DATOS (DISCO) ===
			                // Usamos el metodo silencioso que agregamos
			                app.deleteReservationRecord(cliente.getTable(), cliente.getDay());
			                
			                // === 3. LIBERAR MESA ===
			                if (clientsList.removeClientByTable(tableSearch)) {
			                    cout << ">> MESA LIBERADA. El cliente se ha retirado." << endl;
			                } else cout << ">> Error al liberar la mesa." << endl;
			                
			            } else cout << ">> Error: No se pudo almacenar la factura." << endl;
			            break; 
			        }
			        aux = clientsList.getNext(aux);
			    }
			    if (!found) cout << "Cliente no encontrado en mesas." << endl;
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
    cout << "	   RESERVAS" << endl;
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
    cout << "	   CLIENTES" << endl;
    cout << "-------------------------------" << endl;
    cout << "9. Registrar llegada (Mesa/Cola)" << endl;
    cout << "10. Sentar Cliente (Cola -> Mesa)" << endl;
    cout << "11. Cambiar Dia" << endl;
    cout << "-------------------------------" << endl;
    cout << "	   PEDIDOS" << endl;
    cout << "-------------------------------" << endl;
    cout << "12. Tomar pedidos" << endl;
    cout << "13. Modificar pedido de un cliente" << endl;
    cout << "14. Reporte pedidos" << endl;
    cout << "15. Servir pedidos y Cobrar" << endl;
	cout << "-------------------------------" << endl;
	cout << "0. Salir" << endl;
	cout << "Ingrese su opcion" << endl;
}

void CmdInterface::clearScreen() const {
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::system("cls"); 
}
