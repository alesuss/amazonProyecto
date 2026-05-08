#include "pch.h"
#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include <vector>
#include <ctime>
#include <algorithm>

#include "Cliente.h"
#include "Vendedor.h"
#include "Administrador.h"
#include "Producto.h"
#include "Pedido.h"
#include "Promocion.h"
#include "SistemaAmazon.h"
#include "Util.h"
#include "CarritoCompras.h"
#include "Direccion.h"
#include "Transaccion.h"
#include "Categoria.h"
#include "Resena.h"

using namespace std;

// ═══════════════════════════════════════════════════════════════
//  RUTAS DE ARCHIVOS TXT
// ═══════════════════════════════════════════════════════════════
const string ARCH_CLIENTES = "clientes.txt";
const string ARCH_VENDEDORES = "vendedores.txt";
const string ARCH_PRODUCTOS = "productos.txt";
const string ARCH_PEDIDOS = "pedidos.txt";
const string ARCH_DESPACHOS = "despachos.txt";
const string ARCH_PROMOS = "promociones.txt";
const string ARCH_CATEGORIAS = "categorias.txt";

// ═══════════════════════════════════════════════════════════════
//  UTILIDADES DE CONSOLA
// ═══════════════════════════════════════════════════════════════
void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pausar() {
    cout << "\n  Presione ENTER para continuar...";
    cin.get();
}

void titulo(string t) {
    cout << "\n=================================================\n";
    cout << "  " << t << "\n";
    cout << "=================================================\n";
}

// ═══════════════════════════════════════════════════════════════
//  SERIALIZADORES / DESERIALIZADORES
// ═══════════════════════════════════════════════════════════════

// --- Clientes ---
auto serializarCliente = [](Cliente* c) {
    string linea = to_string(c->getId()) + ";" + c->getNombre() + ";" + c->getEmail() + ";" + c->getPassword() + ";" + to_string(c->getDirecciones()->tam());
    auto* aux = c->getDirecciones()->inicio();
    while (aux != nullptr) {
        linea += ";" + aux->elem->getCalle() + ";" + aux->elem->getCiudad() + ";" + aux->elem->getCp();
        aux = aux->sig;
    }
    return linea;
    };
auto deserializarCliente = [](string linea) -> Cliente* {
    stringstream ss(linea); string id, n, e, p, numDir;
    getline(ss, id, ';'); getline(ss, n, ';'); getline(ss, e, ';'); getline(ss, p, ';');
    Cliente* c = new Cliente(stoi(id), n, e, p);
    if (getline(ss, numDir, ';')) {
        int nd = stoi(numDir);
        for(int i=0; i<nd; i++) {
            string ca, ci, cp;
            getline(ss, ca, ';'); getline(ss, ci, ';'); getline(ss, cp, ';');
            c->agregarDireccion(new Direccion(ca, ci, cp));
        }
    }
    return c;
    };

// --- Vendedores ---
auto serializarVendedor = [](Vendedor* v) {
    return to_string(v->getId()) + ";" + v->getNombre() + ";" + v->getEmail() + ";" + v->getPassword();
    };
auto deserializarVendedor = [](string linea) -> Vendedor* {
    stringstream ss(linea); string id, n, e, p;
    getline(ss, id, ';'); getline(ss, n, ';'); getline(ss, e, ';'); getline(ss, p, ';');
    return new Vendedor(stoi(id), n, e, p);
    };

// --- Productos ---
auto serializarProducto = [](Producto* p) {
    string cat = p->getCategoria() ? p->getCategoria()->getNombre() : "NULL";
    string linea = p->getSku() + ";" + p->getNombre() + ";" + to_string(p->getPrecio()) + ";" + to_string(p->getStock()) + ";" + cat + ";" + to_string(p->getResenas()->tam());
    auto* aux = p->getResenas()->inicio();
    while (aux != nullptr) {
        linea += ";" + to_string(aux->elem->getEstrellas()) + ";" + aux->elem->getComentario();
        aux = aux->sig;
    }
    return linea;
    };
auto deserializarProducto = [](string linea) -> Producto* {
    stringstream ss(linea); string sku, n, p, s;
    getline(ss, sku, ';'); getline(ss, n, ';'); getline(ss, p, ';'); getline(ss, s, ';');
    return new Producto(sku, n, stof(p), stoi(s));
    };

// --- Promociones ---
auto serializarPromo = [](Promocion* pr) {
    return pr->getCodigo() + ";" + to_string(pr->getDescuento());
    };
auto deserializarPromo = [](string linea) -> Promocion* {
    stringstream ss(linea); string cod, desc;
    getline(ss, cod, ';'); getline(ss, desc, ';');
    return new Promocion(cod, stof(desc));
    };

// --- Pedidos ---
// Formato: idPedido;fecha;estado;idCliente;sku;cantidad;precioUnit[;sku;cantidad;precioUnit...]
string serializarPedido(Pedido* ped, int idCliente) {
    string tr = ped->getTransaccion() ? ped->getTransaccion()->getId() + ";" + ped->getTransaccion()->getMetodo() : "NULL;NULL";
    string dir = ped->getDireccion() ? ped->getDireccion()->getCalle() + ";" + ped->getDireccion()->getCiudad() + ";" + ped->getDireccion()->getCp() : "NULL;NULL;NULL";
    string linea = to_string(ped->getId()) + ";" + ped->getFecha() + ";" + ped->getEstado() + ";" + to_string(idCliente) + ";" + tr + ";" + dir;
    auto* det = ped->getDetalles()->inicio();
    while (det != nullptr) {
        linea += ";" + det->elem->getProducto()->getSku()
            + ";" + to_string(det->elem->getCantidad())
            + ";" + to_string(det->elem->getPrecioUnitario());
        det = det->sig;
    }
    return linea;
}

// ═══════════════════════════════════════════════════════════════
//  GUARDAR TODO EL ESTADO
// ═══════════════════════════════════════════════════════════════
void guardarTodo(SistemaAmazon* amazon) {
    // Separar clientes y vendedores
    Lista<Cliente*>  clientes;
    Lista<Vendedor*> vendedores;
    auto* aux = amazon->getUsuarios()->inicio();
    while (aux != nullptr) {
        if (auto* c = dynamic_cast<Cliente*>(aux->elem))       clientes.insertarFinal(c);
        else if (auto* v = dynamic_cast<Vendedor*>(aux->elem)) vendedores.insertarFinal(v);
        aux = aux->sig;
    }

    guardarArchivo(&clientes, ARCH_CLIENTES, serializarCliente);
    guardarArchivo(&vendedores, ARCH_VENDEDORES, serializarVendedor);
    guardarArchivo(amazon->getCategorias(), ARCH_CATEGORIAS, [](Categoria* c) { return c->getNombre(); });
    guardarArchivo(amazon->getInventario()->getLista(), ARCH_PRODUCTOS, serializarProducto);
    guardarArchivoCircular(amazon->getPromos(), ARCH_PROMOS, serializarPromo);

    // Pedidos: recorrer historial de cada cliente
    ofstream fPed(ARCH_PEDIDOS);
    if (fPed.is_open()) {
        auto* uc = amazon->getUsuarios()->inicio();
        while (uc != nullptr) {
            if (auto* c = dynamic_cast<Cliente*>(uc->elem)) {
                int n = c->getHistorial()->tam();
                for (int i = 0; i < n; i++)
                    fPed << serializarPedido(c->getHistorial()->obtener(i), c->getId()) << "\n";
            }
            uc = uc->sig;
        }
        fPed.close();
        cout << "  Guardado: " << ARCH_PEDIDOS << endl;
    }

    // Despachos recientes (pila)
    guardarArchivo(amazon->getDespachosRecientes(), ARCH_DESPACHOS,
        [](Pedido* p) { return to_string(p->getId()) + ";" + p->getFecha() + ";" + p->getEstado(); });
}

// ═══════════════════════════════════════════════════════════════
//  CARGAR TODO EL ESTADO DESDE TXT
// ═══════════════════════════════════════════════════════════════
void cargarTodo(SistemaAmazon* amazon) {
    cargarArchivo(ARCH_CLIENTES, deserializarCliente, [&](Cliente* c) { amazon->registrarUsuario(c); });
    cargarArchivo(ARCH_VENDEDORES, deserializarVendedor, [&](Vendedor* v) { amazon->registrarUsuario(v); });
    cargarArchivo(ARCH_CATEGORIAS, [](string linea) -> Categoria* { return new Categoria(linea); }, [&](Categoria* c) { amazon->registrarCategoria(c); });
    
    cargarArchivo(ARCH_PRODUCTOS, 
        [&](string linea) -> Producto* {
            stringstream ss(linea); string sku, n, p, s, catName, numRes;
            getline(ss, sku, ';'); getline(ss, n, ';'); getline(ss, p, ';'); getline(ss, s, ';');
            getline(ss, catName, ';'); getline(ss, numRes, ';');
            Producto* prod = new Producto(sku, n, stof(p), stoi(s));
            if (catName != "NULL" && catName != "") {
                Categoria* c = amazon->buscarCategoria([catName](Categoria* cat){ return cat->getNombre() == catName; });
                if (c) prod->setCategoria(c);
            }
            if (numRes != "" && numRes != "0") {
                int nr = stoi(numRes);
                for(int i=0; i<nr; i++) {
                    string est, com;
                    getline(ss, est, ';'); getline(ss, com, ';');
                    prod->agregarResena(new Resena(stoi(est), com));
                }
            }
            return prod;
        }, 
        [&](Producto* p) { amazon->registrarProducto(p); });

    cargarArchivo(ARCH_PROMOS, deserializarPromo, [&](Promocion* pr) { amazon->registrarPromo(pr); });

    // Cargar pedidos y reconstruir historial + cola
    ifstream fPed(ARCH_PEDIDOS);
    if (fPed.is_open()) {
        string linea; int cargados = 0;
        while (getline(fPed, linea)) {
            if (linea.empty()) continue;
            stringstream ss(linea);
            string sId, fecha, estado, sIdC, trId, trMet, dirC, dirCi, dirCp;
            getline(ss, sId, ';'); getline(ss, fecha, ';');
            getline(ss, estado, ';'); getline(ss, sIdC, ';');
            getline(ss, trId, ';'); getline(ss, trMet, ';');
            getline(ss, dirC, ';'); getline(ss, dirCi, ';'); getline(ss, dirCp, ';');

            Pedido* ped = new Pedido(stoi(sId), fecha);
            if (estado == "Confirmado") ped->confirmarPedido();
            if (trId != "NULL") ped->setTransaccion(new Transaccion(trId, trMet));
            if (dirC != "NULL") ped->setDireccion(new Direccion(dirC, dirCi, dirCp));

            // detalles
            string sku, sCant, sPrec;
            while (getline(ss, sku, ';')) {
                if (!getline(ss, sCant, ';')) break;
                if (!getline(ss, sPrec, ';')) break;
                Producto* prod = amazon->getInventario()->buscarProducto(
                    [sku](Producto* p) { return p->getSku() == sku; });
                if (prod) ped->agregarDetalle(new DetallePedido(stoi(sCant), stof(sPrec), prod));
            }

            Cliente* c = dynamic_cast<Cliente*>(amazon->buscarUsuario(stoi(sIdC)));
            if (c) c->agregarPedido(ped);
            amazon->encolarPedidoCargado(ped);
            cargados++;
        }
        fPed.close();
        cout << "  Cargados " << cargados << " pedidos desde " << ARCH_PEDIDOS << endl;
    }
    else {
        cout << "  [AVISO] No existe aun: " << ARCH_PEDIDOS << endl;
    }
}

// ═══════════════════════════════════════════════════════════════
//  DATOS DEMO
// ═══════════════════════════════════════════════════════════════
void cargarDatosDemo(SistemaAmazon* s) {
    s->registrarUsuario(new Cliente(1, "Lenin Aguirre", "lenin@upc.edu.pe", "123"));
    s->registrarUsuario(new Cliente(2, "Maria Lopez", "maria@mail.com", "123"));
    s->registrarUsuario(new Vendedor(3, "TechStore", "ventas@tech.com", "123"));

    cout << "  Generando 50,000 clientes aleatorios...\n";
    for(int i = 4; i <= 50003; i++) {
        string dominio = (i % 2 == 0) ? "@gmail.com" : "@hotmail.com";
        s->registrarUsuario(new Cliente(i, "Cliente Masivo " + to_string(i), "cliente" + to_string(i) + dominio, "123"));
    }

    s->registrarProducto(new Producto("A1", "Laptop Gamer", 4500.0, 10));
    s->registrarProducto(new Producto("B2", "Mouse Optico", 80.0, 50));
    s->registrarProducto(new Producto("C3", "Monitor 4K", 1200.0, 0));
    s->registrarProducto(new Producto("D4", "Teclado Mecanico", 350.0, 15));

    cout << "  Generando 50,000 productos aleatorios...\n";
    for(int i = 5; i <= 50004; i++) {
        s->registrarProducto(new Producto(
            "PRD-" + to_string(i),
            "Producto Masivo " + to_string(i),
            (rand() % 10000) / 10.0f,
            rand() % 500
        ));
    }

    s->registrarPromo(new Promocion("SUMMER26", 15.0));
    s->registrarPromo(new Promocion("CYBERAMZ", 30.0));
}

// ═══════════════════════════════════════════════════════════════
//  MENU
// ═══════════════════════════════════════════════════════════════
void menu() {
    cout << "\n=================================================\n";
    cout << "  Sistema de Gestion de Inventarios - Amazon\n";
    cout << "=================================================\n";
    cout << "  1. Ver catalogo de productos\n";
    cout << "  2. Ver usuarios registrados\n";
    cout << "  3. Ver rotacion de promociones (circular)\n";
    cout << "  4. Generar pedido (cliente con autenticacion)\n";
    cout << "  5. Ver cola de pedidos pendientes\n";
    cout << "  6. Despachar siguiente pedido\n";
    cout << "  7. Ver despachos recientes (Pila)\n";
    cout << "  8. Ver historial de compras (adelante)\n";
    cout << "  9. Ver historial de compras (atras)\n";
    cout << " 10. Costo total de un pedido (recursivo)\n";
    cout << " 11. Productos ordenados por nombre (Selection Sort)\n";
    cout << " 12. Productos ordenados por stock (Insertion Sort)\n";
    cout << " 13. Top-N productos mas caros (Shell Sort)\n";
    cout << " 14. Filtrar solo productos con stock (lambda)\n";
    cout << " 15. Filtrar productos mayores a cierto precio (lambda)\n";
    cout << " 16. Guardar TODO en archivos .txt\n";
    cout << " 17. Cargar TODO desde archivos .txt\n";
    cout << " 18. Contar usuarios que son clientes\n";
    cout << " 19. Invertir despachos recientes (Pila::invertir)\n";
    cout << " 20. Recorrer cola con lambda\n";
    cout << " 21. Registrar nuevo cliente\n";
    cout << " 22. Registrar nuevo vendedor\n";
    cout << " 23. Registrar nuevo producto\n";
    cout << " 24. Anadir al carrito y hacer checkout (Cliente)\n";
    cout << " 25. Dejar resena a un producto (Cliente)\n";
    cout << " 26. Crear nueva categoria (Administrativo)\n";
    cout << " 27. Calcular stock total en inventario (Recursividad)\n";
    cout << " 28. Filtrar clientes por dominio de correo (Lambda)\n";
    cout << " 29. Ver productos mejor valorados (> 4 estrellas) (Lambda)\n";
    cout << "  0. Guardar y Salir\n";
    cout << "-------------------------------------------------\n";
    cout << "  Opcion: ";
}

// ═══════════════════════════════════════════════════════════════
//  MAIN
// ═══════════════════════════════════════════════════════════════
int main() {
    srand(time(NULL));

    // Crear carpeta data si no existe
#ifdef _WIN32
    system("if not exist data mkdir data");
#else
    system("mkdir -p data");
#endif

    SistemaAmazon* amazon = new SistemaAmazon();

    // Si clientes.txt existe y tiene contenido -> cargar; sino -> cargar demo
    ifstream check(ARCH_CLIENTES);
    bool hayDatos = check.is_open() && check.peek() != ifstream::traits_type::eof();
    check.close();

    if (hayDatos) {
        titulo("Cargando datos desde archivos .txt");
        cargarTodo(amazon);
        cout << "\n  Sistema restaurado desde archivos.\n";
    }
    else {
        cargarDatosDemo(amazon);
        cout << "\n  Primera ejecucion: datos demo cargados.\n";
        guardarTodo(amazon);
        cout << "  Datos demo guardados en /data\n";
    }

    int op = -1;
    while (op != 0) {
        menu();
        if (!(cin >> op)) { limpiarBuffer(); op = -1; continue; }
        limpiarBuffer();

        switch (op) {

            // ── 1. Catalogo ────────────────────────────────────────
        case 1: {
            titulo("Catalogo de productos");
            amazon->filtrarCatalogo([](Producto* p) { return true; });
            pausar(); break;
        }

              // ── 2. Usuarios ────────────────────────────────────────
        case 2: {
            titulo("Usuarios registrados");
            auto* aux = amazon->getUsuarios()->inicio();
            if (aux == nullptr) cout << "  (vacio)\n";
            while (aux != nullptr) { aux->elem->mostrarPerfil(); aux = aux->sig; }
            pausar(); break;
        }

              // ── 3. Promociones ─────────────────────────────────────
        case 3: {
            titulo("Rotacion de promociones (Lista Circular)");
            auto* p = amazon->getPromos()->primero();
            if (p == nullptr) { cout << "  (sin promociones)\n"; pausar(); break; }
            int cant = amazon->getPromos()->tam();
            for (int i = 0; i < cant; i++) {
                cout << "  " << i + 1 << ". " << p->elem->getCodigo()
                    << "  -" << p->elem->getDescuento() << "%\n";
                p = p->sig;
            }
            pausar(); break;
        }

              // ── 4. Generar pedido DIRECTO (obsoleto, mantenido por retrocompatibilidad) ────────────────
        case 4: {
            titulo("Generar pedido directo (Legacy)");
            cout << "  ID del cliente: ";
            int id; cin >> id; limpiarBuffer();

            Cliente* c = dynamic_cast<Cliente*>(amazon->buscarUsuario(id));
            if (c == nullptr) {
                cout << "  Error: no existe un cliente con ese ID.\n";
                pausar(); break;
            }
            cout << "  Contrasena: ";
            string passIngresada; getline(cin, passIngresada);
            if (passIngresada != c->getPassword()) {
                cout << "  Error: contrasena incorrecta.\n";
                pausar(); break;
            }
            cout << "  Bienvenido, " << c->getNombre() << "!\n";
            cout << "  SKU del producto: ";
            string sku; getline(cin, sku);

            // Temporal variables for legacy compatibility
            Direccion* dirTemporal = new Direccion("Calle Falsa 123", "Lima", "15001");
            Transaccion* transTemporal = new Transaccion("TRX-" + to_string(rand()%1000), "Efectivo");
            c->getCarrito()->agregarItem(amazon->getInventario()->buscarProducto([sku](Producto* p) { return p->getSku() == sku; }));

            Pedido* p = amazon->generarPedidoDesdeCarrito(c, dirTemporal, transTemporal);
            if (p == nullptr) {
                cout << "  Error: SKU incorrecto o producto sin stock.\n";
            }
            else {
                cout << "  Pedido generado: "; p->mostrar();
                // Guardar el nuevo pedido y el stock actualizado de inmediato
                // NOTA: serializarPedido asume una forma, no manejamos guardar direcciones por brevedad en este caso
            }
            pausar(); break;
        }

              // ── 5. Cola de pedidos ─────────────────────────────────
        case 5: {
            titulo("Cola de despachos pendientes (FIFO)");
            if (amazon->getColaDespacho()->estaVacia()) cout << "  (sin pedidos)\n";
            amazon->getColaDespacho()->mostrarCon([](Pedido* p) { cout << "  "; p->mostrar(); });
            pausar(); break;
        }

              // ── 6. Despachar ───────────────────────────────────────
        case 6: {
            titulo("Despachar siguiente pedido");
            if (amazon->getColaDespacho()->estaVacia()) {
                cout << "  No hay pedidos en cola.\n";
            }
            else {
                cout << "  Despachando: "; amazon->getColaDespacho()->frente()->mostrar();
                amazon->atenderSiguientePedido();
                // Reescribir pedidos y despachos para reflejar el nuevo estado
                guardarTodo(amazon);
                cout << "  (despacho guardado en " << ARCH_DESPACHOS << ")\n";
            }
            pausar(); break;
        }

              // ── 7. Despachos recientes ─────────────────────────────
        case 7: {
            titulo("Despachos recientes (Pila - LIFO)");
            auto* aux = amazon->getDespachosRecientes()->inicio();
            if (aux == nullptr) cout << "  (sin despachos)\n";
            while (aux != nullptr) { cout << "  "; aux->elem->mostrar(); aux = aux->sig; }
            pausar(); break;
        }

              // ── 8. Historial adelante ──────────────────────────────
        case 8: {
            titulo("Historial de compras (adelante)");
            cout << "  ID del cliente: "; int id; cin >> id; limpiarBuffer();
            Cliente* c = dynamic_cast<Cliente*>(amazon->buscarUsuario(id));
            if (c == nullptr || c->getHistorial()->tam() == 0) cout << "  Sin historial.\n";
            else { int n = c->getHistorial()->tam(); for (int i = 0;i < n;i++) { cout << "  ";c->getHistorial()->obtener(i)->mostrar(); } }
            pausar(); break;
        }

              // ── 9. Historial atras ─────────────────────────────────
        case 9: {
            titulo("Historial de compras (atras)");
            cout << "  ID del cliente: "; int id; cin >> id; limpiarBuffer();
            Cliente* c = dynamic_cast<Cliente*>(amazon->buscarUsuario(id));
            if (c == nullptr || c->getHistorial()->tam() == 0) cout << "  Sin historial.\n";
            else { int n = c->getHistorial()->tam(); for (int i = n - 1;i >= 0;i--) { cout << "  ";c->getHistorial()->obtener(i)->mostrar(); } }
            pausar(); break;
        }

              // ── 10. Costo recursivo ────────────────────────────────
        case 10: {
            titulo("Costo total de pedido (recursivo)");
            cout << "  ID del cliente: "; int id; cin >> id; limpiarBuffer();
            Cliente* c = dynamic_cast<Cliente*>(amazon->buscarUsuario(id));
            if (c == nullptr || c->getHistorial()->tam() == 0) cout << "  No tiene pedidos.\n";
            else {
                Pedido* ultimo = c->getHistorial()->obtener(c->getHistorial()->tam() - 1);
                cout << "  Costo del pedido #" << ultimo->getId() << ": S/. " << amazon->costoTotalPedido(ultimo) << "\n";
            }
            pausar(); break;
        }

               // ── 11-13. Sorts ───────────────────────────────────────
        case 11: titulo("Productos por nombre (Selection Sort)"); amazon->listarPorNombre(); pausar(); break;
        case 12: titulo("Productos por stock (Insertion Sort)");  amazon->listarPorStock();  pausar(); break;
        case 13: {
            titulo("Top-N productos caros (Shell Sort)");
            cout << "  N: "; int n; cin >> n; limpiarBuffer();
            amazon->listarTopCaros(n); pausar(); break;
        }

               // ── 14-15. Filtros ─────────────────────────────────────
        case 14:
            titulo("Filtro: productos con stock");
            amazon->filtrarCatalogo([](Producto* p) { return p->getStock() > 0; });
            pausar(); break;
        case 15: {
            titulo("Filtro: productos mayores a cierto precio");
            cout << "  Precio minimo: "; float minP; cin >> minP; limpiarBuffer();
            amazon->filtrarCatalogo([minP](Producto* p) { return p->getPrecio() > minP; });
            pausar(); break;
        }

               // ── 16. Guardar todo manual ────────────────────────────
        case 16: {
            titulo("Guardar TODO en archivos .txt");
            guardarTodo(amazon);
            cout << "  Todos los archivos actualizados.\n";
            pausar(); break;
        }

               // ── 17. Cargar todo manual ─────────────────────────────
        case 17: {
            titulo("Cargar TODO desde archivos .txt");
            delete amazon;
            amazon = new SistemaAmazon();
            cargarTodo(amazon);
            cout << "  Sistema recargado desde archivos.\n";
            pausar(); break;
        }

               // ── 18. Contar clientes ────────────────────────────────
        case 18: {
            titulo("Contar clientes (Lista::contarSi)");
            int total = amazon->getUsuarios()->contarSi([](Usuario* u) { return u->rol() == "Cliente"; });
            cout << "  Total de clientes: " << total << "\n";
            pausar(); break;
        }

               // ── 19. Invertir pila ──────────────────────────────────
        case 19: {
            titulo("Invertir despachos recientes (Pila::invertir)");
            amazon->getDespachosRecientes()->invertir();
            cout << "  Pila invertida.\n";
            pausar(); break;
        }

               // ── 20. Cola con lambda ────────────────────────────────
        case 20: {
            titulo("Recorrer cola con lambda");
            amazon->getColaDespacho()->mostrarCon([](Pedido* p) {
                cout << "  -> Pedido #" << p->getId() << " (" << p->getEstado() << ")\n";
                });
            pausar(); break;
        }
               // ── 21. Registrar cliente ──────────────────────────────
        case 21: {
            titulo("Registrar nuevo cliente");
            int id; string nombre, email, pass;
            cout << "  ID      : "; cin >> id; limpiarBuffer();
            cout << "  Nombre  : "; getline(cin, nombre);
            cout << "  Email   : "; getline(cin, email);
            cout << "  Password: "; getline(cin, pass);
            Cliente* nuevo = new Cliente(id, nombre, email, pass);
            amazon->registrarUsuario(nuevo);
            // Guardar inmediatamente al final del txt
            guardarLineaArchivo(ARCH_CLIENTES, serializarCliente(nuevo));
            cout << "  Cliente guardado en " << ARCH_CLIENTES << "\n";
            pausar(); break;
        }

               // ── 22. Registrar vendedor ─────────────────────────────
        case 22: {
            titulo("Registrar nuevo vendedor");
            int id; string nombre, email, pass;
            cout << "  ID      : "; cin >> id; limpiarBuffer();
            cout << "  Nombre  : "; getline(cin, nombre);
            cout << "  Email   : "; getline(cin, email);
            cout << "  Password: "; getline(cin, pass);
            Vendedor* nuevo = new Vendedor(id, nombre, email, pass);
            amazon->registrarUsuario(nuevo);
            guardarLineaArchivo(ARCH_VENDEDORES, serializarVendedor(nuevo));
            cout << "  Vendedor guardado en " << ARCH_VENDEDORES << "\n";
            pausar(); break;
        }

               // ── 23. Registrar producto ─────────────────────────────
        case 23: {
            titulo("Registrar nuevo producto");
            string sku, nombre; float precio; int stock;
            cout << "  SKU    : "; getline(cin, sku);
            cout << "  Nombre : "; getline(cin, nombre);
            cout << "  Precio : "; cin >> precio; limpiarBuffer();
            cout << "  Stock  : "; cin >> stock;  limpiarBuffer();
            Producto* nuevo = new Producto(sku, nombre, precio, stock);
            amazon->registrarProducto(nuevo);
            guardarLineaArchivo(ARCH_PRODUCTOS, serializarProducto(nuevo));
            cout << "  Producto guardado en " << ARCH_PRODUCTOS << "\n";
            pausar(); break;
        }

               // ── 24. Añadir al carrito y hacer checkout ─────────────
        case 24: {
            titulo("Añadir al carrito y Checkout");
            cout << "  ID del cliente: ";
            int id; cin >> id; limpiarBuffer();
            Cliente* c = dynamic_cast<Cliente*>(amazon->buscarUsuario(id));
            if (!c) { cout << "  Cliente no encontrado.\n"; pausar(); break; }
            
            int subop = 1;
            while(subop != 0) {
                cout << "  1. Agregar SKU al carrito\n";
                cout << "  2. Ir a Checkout\n";
                cout << "  0. Cancelar\n";
                cout << "  Opcion: "; cin >> subop; limpiarBuffer();
                
                if (subop == 1) {
                    cout << "  SKU a agregar: "; string sku; getline(cin, sku);
                    Producto* p = amazon->getInventario()->buscarProducto([sku](Producto* pr) { return pr->getSku() == sku; });
                    if (p && p->getStock() > 0) {
                        c->getCarrito()->agregarItem(p);
                        cout << "  Producto agregado al carrito. (Total temp: " << c->getCarrito()->calcularTotal() << ")\n";
                    } else {
                        cout << "  Producto no encontrado o sin stock.\n";
                    }
                } else if (subop == 2) {
                    if (c->getCarrito()->getItems()->tam() == 0) {
                        cout << "  El carrito esta vacio.\n";
                        break;
                    }
                    cout << "  --- CHECKOUT ---\n";
                    cout << "  Direccion (Calle): "; string calle; getline(cin, calle);
                    cout << "  Ciudad: "; string ciudad; getline(cin, ciudad);
                    cout << "  Codigo Postal: "; string cp; getline(cin, cp);
                    Direccion* d = new Direccion(calle, ciudad, cp);
                    c->agregarDireccion(d);
                    
                    cout << "  Metodo de Pago (Ej. Tarjeta, Paypal): "; string metodo; getline(cin, metodo);
                    Transaccion* t = new Transaccion("TRX-" + to_string(rand()%10000), metodo);
                    
                    Pedido* ped = amazon->generarPedidoDesdeCarrito(c, d, t);
                    if (ped) {
                        cout << "  Checkout exitoso!\n";
                        ped->mostrar();
                    } else {
                        cout << "  Error en checkout.\n";
                    }
                    break;
                }
            }
            pausar(); break;
        }

               // ── 25. Dejar reseña a producto ────────────────────────
        case 25: {
            titulo("Dejar resena a un producto");
            cout << "  SKU del producto: "; string sku; getline(cin, sku);
            Producto* p = amazon->getInventario()->buscarProducto([sku](Producto* pr) { return pr->getSku() == sku; });
            if (!p) { cout << "  Producto no encontrado.\n"; pausar(); break; }
            
            cout << "  Estrellas (1-5): "; int estrellas; cin >> estrellas; limpiarBuffer();
            cout << "  Comentario: "; string comentario; getline(cin, comentario);
            Resena* r = new Resena(estrellas, comentario);
            p->agregarResena(r);
            cout << "  Resena agregada con exito.\n";
            pausar(); break;
        }

               // ── 26. Crear categoría ────────────────────────────────
        case 26: {
            titulo("Crear nueva categoria");
            cout << "  Nombre de categoria: "; string nom; getline(cin, nom);
            Categoria* cat = new Categoria(nom);
            amazon->registrarCategoria(cat);
            cout << "  Categoria creada.\n";
            
            cout << "  Desea asignar a un producto existente? (1=Si, 0=No): "; int asig; cin >> asig; limpiarBuffer();
            if (asig == 1) {
                cout << "  SKU: "; string sku; getline(cin, sku);
                Producto* p = amazon->getInventario()->buscarProducto([sku](Producto* pr) { return pr->getSku() == sku; });
                if (p) {
                    p->setCategoria(cat);
                    cout << "  Categoria asignada al producto.\n";
                } else {
                    cout << "  Producto no encontrado.\n";
                }
            }
            pausar(); break;
        }

               // ── 27. Calcular stock total ─────────────────────────────
        case 27: {
            titulo("Stock Total (Recursivo)");
            int total = amazon->calcularStockTotalRecursivo(amazon->getInventario()->getLista()->inicio());
            cout << "  Stock total en todo el inventario: " << total << " unidades.\n";
            pausar(); break;
        }

               // ── 28. Filtrar clientes por dominio ─────────────────────
        case 28: {
            titulo("Filtrar clientes por dominio de correo");
            cout << "  Dominio a buscar (ej. @gmail.com): "; string dominio; getline(cin, dominio);
            int encontrados = amazon->getUsuarios()->contarSi([dominio](Usuario* u) {
                if (u->rol() == "Cliente") {
                    return u->getEmail().find(dominio) != string::npos;
                }
                return false;
            });
            cout << "  Total de clientes con " << dominio << ": " << encontrados << "\n";
            pausar(); break;
        }

               // ── 29. Productos mejor valorados ────────────────────────
        case 29: {
            titulo("Productos Mejor Valorados (> 4 estrellas)");
            amazon->filtrarCatalogo([](Producto* p) {
                if (p->getResenas()->tam() == 0) return false;
                auto* aux = p->getResenas()->inicio();
                float suma = 0;
                while(aux != nullptr) {
                    suma += aux->elem->getEstrellas();
                    aux = aux->sig;
                }
                return (suma / p->getResenas()->tam()) > 4.0;
            });
            pausar(); break;
        }

               // ── 0. Guardar y salir ─────────────────────────────────
        case 0:
            titulo("Guardando datos antes de salir...");
            guardarTodo(amazon);
            cout << "\n  Hasta luego!\n";
            break;

        default:
            cout << "  Opcion invalida.\n";
            pausar();
        }
    }

    delete amazon;
    return 0;
}