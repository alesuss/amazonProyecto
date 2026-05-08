#pragma once

#include "Usuario.h"
#include "Pedido.h"
#include "Direccion.h"
#include "CarritoCompras.h"
#include "ListaDoble.hpp"

class Cliente : public Usuario {
private:
    ListaDoble<Pedido*>* historialPedidos;
    ListaDoble<Direccion*>* direcciones;
    CarritoCompras* carrito;
public:
    Cliente(int id, string n, string e, string p);
    ~Cliente();
    string rol() override;
    void agregarDireccion(Direccion* d);
    void agregarPedido(Pedido* p);
    ListaDoble<Pedido*>* getHistorial();
    CarritoCompras* getCarrito();
    ListaDoble<Direccion*>* getDirecciones();
    void realizarCompra();
};