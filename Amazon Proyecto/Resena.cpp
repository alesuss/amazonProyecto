#include "pch.h"
#include "Resena.h"

Resena::Resena(int e, string c) : estrellas(e), comentario(c) {}

bool Resena::validarResena() {
    return estrellas >= 1 && estrellas <= 5;
}

int Resena::getEstrellas() { return estrellas; }
string Resena::getComentario() { return comentario; }