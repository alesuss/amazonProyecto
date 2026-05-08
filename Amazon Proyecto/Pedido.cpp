#include "pch.h"
#include "Pedido.h"
#include <iostream>

Pedido::Pedido(int id, string f) : idPedido(id), fecha(f), estado("Pendiente"), transaccion(nullptr), envio(nullptr), direccion(nullptr) {
    detalles = new ListaDoble<DetallePedido*>();
}

Pedido::~Pedido() {
    delete detalles;
    delete transaccion;
    delete envio;
}

void Pedido::agregarDetalle(DetallePedido* d) { detalles->insertarFinal(d); }
void Pedido::setTransaccion(Transaccion* t) { transaccion = t; }
void Pedido::setEnvio(Envio* e) { envio = e; }
void Pedido::setDireccion(Direccion* d) { direccion = d; }
void Pedido::confirmarPedido() { estado = "Confirmado"; }
int    Pedido::getId() { return idPedido; }
string Pedido::getFecha() { return fecha; }
string Pedido::getEstado() { return estado; }    // <-- AGREGADO
ListaDoble<DetallePedido*>* Pedido::getDetalles() { return detalles; }
Transaccion* Pedido::getTransaccion() { return transaccion; }
Direccion* Pedido::getDireccion() { return direccion; }

void Pedido::mostrar() {
    cout << "Pedido #" << idPedido << " | Fecha: " << fecha << " | Estado: " << estado << "\n";
    if (transaccion != nullptr) {
        cout << "  [Transaccion asociada registrada]\n";
    }
    if (direccion != nullptr) {
        cout << "  [Envio a: " << direccion->getUbicacionFormateada() << "]\n";
    }
}