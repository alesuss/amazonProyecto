#include "pch.h"
#include "Inventario.h"

Inventario::Inventario() {
    listaProductos = new ListaDoble<Producto*>();
}

Inventario::~Inventario() {
    delete listaProductos;
}

double Inventario::calcularValorTotalRecursivo(int pos, int total) {
    if (pos >= total) return 0.0;
    Producto* p = listaProductos->obtener(pos);
    return (p->getPrecio() * p->getStock()) + calcularValorTotalRecursivo(pos + 1, total);
}

void Inventario::agregar(Producto* p) {
    listaProductos->insertarFinal(p);
}

ListaDoble<Producto*>* Inventario::getLista() {
    return listaProductos;
}

Producto* Inventario::buscarProducto(function<bool(Producto*)> lambda) {
    auto* aux = listaProductos->inicio();
    while (aux != nullptr) {
        if (lambda(aux->elem)) return aux->elem;
        aux = aux->sig;
    }
    return nullptr;
}

double Inventario::obtenerValorInventario() {
    return calcularValorTotalRecursivo(0, listaProductos->tam());
}