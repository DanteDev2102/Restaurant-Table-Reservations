#include "clients.hpp"

Clients::Clients() {
    front = rear = nullptr;
}

bool Clients::isEmpty() {
    return front == nullptr;
}

bool Clients::isFull() {
    PtrClient p = new ClientNode;
    if (p == nullptr)
        return true;
    delete p;
    return false;
}

bool Clients::enqueue(Client value) {
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
        front = firstNode->next;

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

void Clients::setInfo(ClientNode* p, Client value) {
    p->info = value;
}

Clients::~Clients() {
    PtrClient p;
    while (!isEmpty()) {
        p = front->next;
        delete front;
        front = p;
    }
}

