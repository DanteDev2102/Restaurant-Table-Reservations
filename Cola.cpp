#ifndef COLA_CPP
#define COLA_CPP

#include "cola.hpp"

template <typename T>
Cola<T>::Cola() {
    frente = NULL;
    final = NULL;
}

template <typename T>
bool Cola<T>::colaVacia() {
    return (frente == NULL);
}

template <typename T>
void Cola<T>::insertar(T valor) {
    NodoCola<T>* nuevo = new NodoCola<T>();
    nuevo->dato = valor;
    nuevo->next = NULL;

    if (colaVacia()) {
        frente = nuevo;
        final = nuevo;
    } else {
        final->next = nuevo;
        final = nuevo;
    }
}

template <typename T>
void Cola<T>::quitar(T &valor) {
    if (colaVacia()) {
        cout << "Cola vacia." << endl;
        return;
    }

    NodoCola<T>* p = frente;
    valor = p->dato;
    
    frente = frente->next;
    delete p;
    
    if (frente == NULL) {
        final = NULL;
    }
}

template <typename T>
void Cola<T>::borrarCola() {
    NodoCola<T>* p;
    while (!colaVacia()) {
        p = frente;
        frente = frente->next;
        delete p;
    }
    final = NULL;
}

template <typename T>
void Cola<T>::mostrarCola() {
    if (colaVacia()) {
        cout << "\n[!] La lista de espera esta vacia." << endl;
        return;
    }

    NodoCola<T>* aux = frente;
    int turno = 1;
    
    cout << "\n===== LISTA DE ESPERA (RESERVAS) =====" << endl;
    
    while (aux != NULL) {
        cout << "---------- Turno #" << turno << " ------------" << endl;
        cout << "Cliente       : " << aux->dato.name << " (DNI: " << aux->dato.dni << ")" << endl;
        cout << "Solicita Mesa : " << aux->dato.table << endl;
        cout << "Para el dia   : " << aux->dato.day << endl;
        cout << "Cantidad de Personas      : " << aux->dato.qty << endl;
        cout << "Estado        : ESPERANDO DISPONIBILIDAD" << endl;
        
        aux = aux->next;
        turno++;
    }
    cout << "-----------------------------------------" << endl;
}
#endif
