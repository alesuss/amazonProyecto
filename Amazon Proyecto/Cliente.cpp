#include "pch.h"
#include "Cliente.h"
#include <iostream>

Cliente::Cliente(int id, string n, string e, string p) : Usuario(id, n, e, p) {
    historialPedidos = new ListaDoble<Pedido*>();
    direcciones = new ListaDoble<Direccion*>();
    carrito = new CarritoCompras();
}

Cliente::~Cliente() {
    delete historialPedidos;
    delete direcciones;
    delete carrito;
}

string Cliente::rol() { return "Cliente"; }

void Cliente::agregarDireccion(Direccion* d) { direcciones->insertarFinal(d); }
void Cliente::agregarPedido(Pedido* p) { historialPedidos->insertarFinal(p); }
ListaDoble<Pedido*>* Cliente::getHistorial() { return historialPedidos; }
CarritoCompras* Cliente::getCarrito() { return carrito; }
ListaDoble<Direccion*>* Cliente::getDirecciones() { return direcciones; }

void Cliente::realizarCompra() { cout << "Compra iniciada...\n"; }