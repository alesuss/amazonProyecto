#pragma once

#include "Usuario.h"

class Administrador : public Usuario {
private:
    int nivelAcceso;
public:
    Administrador(int id, string n, string e, string p, int nv);
    string rol() override;
    void moderarResenas();
    void gestionarCategorias();
};