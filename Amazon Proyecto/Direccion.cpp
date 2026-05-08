#include "pch.h"
#include "Direccion.h"

Direccion::Direccion(string c, string ci, string cp) : calle(c), ciudad(ci), codigoPostal(cp) {}

string Direccion::getUbicacionFormateada() {
    return calle + ", " + ciudad + " [" + codigoPostal + "]";
}

string Direccion::getCalle() { return calle; }
string Direccion::getCiudad() { return ciudad; }
string Direccion::getCp() { return codigoPostal; }