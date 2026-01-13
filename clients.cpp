#include "clients.hpp"
#include <iostream>

// =======================
// Implementaci贸n de Client
// =======================

Client::Client() : table(0), total(0.0) {}

Client::Client(string dni, string name, int table, string day)
    : dni(dni), name(name), table(table), day(day), total(0.0) {}

string Client::getDni() const { return dni; }
string Client::getName() const { return name; }
int Client::getTable() const { return table; }
string Client::getDay() const { return day; }
Orders& Client::getOrders() { return orders; }
double Client::getTotal() const { return total; }

void Client::setDni(const string& dni) { this->dni = dni; }
void Client::setName(const string& name) { this->name = name; }
void Client::setTable(int table) { this->table = table; }
void Client::setDay(const string& day) { this->day = day; }
void Client::setOrders(const Orders& orders) { this->orders = orders; }
void Client::setTotal(double total) { this->total = total; }

// =======================
// Implementaci贸n de Clients
// =======================

Clients::Clients() {
    front = rear = nullptr;
}

Clients::~Clients() {
    PtrClient p;
    while (front != nullptr) {
        p = front->next;
        delete front;
        front = p;
    }
}

bool Clients::isEmpty() {
    return front == nullptr;
}

bool Clients::isFull() {
    PtrClient p = new(nothrow) ClientNode;
    if (p == nullptr)
        return true;
    delete p;
    return false;
}

bool Clients::enqueue(const Client& value) {
    if (!isFull()) {
        PtrClient newNode = new ClientNode;
        newNode->info = value;
        newNode->next = nullptr;

        if (rear == nullptr)
            front = newNode;
        else
            rear->next = newNode;

        rear = newNode;
        return true;
    }
    return false;
}

bool Clients::dequeue(Client& value) {
    if (!isEmpty()) {
        PtrClient firstNode = front;
        value = firstNode->info;
        front = front->next;

        if (front == nullptr)
            rear = nullptr;

        delete firstNode;
        return true;
    }
    return false;
}

PtrClient Clients::getFront() {
    return front;
}

PtrClient Clients::getRear() {
    return rear;
}

Client& Clients::getInfo(ClientNode* p) {
    return p->info;
}

void Clients::setInfo(ClientNode* p, const Client& value) {
    if (p != nullptr)
        p->info = value;
}

void Clients::showQueue(bool isWaitingList) {
    if (isEmpty()) {
        cout << (isWaitingList ? "La cola de espera esta vacia." : "No hay clientes en mesas.") << endl;
        return;
    }

    PtrClient aux = front;
    int turno = 1;

    cout << (isWaitingList ? "\n=== CLIENTES EN ESPERA ===" : "\n=== CLIENTES EN MESAS ===") << endl;
    
    while (aux != nullptr) {
        Client& info = getInfo(aux);
        
        cout << "-------------------------" << endl;
        if (isWaitingList) {
            cout << "Turno: " << turno << endl;
            cout << "Cliente: " << info.getName() << " (DNI: " << info.getDni() << ")" << endl;
            cout << "Espera para dia: " << info.getDay() << endl;
        } else {
            cout << "MESA: " << info.getTable() << endl;
            cout << "Cliente: " << info.getName() << endl;
            cout << "Total Actual: " << info.getTotal() << endl;
        }
        
        aux = getNext(aux);
        turno++;
    }
    cout << "-------------------------" << endl;
}

bool Clients::isTableOccupied(int tableNum) {
    // Si la mesa es 0, no cuenta como ocupada (0 es para cola de espera)
    if (tableNum == 0) return false;

    PtrClient aux = front;
    while (aux != nullptr) {
        if (aux->info.getTable() == tableNum) {
            return true; // 隆Encontr贸 a alguien comiendo ah铆!
        }
        aux = aux->next;
    }
    return false; // Nadie tiene esa mesa
}
// Verifica si una cedula ya existe en esta lista
bool Clients::isClientInList(string dniCheck) {
    if (isEmpty()) return false;

    PtrClient aux = front;
    while (aux != nullptr) {
        if (aux->info.getDni() == dniCheck) {
            return true; // 隆Lo encontr贸!
        }
        aux = aux->next;
    }
    return false; // No est谩 aqu铆
}

ClientNode* Clients::getNext(ClientNode* p) const {
    return (p != nullptr) ? p->next : nullptr;
}

void Clients::setNext(ClientNode* p, ClientNode* nextNode) {
    if (p != nullptr)
        p->next = nextNode;
}

// Eliminar respetando la estructura FIFO (Usando una cola auxiliar)
bool Clients::removeClientByTable(int tableNum) {
    if (isEmpty()) return false;

    bool encontrado = false;
    
    // 1. Creamos una cola temporal
    // Nota: Como estamos dentro de la clase Clients, podemos crear otra instancia
    Clients colaAux; 
    Client clienteTemp;

    // 2. Vaciamos la cola ORIGINAL en la AUXILIAR (Filtrando)
    while (!this->isEmpty()) {
        this->dequeue(clienteTemp); // Sacamos al primero (FIFO)

        if (clienteTemp.getTable() == tableNum) {
            // O ENCONTRAMOS!
            // Simplemente NO lo metemos en la cola auxiliar.
            // Al salir de este ciclo, este cliente desaparece.
            encontrado = true;
        } else {
            // No es el que buscamos, lo guardamos en la auxiliar
            colaAux.enqueue(clienteTemp);
        }
    }

    // 3. Restauramos la cola ORIGINAL
    // Pasamos todos los sobrevivientes de vuelta a "this"
    while (!colaAux.isEmpty()) {
        colaAux.dequeue(clienteTemp);
        this->enqueue(clienteTemp);
    }

    return encontrado;
}

