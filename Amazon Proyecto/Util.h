#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <functional>

using namespace std;

// ═══════════════════════════════════════════════════════════════
//  guardarArchivo (estructuras lineales)
// ═══════════════════════════════════════════════════════════════
template <typename Estructura, typename FuncSerializar>
void guardarArchivo(Estructura* est, string ruta, FuncSerializar serializar) {
    ofstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "  [ERROR] No se pudo abrir: " << ruta << endl;
        return;
    }
    auto aux = est->inicio();
    while (aux != nullptr) {
        archivo << serializar(aux->elem) << endl;
        aux = aux->sig;
    }
    archivo.close();
    cout << "  Guardado: " << ruta << endl;
}

// ═══════════════════════════════════════════════════════════════
//  guardarArchivoCircular (ListaCircular)
// ═══════════════════════════════════════════════════════════════
template <typename Estructura, typename FuncSerializar>
void guardarArchivoCircular(Estructura* est, string ruta, FuncSerializar serializar) {
    ofstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "  [ERROR] No se pudo abrir: " << ruta << endl;
        return;
    }
    auto aux = est->primero();
    unsigned int n = est->tam();
    for (unsigned int i = 0; i < n; i++) {
        archivo << serializar(aux->elem) << endl;
        aux = aux->sig;
    }
    archivo.close();
    cout << "  Guardado: " << ruta << endl;
}

// ═══════════════════════════════════════════════════════════════
//  cargarArchivo
// ═══════════════════════════════════════════════════════════════
template <typename FuncDeserializar, typename FuncAgregar>
void cargarArchivo(string ruta, FuncDeserializar deserializar, FuncAgregar agregar) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "  [AVISO] No existe aun: " << ruta << endl;
        return;
    }
    string linea;
    int cargados = 0;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        agregar(deserializar(linea));
        cargados++;
    }
    archivo.close();
    cout << "  Cargados " << cargados << " registros desde " << ruta << endl;
}

// ═══════════════════════════════════════════════════════════════
//  guardarLineaArchivo — agrega UNA linea al final (append)
//  Para persistir registros nuevos al instante sin reescribir
// ═══════════════════════════════════════════════════════════════
inline void guardarLineaArchivo(const string& ruta, const string& linea) {
    ofstream archivo(ruta, ios::app);
    if (!archivo.is_open()) {
        cout << "  [ERROR] No se pudo abrir para escritura: " << ruta << endl;
        return;
    }
    archivo << linea << endl;
    archivo.close();
}