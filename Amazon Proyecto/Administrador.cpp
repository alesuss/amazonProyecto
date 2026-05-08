#include "pch.h"
#include "Administrador.h"
#include <iostream>

Administrador::Administrador(int id, string n, string e, string p, int nv) : Usuario(id, n, e, p), nivelAcceso(nv) {}

string Administrador::rol() { return "Administrador"; }

void Administrador::moderarResenas() { cout << "Moderando resenas...\n"; }
void Administrador::gestionarCategorias() { cout << "Actualizando categorias...\n"; }