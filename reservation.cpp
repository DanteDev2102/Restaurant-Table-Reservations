#include "reservation.hpp"
#include "utils/utility.hpp" 

//Constructor
Reservations::Reservations(){
	first = nullptr;
}


// SET y GET
void Reservations::setFirst(Reservation* p){
	first = p;
}
// agrege estaba declarada en reservation.hpp
Reservation* Reservations::getFirst() {
    return first;
}


bool Reservations::full(){
	Reservation* p;
    p=new Reservation();
	
	if(p==nullptr)
	{
		return true;
	}else
	{
		delete p;
		return false;
	}
}

Reservation* Reservations::findReservationByDate(int table, string date) {
    Reservation* p = getFirst();
    while(p != nullptr) {
        if(p->getTable() == table && toLower(p->getDate()) == toLower(date)) {
            return p;
        } else {
            p = p->getNext();
        }
    }
    return p;
}

bool Reservations::insertAtBeginning(int table, int qty, string name, string dni, string date){
	// 1 - Verificamos si hay espacio en memoria
	if(!full()){
		
		// 2 - Una misma mesa no puede ser reservada por dos clientes diferentes el mismo dia
		if(findReservationByDate(table, date) != nullptr) {
			return false;
		}

		// 3 - Creamos nueva reserva e insertamos al inicio si se pasan todas las verificaciones
		Reservation* newNode = new Reservation(table, qty, name, dni, date);
		newNode->next = first;
		first = newNode;
		return true;
	}else{
		return false;
	}
}

bool Reservations::updateReservation(Reservation* ptr, int table, int qty, string name, string dni, string date) {
	if(ptr == nullptr) {
        return false;
    }
    if(!checkReservationData(table, qty, name, dni, date)) {
        return false; // Update values are not valid
    }
    Reservation *foundReservation = findReservationByDate(table, date);
    if(foundReservation != nullptr && foundReservation != ptr) {
        return false; // A reservation already exists
    }
    Reservation* p = ptr;
    p->table = table;
    p->qty = qty;
    p->name = name;
    p->dni = dni;
    p->date = date;
    return true;
}

bool Reservations::deleteReservation(Reservation* ptr, Reservations& cancelledList) {
    Reservation* old;
	if(ptr == nullptr) {
        return false;
    }
    Reservation* p = getFirst();
    if(ptr == p) {
    	setFirst(ptr->getNext());
	} else {
			while (p != nullptr && p->getNext() != ptr) {
				p = p->getNext();
		}
	}
	old = ptr;
	p->next = ptr->getNext();
	cancelledList.insertAtBeginning(ptr->getTable(), ptr->getQty(), ptr->getName(), ptr->getDni(), ptr->getDate());
    delete old;
    return true;
}

// Buscar reservacion por cedula
Reservation* Reservations::searchReservationByDni(const string& dni, Reservation* start) {
    Reservation* p = start;
    while (p != nullptr) {
        if (p->getDni() == dni) {
            return p;
        }
        p = p->next;
    }
    return nullptr;
}


// Contar Reservaciones
int Reservations::getCount() {
    int cnt = 0;
    Reservation* p = getFirst();
    while (p != nullptr) {
        ++cnt;
        p = p->getNext();
    }
    return cnt;
}

//Verifica Lista esta Vacia
bool Reservations::isEmpty() {
    return getFirst() == nullptr;
}

bool Reservations::consumeReservation(Reservation* ptr) { //inhabilitar reservas despues de atendido
    if (ptr == nullptr || isEmpty()) {
        return false;
    }

    // Caso 1: Es el primero de la lista
    if (ptr == first) {
        first = first->getNext();
        delete ptr;
        return true;
    }

    // Caso 2: Está en el resto de la lista
    Reservation* prev = first;
    while (prev != nullptr) {
        if (prev->getNext() == ptr) {
            prev->next = ptr->getNext(); // Saltamos el nodo
            delete ptr; // Lo borramos de la memoria
            return true;
        }
        prev = prev->getNext();
    }

    return false; // No se encontró
}


