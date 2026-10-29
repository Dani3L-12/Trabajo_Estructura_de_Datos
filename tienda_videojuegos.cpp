/*
 * SISTEMA DE GESTION DE TIENDA DIGITAL DE VIDEOJUEGOS
 * Estructura de Datos - Entregable 2
 *
 * Estructuras utilizadas (todas con nodos y punteros):
 *   - Lista enlazada simple : catalogo de videojuegos
 *   - Cola (FIFO)           : solicitudes de compra pendientes
 *   - Pila (LIFO)           : historial de acciones administrativas
 *
 * Compila en C++98 y C++11 (no usa to_string ni nullptr).
 */
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;

// ==========================================================
// 1. ESTRUCTURAS DE DATOS Y NODOS
// ==========================================================

// Datos de un videojuego (catalogo)
struct Videojuego {
    int codigo;
    string nombre;
    string genero;
    float precio;
};

// Nodo de la lista enlazada (catalogo)
struct Nodo {
    Videojuego dato;
    Nodo* siguiente;

    Nodo(Videojuego juego) {
        dato = juego;
        siguiente = NULL;
    }
};

// Nodo de la pila (historial de acciones)
struct NodoPila {
    string dato;
    NodoPila* siguiente;

    NodoPila(string accion) {
        dato = accion;
        siguiente = NULL;
    }
};

// Datos de una solicitud de compra
struct Solicitud {
    string usuario;
    int codigoJuego;
};

// Nodo de la cola (solicitudes de compra)
struct NodoCola {
    Solicitud dato;
    NodoCola* siguiente;

    NodoCola(Solicitud solicitud) {
        dato = solicitud;
        siguiente = NULL;
    }
};

// ==========================================================
// 2. FUNCIONES AUXILIARES DE ENTRADA Y TEXTO
//    Todas las lecturas se hacen con getline: asi nunca queda
//    basura en el buffer ni se bloquea cin al escribir letras.
// ==========================================================

// Convierte un entero a texto (reemplaza a to_string)
string aTexto(int numero) {
    ostringstream ss;
    ss << numero;
    return ss.str();
}

// Quita espacios al inicio y al final
string recortar(const string& texto) {
    size_t ini = texto.find_first_not_of(" \t\r\n");
    if (ini == string::npos) {
        return "";
    }
    size_t fin = texto.find_last_not_of(" \t\r\n");
    return texto.substr(ini, fin - ini + 1);
}

// Lee una linea completa. Si se cierra la entrada, termina el programa.
string leerLinea(const string& mensaje) {
    string linea;
    cout << mensaje;
    if (!getline(cin, linea)) {
        cout << "\nEntrada finalizada. Saliendo del sistema...\n";
        exit(0);
    }
    return recortar(linea);
}

// Lee un entero. Devuelve false si lo escrito no es un entero valido.
bool leerEntero(const string& mensaje, int& valor) {
    istringstream ss(leerLinea(mensaje));
    char extra;
    return (ss >> valor) && !(ss >> extra);
}

// Lee un decimal. Devuelve false si lo escrito no es un numero valido.
bool leerDecimal(const string& mensaje, float& valor) {
    istringstream ss(leerLinea(mensaje));
    char extra;
    return (ss >> valor) && !(ss >> extra);
}

// ==========================================================
// 3. PILA: HISTORIAL DE ACCIONES (LIFO)
// ==========================================================

// Push: inserta una accion en la cima
void insertarEnCima(NodoPila*& cima, string accion) {
    NodoPila* nuevo = new NodoPila(accion);
    nuevo->siguiente = cima;
    cima = nuevo;
    cout << "[Historial] Accion registrada.\n";
}

// Pop: retira la accion de la cima
void eliminarDeCima(NodoPila*& cima) {
    if (cima == NULL) {
        cout << "La pila esta vacia.\n";
        return;
    }
    NodoPila* aux = cima;
    string accion = aux->dato;
    cima = cima->siguiente;
    delete aux;
    cout << "Accion retirada: " << accion << "\n";
}

// Muestra el historial desde la accion mas reciente
void mostrarHistorial(NodoPila* cima) {
    if (cima == NULL) {
        cout << "El historial esta vacio.\n";
        return;
    }
    cout << "\n=== HISTORIAL DE ACCIONES (CIMA -> BASE) ===\n";
    NodoPila* temp = cima;
    while (temp != NULL) {
        cout << "- " << temp->dato << "\n"; 
        temp = temp->siguiente;
    }
}

// 4. LISTA ENLAZADA: CATALOGO DE VIDEOJUEGOS
// Muestra los datos de un videojuego en una linea
void imprimirVideojuego(const Videojuego& juego) {
    cout << "Codigo: " << juego.codigo
         << " | Nombre: " << juego.nombre
         << " | Genero: " << juego.genero
         << " | Precio: S/. " << fixed << setprecision(2) << juego.precio << "\n";
}

// Auxiliar: devuelve el nodo con ese codigo, o NULL si no existe
Nodo* buscarPorCodigo(Nodo* inicio, int codigo) {
    Nodo* temp = inicio;
    while (temp != NULL) {
        if (temp->dato.codigo == codigo) {
            return temp;
        }
        temp = temp->siguiente;
    }
    return NULL;
}

// Insertar al final. Devuelve true si se registro.
bool insertarVideojuego(Nodo*& inicio, int codigo, string nombre, string genero, float precio) {
    if (codigo <= 0) {
        cout << "Codigo invalido\n";
        return false;
    }
    if (precio < 0) {
        cout << "Precio invalido\n";
        return false;
    }
    if (nombre.empty() || genero.empty()) {
        cout << "Datos invalidos\n";
        return false;
    }
    if (buscarPorCodigo(inicio, codigo) != NULL) {
        cout << "Codigo duplicado\n";
        return false;
    }

    Videojuego juego;
    juego.codigo = codigo;
    juego.nombre = nombre;
    juego.genero = genero;
    juego.precio = precio;
    Nodo* nuevo = new Nodo(juego);

    if (inicio == NULL) {
        inicio = nuevo;
    } else {
        Nodo* temp = inicio;
        while (temp->siguiente != NULL) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevo;
    }

    cout << "Videojuego registrado\n";
    return true;
}

// Mostrar todo el catalogo
void mostrarVideojuegos(Nodo* inicio) {
    if (inicio == NULL) {
        cout << "\nEl catalogo de videojuegos esta vacio.\n";
        return;
    }
    cout << "\n================ CATALOGO DE VIDEOJUEGOS ================\n";
    Nodo* temp = inicio;
    while (temp != NULL) {
        imprimirVideojuego(temp->dato);
        temp = temp->siguiente;
    }
}

// Buscar por criterio ("codigo" o "nombre")
void buscarVideojuego(Nodo* inicio, string criterio, string valor) {
    if (inicio == NULL) {
        cout << "La lista esta vacia\n";
        return;
    }

    bool encontrado = false;
    Nodo* temp = inicio;

    while (temp != NULL && !encontrado) {
        if ((criterio == "codigo" && aTexto(temp->dato.codigo) == valor) ||
            (criterio == "nombre" && temp->dato.nombre == valor)) {
            encontrado = true;
        } else {
            temp = temp->siguiente;
        }
    }

    if (encontrado) {
        cout << "Videojuego encontrado:\n";
        imprimirVideojuego(temp->dato);
    } else {
        cout << "Videojuego no encontrado\n";
    }
}

// Actualizar nombre, genero y precio. Devuelve true si se modifico.
bool actualizarVideojuego(Nodo* inicio, int codigo) {
    if (inicio == NULL) {
        cout << "La lista esta vacia\n";
        return false;
    }

    Nodo* temp = inicio;
    while (temp != NULL && temp->dato.codigo != codigo) {
        temp = temp->siguiente;
    }

    if (temp == NULL) {
        cout << "Videojuego no encontrado\n";
        return false;
    }

    string nuevoNombre = leerLinea("Nuevo nombre: ");
    string nuevoGenero = leerLinea("Nuevo genero: ");
    float nuevoPrecio = 0;
    bool precioValido = leerDecimal("Nuevo precio: ", nuevoPrecio);

    if (nuevoNombre.empty() || nuevoGenero.empty() || !precioValido || nuevoPrecio < 0) {
        cout << "Datos invalidos. No se realizaron cambios\n";
        return false;
    }

    temp->dato.nombre = nuevoNombre;
    temp->dato.genero = nuevoGenero;
    temp->dato.precio = nuevoPrecio;
    cout << "Videojuego actualizado\n";
    return true;
}

// Eliminar por codigo. Devuelve true si se elimino.
bool eliminarVideojuego(Nodo*& inicio, int codigo) {
    if (inicio == NULL) {
        cout << "La lista esta vacia\n";
        return false;
    }

    // Caso especial: eliminar la cabeza
    if (inicio->dato.codigo == codigo) {
        Nodo* aux = inicio;
        inicio = inicio->siguiente;
        delete aux;
        cout << "Videojuego eliminado\n";
        return true;
    }

    Nodo* anterior = inicio;
    Nodo* actual = inicio->siguiente;

    while (actual != NULL && actual->dato.codigo != codigo) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual == NULL) {
        cout << "Videojuego no encontrado\n";
        return false;
    }

    anterior->siguiente = actual->siguiente;
    delete actual;
    cout << "Videojuego eliminado\n";
    return true;
}

// ==========================================================
// 5. COLA: SOLICITUDES DE COMPRA (FIFO)
//    Se controla con dos punteros: frente (sale) y final (entra)
// ==========================================================

// Enqueue: inserta una solicitud por el final de la cola.
// Valida que el usuario no este vacio y que codigoJuego > 0.
// Devuelve true si se registro.
bool insertarPorElFinal(NodoCola*& frente, NodoCola*& final, string usuario, int codigoJuego) {
    if (usuario.empty() || codigoJuego <= 0) {
        cout << "Solicitud incompleta\n";
        return false;
    }

    Solicitud solicitud;
    solicitud.usuario = usuario;
    solicitud.codigoJuego = codigoJuego;

    NodoCola* nuevo = new NodoCola(solicitud);

    // Si la cola esta vacia, el nuevo nodo es frente y final a la vez
    if (frente == NULL) {
        frente = nuevo;
        final = nuevo;
    } else {
        final->siguiente = nuevo;
        final = nuevo;
    }

    cout << "Solicitud registrada\n";
    return true;
}

// Dequeue: atiende y procesa la solicitud mas antigua (la del frente)
void eliminarPorElFrente(NodoCola*& frente, NodoCola*& final) {
    if (frente == NULL) {
        cout << "La cola esta vacia\n";
        return;
    }

    NodoCola* aux = frente;
    Solicitud solicitud = aux->dato;

    frente = frente->siguiente;

    // Si ya no quedan solicitudes, final tambien debe quedar en NULL
    if (frente == NULL) {
        final = NULL;
    }

    delete aux;

    cout << "Solicitud procesada: " << solicitud.usuario
         << " - " << solicitud.codigoJuego << "\n";
}

// Muestra las solicitudes pendientes, desde la mas antigua
void consultarSolicitudes(NodoCola* frente) {
    if (frente == NULL) {
        cout << "La cola esta vacia\n";
        return;
    }

    cout << "\n--- SOLICITUDES PENDIENTES ---\n";
    int posicion = 1;
    NodoCola* temp = frente;

    while (temp != NULL) {
        cout << posicion << ". Usuario: " << temp->dato.usuario
             << " | Codigo del juego: " << temp->dato.codigoJuego << "\n";
        temp = temp->siguiente;
        posicion++;
    }
}

// ==========================================================
// 6. LIBERACION DE MEMORIA DINAMICA
//    Se llaman al salir del programa: cada nodo creado con
//    new se elimina con delete para evitar fugas de memoria.
// ==========================================================

// Libera todos los nodos del catalogo
void liberarCatalogo(Nodo*& inicio) {
    while (inicio != NULL) {
        Nodo* aux = inicio;
        inicio = inicio->siguiente;
        delete aux;
    }
}

// Libera todos los nodos del historial
void liberarPila(NodoPila*& cima) {
    while (cima != NULL) {
        NodoPila* aux = cima;
        cima = cima->siguiente;
        delete aux;
    }
}

// Libera todos los nodos de la cola y deja frente y final en NULL
void liberarCola(NodoCola*& frente, NodoCola*& final) {
    while (frente != NULL) {
        NodoCola* aux = frente;
        frente = frente->siguiente;
        delete aux;
    }
    final = NULL;
}



