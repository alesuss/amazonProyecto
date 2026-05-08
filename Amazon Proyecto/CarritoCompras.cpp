#include "pch.h"
#include "CarritoCompras.h"

CarritoCompras::CarritoCompras() : totalTemporal(0.0) {
    items = new ListaDoble<Producto*>();
}

CarritoCompras::~CarritoCompras() {
    delete items;
}

void CarritoCompras::agregarItem(Producto* p) {
    items->insertarFinal(p);
    totalTemporal += p->getPrecio();
}

float CarritoCompras::calcularTotal() {
    return totalTemporal;
}

ListaDoble<Producto*>* CarritoCompras::getItems() {
    return items;
}

void CarritoCompras::vaciar() {
    while (items->tam() > 0) {
        items->eliminarInicio();
    }
    totalTemporal = 0.0;
}