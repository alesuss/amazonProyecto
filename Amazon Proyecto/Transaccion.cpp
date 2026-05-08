#include "pch.h"
#include "Transaccion.h"

Transaccion::Transaccion(string id, string m) : idTransaccion(id), metodoPago(m) {}

bool Transaccion::procesarPago() {
    return true;
}

string Transaccion::getId() { return idTransaccion; }
string Transaccion::getMetodo() { return metodoPago; }