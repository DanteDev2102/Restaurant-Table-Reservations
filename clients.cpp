#include "clients.hpp"
#include <iostream>

// =======================
// Implementación de Client
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
// Implementación de Clients
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

Clients::PtrClient Clients::getFront() {
    return front;
}

Clients::PtrClient Clients::getRear() {
    return rear;
}

Client Clients::getInfo(ClientNode* p) {
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
        Client info = aux->info; 
        
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
        
        aux = aux->next;
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
            return true; // ¡Encontró a alguien comiendo ahí!
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
            return true; // ¡Lo encontró!
        }
        aux = aux->next;
    }
    return false; // No está aquí
}

