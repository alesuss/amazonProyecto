#pragma once

#include <string>
#include "ListaDoble.hpp"
#include "DetallePedido.h"
#include "Transaccion.h"
#include "Envio.h"
#include "Direccion.h"
using namespace std;

class Pedido {
private:
    int idPedido;
    string fecha;
    string estado;
    ListaDoble<DetallePedido*>* detalles;
    Transaccion* transaccion;
    Envio* envio;
    Direccion* direccion;
public:
    Pedido(int id, string f);
    ~Pedido();
    void agregarDetalle(DetallePedido* d);
    void setTransaccion(Transaccion* t);
    void setEnvio(Envio* e);
    void setDireccion(Direccion* d);
    void confirmarPedido();
    int getId();
    string getFecha();
    string getEstado();
    ListaDoble<DetallePedido*>* getDetalles();
    Transaccion* getTransaccion();
    Direccion* getDireccion();
    void mostrar();
};