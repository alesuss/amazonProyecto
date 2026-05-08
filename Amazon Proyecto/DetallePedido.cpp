#include "pch.h"
#include "DetallePedido.h"

DetallePedido::DetallePedido(int c, float p, Producto* ref)
    : cantidad(c), precioUnitario(p), referencia(ref) {
}

float DetallePedido::calcularSubtotal() { return cantidad * precioUnitario; }
Producto* DetallePedido::getProducto() { return referencia; }
int   DetallePedido::getCantidad() { return cantidad; }
float DetallePedido::getPrecioUnitario() { return precioUnitario; }