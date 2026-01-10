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
            
            // 1. REPORTE Z (Facturación)
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
                cout << "CANTIDAD DE VENTAS: " << countFacturas << endl;
            }

            // 2. Confirmar Limpieza
            cout << "\n[!] ADVERTENCIA: Se eliminaran todos los clientes (Mesas/Cola)." << endl;
            cout << "¿Confirmar cierre? (1: Si / 0: No): ";
            int opc; cin >> opc;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (opc != 1) {
                cout << ">> Cancelado." << endl;
                system("pause"); break;
            }
            
            string nuevoDia = readValidDay("Ingrese el nuevo dia: ");
            
            // 3. LIMPIEZA TOTAL (Usando dequeue, respeta Cola)
            Client basura;
            Invoice basuraInv;
            
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
						const MenuItem* item = findMenuItem(dishCode);
						
                		string nombre = (item != nullptr) ? item->getName() : "Desconocido";
                		double price = (item != nullptr) ? item->getPrice() : 0.00;
                		
                		cout << "------------------------------------------------------"<<endl;
                		cout << " Platillo: "<< nombre <<" || " << "Precio: "<<price<<" Bs."<<endl;
                		cout << "------------------------------------------------------"<<endl;
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
		case 13: {
		    
		    cout << "\n--- MODIFICAR PEDIDO DE UN CLIENTE ---" << endl;
		
		    if (clientsList.isEmpty()) {
		        cout << "No hay clientes en mesas." << endl;
		        system("pause");
		        break;
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
		
		            const MenuItem* oldItem = findMenuItem(oldCodeDish); 
		            const MenuItem* newItem = findMenuItem(newCodeDish); 
		
		            if (!oldItem || !newItem) { 
		                cout << ">> Error: Codigo de plato invalido." << endl; 
		                break;
		            }
		
		            // Verificar si el plato viejo está en la pila
		            if (!cliente.getOrders().containsDish(oldCodeDish)) {
		                cout << ">> Error: El cliente no tiene ese plato en su pedido." << endl;
		                break;
		            }
		
		            // Mostrar nombres platos y confirmar
		            cout << "Modificando \"" << oldItem->getName() << "\" por \"" << newItem->getName() << "\"..." << endl;
		            char confirmar;
		            cout << "¿Esta seguro de realizar esta modificacion? (s/n): ";
		            cin >> confirmar;
		            cin.ignore(numeric_limits<streamsize>::max(), '\n');
		
		            if (confirmar != 's' && confirmar != 'S') {
		                cout << ">> Modificacion cancelada por el usuario." << endl;
		                break;
		            }
		
		            string newNotes;
		            cout << "Nuevas notas del pedido: ";
		            getline(cin, newNotes);
		
		            bool ok = cliente.getOrders().modifyDish(oldCodeDish, newCodeDish, newNotes);
		            if (ok) {
		                cliente.setTotal(cliente.getOrders().totalPrice());
		                cout << ">> Pedido modificado exitosamente." << endl;
		            } else {
		                cout << ">> Error inesperado al modificar el pedido." << endl;
		            }
		
		            break;
		        }
		        aux = clientsList.getNext(aux);
		    }
		
		    if (!foundClient) {
		        cout << "Cliente no encontrado en mesa." << endl;
		    }
		
		    system("pause");
		    break;
		}


		case 14: {
		    cout << "\n--- REPORTE DE PEDIDOS POR MESA ---" << endl;
		
		    if (clientsList.isEmpty()) {
		        cout << "No hay clientes en mesas." << endl;
		        system("pause");
		        break;
		    }
		
		    PtrClient aux = clientsList.getFront();
		    bool hayPedidos = false;
		
		    while (aux != nullptr) {
		        Client& cliente = clientsList.getInfo(aux);
		        Orders& pilaOriginal = cliente.getOrders();
		
		        if (!pilaOriginal.isEmpty()) {
		            hayPedidos = true;
		            cout << "\n==============================" << endl;
		            cout << "Mesa: " << cliente.getTable() << endl;
		            cout << "Pedidos:" << endl;
		
		            Orders copia;
		            Orders auxiliar;
		            Order temp;
		
		            // Volcar la pila original en auxiliar (para invertirla)
		            while (!pilaOriginal.isEmpty()) {
		                pilaOriginal.pop(temp);
		                auxiliar.push2(temp.getDishCode(), temp.getPrice(), temp.getNotes());
		            }
		
		            // Restaurar pila original y construir copia para mostrar
		            while (!auxiliar.isEmpty()) {
		                auxiliar.pop(temp);
		                pilaOriginal.push2(temp.getDishCode(), temp.getPrice(), temp.getNotes()); // restaurar original
		                copia.push2(temp.getDishCode(), temp.getPrice(), temp.getNotes());        // copia para mostrar
		            }
		
		            // Mostrar la copia
		            Order pedido;
		            int i = 1;
		            while (!copia.isEmpty()) {
		                copia.pop(pedido);
		                const MenuItem* item = findMenuItem(pedido.getDishCode());
		                string nombre = (item != nullptr) ? item->getName() : "Desconocido";
		
		                cout << fixed << setprecision(2);
		                cout << "  " << i++ << ". " << nombre
		                     << " (Codigo: " << pedido.getDishCode()
		                     << ", Precio: Bs. " << pedido.getPrice()
		                     << ", Notas: " << pedido.getNotes() << ")" << endl;
		            }
		
		            cout << fixed << setprecision(2);
		            cout << "Total: Bs. " << cliente.getTotal() << endl;
		            cout << "==============================" << endl;
		        }
		
		        aux = clientsList.getNext(aux);
		    }
		
		    if (!hayPedidos) {
		        cout << "No hay pedidos registrados en ninguna mesa." << endl;
		    }
		
		    system("pause");
		    break;
		}

	case 15: {
			    cout << "\n--- SERVIR PLATILLOS (ENTREGA) ---" << endl;
			
			    if (clientsList.isEmpty()) {
			        cout << "No hay clientes en mesas." << endl;
			        system("pause");
			        break;
			    }
			
			    int maxMesas = app.getQtyTables();
			    int tableSearch = readIntegers("Digite la mesa a servir: ", 1, maxMesas);
			
			    ClientNode* aux = clientsList.getFront();
			    bool found = false;
			
			    while (aux != nullptr) {
			        Client& cliente = clientsList.getInfo(aux);
			
			        if (cliente.getTable() == tableSearch) {
			            found = true;
			            Orders& pedidos = cliente.getOrders();
			            
			            if (pedidos.isEmpty()) {
			                cout << "[!] Esta mesa no tiene pedidos pendientes." << endl;
			                break; 
			            }
			
			            cout << "\n>> Entregando pedidos a: " << cliente.getName() << endl;
			            cout << "--- PLATILLOS ---" << endl;
			            
			            // Solo mostramos sin borrar la pila (usamos copia)
			            Orders copia = pedidos; 
			            Order item;
			            int i = 1;
			            while (copia.pop(item)) {
			                const MenuItem* m = findMenuItem(item.getDishCode());
			                string n = (m != nullptr) ? m->getName() : "Unknown";
			                cout << i++ << ". " << n << " (" << item.getNotes() << ")" << endl;
			            }
			            
			            cout << "\n>> [OK] Comida entregada. Buen provecho." << endl;
			            break; 
			        }
			        aux = clientsList.getNext(aux);
			    }
			
			    if (!found) cout << "No se encontro la mesa " << tableSearch << "." << endl;
			    system("pause");
			    break;
		}
		
		case 16: {
			    cout << "\n--- COBRAR CUENTA Y LIBERAR MESA ---" << endl;
			
			    if (clientsList.isEmpty()) {
			        cout << "No hay clientes en mesas." << endl;
			        system("pause");
			        break;
			    }
			
			    int maxMesas = app.getQtyTables();
			    int tableSearch = readIntegers("Digite la mesa a cobrar: ", 1, maxMesas);
			
                // Verificar visualmente si existe
			    bool mesaOcupada = clientsList.isTableOccupied(tableSearch);
			    if (!mesaOcupada) {
			        cout << ">> Error: La mesa " << tableSearch << " ya esta vacia." << endl;
                    system("pause");
                    break;
			    }

                // Obtener datos del cliente antes de borrarlo
                Client clienteCobrar;
                ClientNode* aux = clientsList.getFront();
                while(aux != nullptr) {
                    if (clientsList.getInfo(aux).getTable() == tableSearch) {
                        clienteCobrar = clientsList.getInfo(aux);
                        break;
                    }
                    aux = clientsList.getNext(aux);
                }
                
                // Mostrar la cuenta
	            Orders& pedidos = clienteCobrar.getOrders();
	            cout << "\n>> Generando factura para: " << clienteCobrar.getName() << endl;
	            
	            // Detalle
	            cout << "\n--- FACTURA FINAL ---" << endl;
	            Orders copiaParaMostrar = pedidos; 
	            Order itemOrder;
	            
	            while (copiaParaMostrar.pop(itemOrder)) {
	                const MenuItem* menuItem = findMenuItem(itemOrder.getDishCode());
	                string platoName = (menuItem != nullptr) ? menuItem->getName() : "Unknown";
	                cout << "- " << platoName << " ... Bs. " << itemOrder.getPrice() << endl;
	            }
                
                double totalPagar = clienteCobrar.getTotal();
	            cout << "--------------------------" << endl;
	            cout << "TOTAL A PAGAR: Bs. " << fixed << setprecision(2) << totalPagar << endl;
	            cout << "--------------------------" << endl;
	
	            // Guardar en Historial de Facturas (Invoices)
                Orders copiaParaFactura = pedidos; 
	            Invoice factura(
	                clienteCobrar.getDni(),
	                clienteCobrar.getName(),
	                clienteCobrar.getTable(),
	                clienteCobrar.getDay(),
	                copiaParaFactura,
	                totalPagar
	            );
	
	            if (invoices.insertAtBeginning(factura)) {
	                cout << ">> Factura guardada en sistema." << endl;
                    
                    // AQUI OCURRE LA SALIDA DEL CLIENTE (USANDO LA FUNCION QUE RESPETA LA COLA)
                    // Asegurate de haber pegado removeClientByTable en clients.cpp como vimos antes
                    if (clientsList.removeClientByTable(tableSearch)) {
                        cout << ">> [MESA " << tableSearch << " LIBERADA EXITOSAMENTE]" << endl;
                        cout << ">> El cliente se ha retirado." << endl;
                    } else {
                        cout << ">> Error critico al liberar mesa." << endl;
                    }
                    
	            } else {
	                cout << ">> Error: Memoria llena (Facturas)." << endl;
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
    cout << "15. Servir pedidos" << endl;
    cout << "16. Cobrar y liberar mesa" << endl;
	cout << "-------------------------------" << endl;
	cout << "0. Salir" << endl;
	cout << "Ingrese su opcion" << endl;
}

void CmdInterface::clearScreen() const {
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::system("cls"); 
}
