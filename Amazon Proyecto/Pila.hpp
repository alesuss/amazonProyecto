#pragma once


// ═══════════════════════════════════════════════════════════════
//  Pila<T>
//  Pila generica (LIFO: Last In, First Out) implementada con
//  nodos enlazados. El "tope" es el primer nodo (ini), de modo
//  que apilar y desapilar son operaciones O(1).
//
//  Estructura totalmente independiente: define su propio Nodo
//  anidado, no depende de Lista ni de ningun otro header.
// ═══════════════════════════════════════════════════════════════
template <typename T>
class Pila {
public:
    // Nodo anidado: publico para permitir recorrer la pila desde fuera
    // mediante inicio(). El acceso es Pila<T>::Nodo*.
    struct Nodo {
        T     elem;
        Nodo* sig;

        Nodo(T elem) : elem(elem), sig(nullptr) {}
        Nodo(T elem, Nodo* sig) : elem(elem), sig(sig) {}
    };

private:
    Nodo* ini;    // puntero al nodo del tope
    unsigned int    lon;    // cantidad de nodos

public:
    // Constructor y destructor
    Pila();
    ~Pila();

    // Consultas
    unsigned int    tam();
    bool            estaVacia();
    Nodo* inicio();           // devuelve el nodo del tope (para recorrer)

    // Operaciones propias de pila
    void            apilar(T elem);     // push: inserta en el tope
    void            desapilar();        // pop:  elimina el tope
    T               tope();             // peek: consulta el elemento del tope

    // Utilidades
    void            vaciar();           // elimina todos los elementos
    bool            buscar(T elem);     // indica si el elemento esta en la pila

    // Metodo nuevo: invierte el orden de los elementos.
    void            invertir();
};


// ═══════════════════════════════════════════════════════════════
//  Constructor y destructor
// ═══════════════════════════════════════════════════════════════
template <typename T>
Pila<T>::Pila() {
    this->ini = nullptr;
    this->lon = 0;
}

template <typename T>
Pila<T>::~Pila() {
    Nodo* aux;
    while (ini != nullptr) {
        aux = ini;
        ini = ini->sig;
        delete aux;
    }
    lon = 0;
}


// ═══════════════════════════════════════════════════════════════
//  Consultas
// ═══════════════════════════════════════════════════════════════
template <typename T>
unsigned int Pila<T>::tam() {
    return lon;
}

template <typename T>
bool Pila<T>::estaVacia() {
    return lon == 0;
}

template <typename T>
typename Pila<T>::Nodo* Pila<T>::inicio() {
    return ini;
}


// ═══════════════════════════════════════════════════════════════
//  Operaciones de pila
// ═══════════════════════════════════════════════════════════════

// Inserta un elemento en el tope de la pila (O(1)).
template <typename T>
void Pila<T>::apilar(T elem) {
    ini = new Nodo(elem, ini);
    lon++;
}

// Elimina el elemento del tope (O(1)). Si la pila esta vacia, no hace nada.
template <typename T>
void Pila<T>::desapilar() {
    if (lon == 0) return;
    Nodo* aux = ini;
    ini = ini->sig;
    delete aux;
    lon--;
}

// Devuelve el elemento del tope sin eliminarlo.
// Precondicion: la pila no debe estar vacia.
template <typename T>
T Pila<T>::tope() {
    return ini->elem;
}


// ═══════════════════════════════════════════════════════════════
//  Utilidades
// ═══════════════════════════════════════════════════════════════

// Elimina todos los elementos. La pila queda vacia y reutilizable.
template <typename T>
void Pila<T>::vaciar() {
    Nodo* aux;
    while (ini != nullptr) {
        aux = ini;
        ini = ini->sig;
        delete aux;
    }
    lon = 0;
}

// Devuelve true si el elemento esta en la pila. O(n).
// Requiere que el tipo T soporte el operador ==.
// Solo informa si existe; no entrega punteros a nodos internos.
template <typename T>
bool Pila<T>::buscar(T elem) {
    Nodo* aux = ini;
    while (aux != nullptr) {
        if (aux->elem == elem) return true;
        aux = aux->sig;
    }
    return false;
}

// Metodo nuevo: invierte el orden de los elementos de la pila.
// Lo que era el tope queda al fondo y viceversa.
template <typename T>
void Pila<T>::invertir() {
    Nodo* aux = ini;
    Nodo* nueva = nullptr;
    while (aux != nullptr) {
        Nodo* sig = aux->sig;
        aux->sig = nueva;
        nueva = aux;
        aux = sig;
    }
    ini = nueva;
}
