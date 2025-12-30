#ifndef COLA_HPP
#define COLA_HPP

#include <iostream>
using namespace std;

// Nodo Genérico: T será "DatosCliente" más adelante
template <typename T>
struct NodoCola {
    T dato;
    NodoCola<T>* next;
};

template <typename T>
class Cola {
    private:
        NodoCola<T>* frente;
        NodoCola<T>* final;
    
    public:
        Cola();
        bool colaVacia();
        void insertar(T valor);
        void quitar(T &valor);
        void borrarCola();
        void mostrarCola(); 
};

// TRUCO DE TEMPLATES: Incluir la implementación al final del header
#include "cola.cpp" 

#endif
