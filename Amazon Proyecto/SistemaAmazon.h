#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Inventario.h"
#include "Usuario.h"
#include "Cliente.h"
#include "Vendedor.h"
#include "Administrador.h"
#include "Promocion.h"
#include "Categoria.h"
#include "Direccion.h"
#include "Transaccion.h"
#include "Lista.hpp"
#include "Cola.hpp"
#include "Pila.hpp"
#include "ListaCircular.hpp"
#include "SelectionSort.hpp"
#include "InsertionSort.hpp"
#include "ShellSort.hpp"

using namespace std;

class SistemaAmazon {
private:
    Inventario* inventarioGlobal;
    Lista<Usuario*>* usuarios;
    Lista<Categoria*>* categoriasGlobales;
    ListaCircular<Promocion*>* rotacionPromos;
    Cola<Pedido*>* colaDespacho;
    Pila<Pedido*>* despachosRecientes;
    int contadorPedidos;

    // =========================================================
    // RECURSIVIDAD 1
    // =========================================================
    double calcularCostoPedidoRecursivo(Pedido* ped, int pos) {
        if (pos >= ped->getDetalles()->tam()) return 0.0;
        double subtotalActual = ped->getDetalles()->obtener(pos)->calcularSubtotal();
        return subtotalActual + calcularCostoPedidoRecursivo(ped, pos + 1);
    }

    // =========================================================
    // RECURSIVIDAD 3
    // =========================================================
    Usuario* buscarUsuarioRecursivo(int pos, int idABuscar) {
        if (pos >= usuarios->tam()) return nullptr;
        Usuario* u = usuarios->obtener(pos);
        if (u->getId() == idABuscar) return u;
        return buscarUsuarioRecursivo(pos + 1, idABuscar);
    }
public:

    // =========================================================
    // RECURSIVIDAD 2
    // =========================================================
    int calcularStockTotalRecursivo(ListaDoble<Producto*>::Nodo* nodo) {
        if (nodo == nullptr) return 0;
        return nodo->elem->getStock() + calcularStockTotalRecursivo(nodo->sig);
    }

    SistemaAmazon() {
        inventarioGlobal = new Inventario();
        usuarios = new Lista<Usuario*>();
        categoriasGlobales = new Lista<Categoria*>();
        rotacionPromos = new ListaCircular<Promocion*>();
        colaDespacho = new Cola<Pedido*>();
        despachosRecientes = new Pila<Pedido*>();
        contadorPedidos = 1;
    }

    ~SistemaAmazon() {
        delete inventarioGlobal;
        delete usuarios;
        delete categoriasGlobales;
        delete rotacionPromos;
        delete colaDespacho;
        delete despachosRecientes;
    }

    // ── Getters ──────────────────────────────────────────────
    Inventario* getInventario() { return inventarioGlobal; }
    Lista<Usuario*>* getUsuarios() { return usuarios; }
    Lista<Categoria*>* getCategorias() { return categoriasGlobales; }
    ListaCircular<Promocion*>* getPromos() { return rotacionPromos; }
    Cola<Pedido*>* getColaDespacho() { return colaDespacho; }
    Pila<Pedido*>* getDespachosRecientes() { return despachosRecientes; }

    // ── Registro ─────────────────────────────────────────────
    void registrarUsuario(Usuario* u) { usuarios->insertarFinal(u); }
    void registrarProducto(Producto* p) { inventarioGlobal->agregar(p); }
    void registrarPromo(Promocion* p) { rotacionPromos->insertarFinal(p); }
    void registrarCategoria(Categoria* c) { categoriasGlobales->insertarFinal(c); }

    // ── Busqueda ─────────────────────────────────────────────
    Usuario* buscarUsuario(int id) {
        return buscarUsuarioRecursivo(0, id);
    }

    Categoria* buscarCategoria(function<bool(Categoria*)> pred) {
        auto* aux = categoriasGlobales->inicio();
        while (aux != nullptr) {
            if (pred(aux->elem)) return aux->elem;
            aux = aux->sig;
        }
        return nullptr;
    }

    // ── Generar pedido nuevo ─────────────────────────────────
    Pedido* generarPedidoDesdeCarrito(Cliente* c, Direccion* dir, Transaccion* trans) {
        if (c == nullptr || c->getCarrito()->getItems()->tam() == 0) return nullptr;

        Pedido* ped = new Pedido(contadorPedidos++, "2026-05-10");
        
        auto* item = c->getCarrito()->getItems()->inicio();
        while (item != nullptr) {
            Producto* p = item->elem;
            ped->agregarDetalle(new DetallePedido(1, p->getPrecio(), p));
            p->actualizarStock(-1);
            item = item->sig;
        }

        ped->setDireccion(dir);
        ped->setTransaccion(trans);

        c->agregarPedido(ped);
        colaDespacho->encolar(ped);
        c->getCarrito()->vaciar();

        return ped;
    }

    // ── Encolar pedido cargado desde txt (estado Pendiente) ──
    //    Usado al restaurar desde archivos para reconstruir la cola
    void encolarPedidoCargado(Pedido* ped) {
        if (ped->getEstado() == "Pendiente") {
            colaDespacho->encolar(ped);
        }
        // Si ya estaba Confirmado va directo a despachos recientes
        if (ped->getEstado() == "Confirmado") {
            despachosRecientes->apilar(ped);
        }
        // Actualizar contador para que el siguiente pedido nuevo
        // no reutilice un ID ya existente
        if (ped->getId() >= contadorPedidos)
            contadorPedidos = ped->getId() + 1;
    }

    // ── Despacho ─────────────────────────────────────────────
    void atenderSiguientePedido() {
        if (colaDespacho->estaVacia()) return;
        Pedido* ped = colaDespacho->frente();
        ped->confirmarPedido();
        ped->setEnvio(new Envio("TRK" + to_string(ped->getId()), "2026-05-12"));
        despachosRecientes->apilar(ped);
        colaDespacho->desencolar();
    }

    // ── Costo recursivo ──────────────────────────────────────
    double costoTotalPedido(Pedido* p) {
        return calcularCostoPedidoRecursivo(p, 0);
    }

    // ── Ordenamientos ────────────────────────────────────────
    void listarPorNombre() {
        vector<Producto*> v;
        auto* aux = inventarioGlobal->getLista()->inicio();
        while (aux != nullptr) { v.push_back(aux->elem); aux = aux->sig; }
        SelectionSort<Producto*>::ordenar(v, [](Producto* a, Producto* b) {
            return a->getNombre() < b->getNombre();
            });
        cout << "  [Mostrando los " << v.size() << " productos ordenados... esto puede tardar un poco en pantalla]\n";
        for (int i = 0; i < (int)v.size(); i++) v[i]->mostrar();
    }

    void listarPorStock() {
        vector<Producto*> v;
        auto* aux = inventarioGlobal->getLista()->inicio();
        while (aux != nullptr) { v.push_back(aux->elem); aux = aux->sig; }
        InsertionSort<Producto*>::ordenar(v, [](Producto* a, Producto* b) {
            return a->getStock() < b->getStock();
            });
        cout << "  [Mostrando los " << v.size() << " productos ordenados... esto puede tardar un poco en pantalla]\n";
        for (int i = 0; i < (int)v.size(); i++) v[i]->mostrar();
    }

    void listarTopCaros(int n) {
        vector<Producto*> v;
        auto* aux = inventarioGlobal->getLista()->inicio();
        while (aux != nullptr) { v.push_back(aux->elem); aux = aux->sig; }
        ShellSort<Producto*>::ordenar(v, [](Producto* a, Producto* b) {
            return a->getPrecio() > b->getPrecio();
            });
        int mostrados = 0;
        for (int i = 0; i < (int)v.size() && mostrados < n; i++, mostrados++)
            v[i]->mostrar();
    }

    void filtrarCatalogo(function<bool(Producto*)> pred) {
        auto* aux = inventarioGlobal->getLista()->inicio();
        int contador = 0;
        while (aux != nullptr) {
            if (pred(aux->elem)) { aux->elem->mostrar(); contador++; }
            aux = aux->sig;
        }
        cout << "  -- Total encontrados: " << contador << " --\n";
    }
};