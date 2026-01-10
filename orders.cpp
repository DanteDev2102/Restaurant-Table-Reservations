#include "orders.hpp"

// =======================
// ImplementaciÃ³n de Order
// =======================

Order::Order() : dishCode(0), price(0.0), notes("") {}

Order::Order(int code, double price, const string& notes)
    : dishCode(code), price(price), notes(notes) {}

int Order::getDishCode() const { return dishCode; }
double Order::getPrice() const { return price; }
string Order::getNotes() const { return notes; }

void Order::setDishCode(int code) { dishCode = code; }
void Order::setPrice(double price) { this->price = price; }
void Order::setNotes(const string& notes) { this->notes = notes; }

// =======================
// ImplementaciÃ³n de Orders
// =======================

Orders::Orders() {
    top = nullptr;
}

Orders::~Orders() {
    OrderPtr current = top;
    while (current != nullptr) {
        OrderPtr temp = current;
        current = current->next;
        delete temp;
    }
}

bool Orders::isEmpty() {
    return top == nullptr;
}

bool Orders::isFull() {
    OrderPtr p = new(nothrow) OrderNode;
    if (p == nullptr)
        return true;
    delete p;
    return false;
}

bool Orders::push(int dishCode, const string& notes) {
    const MenuItem* item = findMenuItem(dishCode);
    if (item == nullptr) return false;

    Order newOrder(dishCode, item->getPrice(), notes);
    OrderPtr newNode = new OrderNode;
    newNode->info = newOrder;
    newNode->next = top;
    top = newNode;
    return true;
}

bool Orders::pop(Order& value) {
    if (!isEmpty()) {
        OrderPtr oldTop = top;
        value = oldTop->info;
        top = top->next;
        delete oldTop;
        return true;
    }
    return false;
}

Orders::OrderPtr Orders::getTop() {
    return top;
}

void Orders::setTop(OrderPtr p) {
    top = p;
}

Order Orders::getInfo(OrderNode* p) {
    return p->info;
}

void Orders::setInfo(OrderNode* p, const Order& value) {
    if (p != nullptr)
        p->info = value;
}

double Orders::totalPrice() {
    double total = 0.0;
    OrderPtr current = top;
    while (current != nullptr) {
        total += current->info.getPrice();
        current = current->next;
    }
    return total;
}

int Orders::size() {
    int count = 0;
    OrderPtr current = top;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

//modificarPlato
bool Orders::modifyDish(int oldCodeDish, int newCodeDish, const std::string& newNotes) {
    if (isEmpty()) return false;

    const MenuItem* newItem = findMenuItem(newCodeDish);
    if (newItem == nullptr) return false;

    OrderPtr tempStack = nullptr;
    bool found = false;
    Order modifiedOrder(newCodeDish, newItem->getPrice(), newNotes);

    while (!isEmpty()) {
        Order topOrder;
        pop(topOrder);

        if (!found && topOrder.getDishCode() == oldCodeDish) {
            found = true;
            // No reinsertamos el pedido viejo
        } else {
            OrderPtr tempNode = new OrderNode;
            tempNode->info = topOrder;
            tempNode->next = tempStack;
            tempStack = tempNode;
        }
    }

    while (tempStack != nullptr) {
        OrderPtr temp = tempStack;
        //push(temp->info.getDishCode(), temp->info.getNotes());
        push2(temp->info.getDishCode(), temp->info.getPrice(), temp->info.getNotes());

        
        tempStack = tempStack->next;
        delete temp;
    }

    if (found) {
        push2(modifiedOrder.getDishCode(), modifiedOrder.getPrice(), modifiedOrder.getNotes());

		//push(modifiedOrder.getDishCode(), modifiedOrder.getNotes());
    }

    return found;
}

// verifica si la pila contiene el plato
bool Orders::containsDish(int code) const {
    OrderPtr current = top; // Comienza desde el tope de la pila
    while (current != nullptr) {
        if (current->info.getDishCode() == code) {
            return true; // Si encuentra el código, devuelve true
        }
        current = current->next; // Avanza al siguiente nodo
    }
    return false; // Si recorre toda la pila y no lo encuentra, devuelve false
}


  // Versión extendida 
bool Orders::push2(int dishCode, double price, const string& notes) {
    if (isFull()) return false;
    Order nuevo(dishCode, price, notes);
    OrderNode* nuevoNodo = new OrderNode;
    nuevoNodo->info = nuevo;
    nuevoNodo->next = top;
    top = nuevoNodo;
    return true;
}

// Constructor de copia (copia profunda)
Orders::Orders(const Orders& other) {
    top = nullptr;
    OrderNode* current = other.top;
    vector<Order> temp;

    while (current != nullptr) {
        temp.push_back(current->info);
        current = current->next;
    }

    for (auto it = temp.rbegin(); it != temp.rend(); ++it) {
        push(it->getDishCode(), it->getNotes());
    }
}


// Operador de asignación (copia profunda)
Orders& Orders::operator=(const Orders& other) {
    if (this != &other) {
        Order dummy;
        while (!isEmpty()) {
            pop(dummy);
        }

        OrderNode* current = other.top;
        vector<Order> temp;

        while (current != nullptr) {
            temp.push_back(current->info);
            current = current->next;
        }

        for (auto it = temp.rbegin(); it != temp.rend(); ++it) {
            push(it->getDishCode(), it->getNotes());
        }
    }
    return *this;
}



