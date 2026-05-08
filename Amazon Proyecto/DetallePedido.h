#pragma once

#include "Producto.h"

class DetallePedido {
private:
    int cantidad;
    float precioUnitario;
    Producto* referencia;
public:
    DetallePedido(int c, float p, Producto* ref);
    float calcularSubtotal();
    Producto* getProducto();
    int   getCantidad();   
    float getPrecioUnitario();
};