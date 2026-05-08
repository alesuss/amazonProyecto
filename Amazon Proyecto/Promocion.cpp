#include "pch.h"
#include "Promocion.h"

Promocion::Promocion(string c, float d) : codigo(c), descuento(d) {}

bool   Promocion::esValida() { return descuento > 0; }
string Promocion::getCodigo() { return codigo; }
float  Promocion::getDescuento() { return descuento; }