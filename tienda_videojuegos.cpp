#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

// ==========================================================
// 1. CONSTANTES Y ESTRUCTURAS DE DATOS
// ==========================================================

const char* ARCHIVO_CATALOGO    = "catalogo.txt";
const char* ARCHIVO_SOLICITUDES = "solicitudes.txt";
const char* ARCHIVO_HISTORIAL   = "historial.txt";
const char  SEPARADOR           = '|';   // separa los campos en los archivos

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

// 2. FUNCIONES AUXILIARES DE ENTRADA Y TEXTO

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

// Convierte texto a entero. Devuelve false si no es un entero valido.
bool textoAEntero(const string& texto, int& valor) {
    istringstream ss(texto);
    char extra;
    return (ss >> valor) && !(ss >> extra);
}

// Convierte texto a decimal. Devuelve false si no es un numero valido.
bool textoADecimal(const string& texto, float& valor) {
    istringstream ss(texto);
    char extra;
    return (ss >> valor) && !(ss >> extra);
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

// Lee un entero por teclado. Devuelve false si lo escrito no es valido.
bool leerEntero(const string& mensaje, int& valor) {
    return textoAEntero(leerLinea(mensaje), valor);
}

// Lee un decimal por teclado. Devuelve false si lo escrito no es valido.
bool leerDecimal(const string& mensaje, float& valor) {
    return textoADecimal(leerLinea(mensaje), valor);
}

// Un texto es valido si no esta vacio y no contiene el separador de archivos
bool textoValido(const string& texto) {
    return !texto.empty() && texto.find(SEPARADOR) == string::npos;
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

// ==========================================================
// 4. LISTA ENLAZADA: CATALOGO DE VIDEOJUEGOS
// ==========================================================

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

// Auxiliar (sin mensajes): enlaza un videojuego al final de la lista
void agregarAlFinal(Nodo*& inicio, Videojuego juego) {
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
    if (!textoValido(nombre) || !textoValido(genero)) {
        cout << "No se permite el caracter '|' en los textos\n";
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
    agregarAlFinal(inicio, juego);

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

    if (!textoValido(nuevoNombre) || !textoValido(nuevoGenero) ||
        !precioValido || nuevoPrecio < 0) {
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

    // Caso especial: eliminar el inicio
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
// ==========================================================

// Auxiliar (sin mensajes): enlaza una solicitud al final de la cola
void encolar(NodoCola*& frente, NodoCola*& final, Solicitud solicitud) {
    NodoCola* nuevo = new NodoCola(solicitud);

    if (frente == NULL) {
        frente = nuevo;
        final = nuevo;
    } else {
        final->siguiente = nuevo;
        final = nuevo;
    }
}

// Enqueue: inserta por el final. Valida datos y que el juego exista.
bool insertarPorElFinal(NodoCola*& frente, NodoCola*& final, Nodo* catalogo,
                        string usuario, int codigoJuego) {
    if (usuario.empty() || codigoJuego <= 0) {
        cout << "Solicitud incompleta\n";
        return false;
    }
    if (!textoValido(usuario)) {
        cout << "No se permite el caracter '|' en los textos\n";
        return false;
    }
    if (buscarPorCodigo(catalogo, codigoJuego) == NULL) {
        cout << "El videojuego solicitado no existe en el catalogo\n";
        return false;
    }

    Solicitud solicitud;
    solicitud.usuario = usuario;
    solicitud.codigoJuego = codigoJuego;
    encolar(frente, final, solicitud);

    cout << "Solicitud registrada\n";
    return true;
}

// Dequeue: procesa la solicitud del frente
void eliminarPorElFrente(NodoCola*& frente, NodoCola*& final) {
    if (frente == NULL) {
        cout << "La cola esta vacia\n";
        return;
    }

    NodoCola* aux = frente;
    Solicitud solicitud = aux->dato;
    frente = frente->siguiente;

    if (frente == NULL) {
        final = NULL;
    }

    delete aux;
    cout << "Solicitud procesada: " << solicitud.usuario
         << " - " << solicitud.codigoJuego << "\n";
}

// Muestra las solicitudes pendientes en orden de llegada
void mostrarSolicitudes(NodoCola* frente) {
    if (frente == NULL) {
        cout << "\nNo hay solicitudes de compra pendientes.\n";
        return;
    }
    cout << "\n=========== COLA DE SOLICITUDES PENDIENTES ===========\n";
    NodoCola* temp = frente;
    int turno = 1;
    while (temp != NULL) {
        cout << "Turno #" << turno++
             << " | Usuario: " << temp->dato.usuario
             << " | Codigo de juego: " << temp->dato.codigoJuego << "\n";
        temp = temp->siguiente;
    }
}

// Cuenta cuantas solicitudes pendientes piden un videojuego
int contarSolicitudesDe(NodoCola* frente, int codigo) {
    int cantidad = 0;
    NodoCola* temp = frente;
    while (temp != NULL) {
        if (temp->dato.codigoJuego == codigo) {
            cantidad++;
        }
        temp = temp->siguiente;
    }
    return cantidad;
}

// ==========================================================
// 6. PERSISTENCIA: GUARDAR Y CARGAR ARCHIVOS DE TEXTO
//    catalogo.txt    -> codigo|nombre|genero|precio   (una linea por juego)
//    solicitudes.txt -> usuario|codigo                (del frente al final)
//    historial.txt   -> texto de la accion            (de la cima a la base)
// ==========================================================

// Divide una linea en campos separados por '|'.
// Devuelve la cantidad de campos (maximo+1 si hay mas de los esperados).
int dividir(const string& linea, string campos[], int maximo) {
    int cantidad = 0;
    size_t inicio = 0;

    while (cantidad < maximo) {
        size_t pos = linea.find(SEPARADOR, inicio);
        if (pos == string::npos) {
            campos[cantidad++] = linea.substr(inicio);
            return cantidad;
        }
        campos[cantidad++] = linea.substr(inicio, pos - inicio);
        inicio = pos + 1;
    }
    return maximo + 1;
}

// ---------- GUARDAR ----------

bool guardarCatalogo(Nodo* inicio) {
    ofstream archivo(ARCHIVO_CATALOGO);
    if (!archivo) {
        return false;
    }
    archivo << fixed << setprecision(2);
    Nodo* temp = inicio;
    while (temp != NULL) {
        archivo << temp->dato.codigo << SEPARADOR
                << temp->dato.nombre << SEPARADOR
                << temp->dato.genero << SEPARADOR
                << temp->dato.precio << "\n";
        temp = temp->siguiente;
    }
    return archivo.good();
}

bool guardarSolicitudes(NodoCola* frente) {
    ofstream archivo(ARCHIVO_SOLICITUDES);
    if (!archivo) {
        return false;
    }
    NodoCola* temp = frente;
    while (temp != NULL) {
        archivo << temp->dato.usuario << SEPARADOR << temp->dato.codigoJuego << "\n";
        temp = temp->siguiente;
    }
    return archivo.good();
}

bool guardarHistorial(NodoPila* cima) {
    ofstream archivo(ARCHIVO_HISTORIAL);
    if (!archivo) {
        return false;
    }
    NodoPila* temp = cima;
    while (temp != NULL) {
        archivo << temp->dato << "\n";
        temp = temp->siguiente;
    }
    return archivo.good();
}

// Guarda las tres estructuras. Avisa si algun archivo no se pudo escribir.
void guardarTodo(Nodo* catalogo, NodoCola* frente, NodoPila* historial) {
    bool ok = guardarCatalogo(catalogo);
    ok = guardarSolicitudes(frente) && ok;
    ok = guardarHistorial(historial) && ok;
    if (!ok) {
        cout << "ADVERTENCIA: no se pudieron guardar los datos en disco.\n";
    }
}

// ---------- CARGAR ----------
// Si un archivo no existe (primera ejecucion) la estructura queda vacia.
// Las lineas danadas o inconsistentes se ignoran y se cuentan en 'descartadas'.

int cargarCatalogo(Nodo*& inicio, int& descartadas) {
    ifstream archivo(ARCHIVO_CATALOGO);
    if (!archivo) {
        return 0;
    }

    int cargados = 0;
    string linea;
    while (getline(archivo, linea)) {
        linea = recortar(linea);
        if (linea.empty()) {
            continue;
        }

        string campos[4];
        Videojuego juego;
        bool valido = dividir(linea, campos, 4) == 4
                      && textoAEntero(campos[0], juego.codigo)
                      && textoADecimal(campos[3], juego.precio)
                      && juego.codigo > 0
                      && juego.precio >= 0
                      && !campos[1].empty()
                      && !campos[2].empty()
                      && buscarPorCodigo(inicio, juego.codigo) == NULL;

        if (valido) {
            juego.nombre = campos[1];
            juego.genero = campos[2];
            agregarAlFinal(inicio, juego);
            cargados++;
        } else {
            descartadas++;
        }
    }
    return cargados;
}

int cargarSolicitudes(NodoCola*& frente, NodoCola*& final, Nodo* catalogo, int& descartadas) {
    ifstream archivo(ARCHIVO_SOLICITUDES);
    if (!archivo) {
        return 0;
    }

    int cargadas = 0;
    string linea;
    while (getline(archivo, linea)) {
        linea = recortar(linea);
        if (linea.empty()) {
            continue;
        }

        string campos[2];
        Solicitud solicitud;
        bool valida = dividir(linea, campos, 2) == 2
                      && !campos[0].empty()
                      && textoAEntero(campos[1], solicitud.codigoJuego)
                      && solicitud.codigoJuego > 0
                      && buscarPorCodigo(catalogo, solicitud.codigoJuego) != NULL;

        if (valida) {
            solicitud.usuario = campos[0];
            encolar(frente, final, solicitud);
            cargadas++;
        } else {
            descartadas++;
        }
    }
    return cargadas;
}

// La primera linea del archivo es la cima, por eso se enlaza al final de la pila
int cargarHistorial(NodoPila*& cima) {
    ifstream archivo(ARCHIVO_HISTORIAL);
    if (!archivo) {
        return 0;
    }

    int cargadas = 0;
    NodoPila* ultimo = NULL;
    string linea;
    while (getline(archivo, linea)) {
        linea = recortar(linea);
        if (linea.empty()) {
            continue;
        }

        NodoPila* nuevo = new NodoPila(linea);
        if (cima == NULL) {
            cima = nuevo;
        } else {
            ultimo->siguiente = nuevo;
        }
        ultimo = nuevo;
        cargadas++;
    }
    return cargadas;
}

// Carga las tres estructuras al iniciar (el catalogo va primero)
void cargarTodo(Nodo*& catalogo, NodoCola*& frente, NodoCola*& final, NodoPila*& historial) {
    int descartadas = 0;
    int juegos = cargarCatalogo(catalogo, descartadas);
    int solicitudes = cargarSolicitudes(frente, final, catalogo, descartadas);
    int acciones = cargarHistorial(historial);

    if (juegos + solicitudes + acciones > 0) {
        cout << "Datos cargados: " << juegos << " videojuego(s), "
             << solicitudes << " solicitud(es), "
             << acciones << " accion(es) de historial.\n";
    }
    if (descartadas > 0) {
        cout << "ADVERTENCIA: se ignoraron " << descartadas
             << " linea(s) invalida(s) en los archivos.\n";
    }
}

// ==========================================================
// 7. LIBERACION DE MEMORIA (RNF-07)
// ==========================================================

void liberarLista(Nodo*& inicio) {
    while (inicio != NULL) {
        Nodo* aux = inicio;
        inicio = inicio->siguiente;
        delete aux;
    }
}

void liberarPila(NodoPila*& cima) {
    while (cima != NULL) {
        NodoPila* aux = cima;
        cima = cima->siguiente;
        delete aux;
    }
}

void liberarCola(NodoCola*& frente, NodoCola*& final) {
    while (frente != NULL) {
        NodoCola* aux = frente;
        frente = frente->siguiente;
        delete aux;
    }
    final = NULL;
}

// ==========================================================
// 8. OPCIONES DEL MENU
//    Leen los datos, llaman a la operacion y, si salio bien,
//    registran la accion en el historial (pila).
// ==========================================================

void opcionRegistrar(Nodo*& catalogo, NodoPila*& historial) {
    cout << "\n--- REGISTRAR NUEVO VIDEOJUEGO ---\n";
    int codigo = 0;
    float precio = 0;

    if (!leerEntero("Codigo: ", codigo)) {
        cout << "Codigo invalido\n";
        return;
    }
    string nombre = leerLinea("Nombre: ");
    string genero = leerLinea("Genero: ");
    if (!leerDecimal("Precio: ", precio)) {
        cout << "Precio invalido\n";
        return;
    }

    if (insertarVideojuego(catalogo, codigo, nombre, genero, precio)) {
        insertarEnCima(historial, "Registro de juego: " + nombre + " (ID: " + aTexto(codigo) + ")");
    }
}

void opcionBuscar(Nodo* catalogo) {
    cout << "\n--- BUSCAR VIDEOJUEGO ---\n";
    cout << "1. Buscar por codigo\n";
    cout << "2. Buscar por nombre\n";

    int opcion = 0;
    if (!leerEntero("Seleccione una opcion: ", opcion)) {
        cout << "Opcion invalida.\n";
        return;
    }

    if (opcion == 1) {
        int codigo = 0;
        if (!leerEntero("Ingrese el codigo a buscar: ", codigo)) {
            cout << "Codigo invalido\n";
            return;
        }
        buscarVideojuego(catalogo, "codigo", aTexto(codigo));
    } else if (opcion == 2) {
        string nombre = leerLinea("Ingrese el nombre a buscar: ");
        buscarVideojuego(catalogo, "nombre", nombre);
    } else {
        cout << "Opcion invalida.\n";
    }
}

void opcionActualizar(Nodo* catalogo, NodoPila*& historial) {
    cout << "\n--- ACTUALIZAR VIDEOJUEGO ---\n";
    int codigo = 0;
    if (!leerEntero("Codigo del videojuego a modificar: ", codigo)) {
        cout << "Codigo invalido\n";
        return;
    }
    if (actualizarVideojuego(catalogo, codigo)) {
        insertarEnCima(historial, "Actualizacion de juego ID: " + aTexto(codigo));
    }
}

void opcionEliminar(Nodo*& catalogo, NodoCola* frente, NodoPila*& historial) {
    cout << "\n--- ELIMINAR VIDEOJUEGO ---\n";
    int codigo = 0;
    if (!leerEntero("Codigo a eliminar: ", codigo)) {
        cout << "Codigo invalido\n";
        return;
    }

    // Integridad: no se elimina un juego que tiene solicitudes pendientes
    int pendientes = contarSolicitudesDe(frente, codigo);
    if (pendientes > 0) {
        cout << "No se puede eliminar: hay " << pendientes
             << " solicitud(es) pendiente(s) para este videojuego\n";
        return;
    }

    // Se guarda el nombre antes de borrar el nodo, para el historial
    string nombre = "";
    Nodo* existente = buscarPorCodigo(catalogo, codigo);
    if (existente != NULL) {
        nombre = existente->dato.nombre;
    }

    if (eliminarVideojuego(catalogo, codigo)) {
        insertarEnCima(historial, "Eliminacion de juego: " + nombre + " (ID: " + aTexto(codigo) + ")");
    }
}

void opcionSolicitud(NodoCola*& frente, NodoCola*& final, Nodo* catalogo) {
    cout << "\n--- REGISTRAR SOLICITUD DE COMPRA ---\n";
    string usuario = leerLinea("Nombre del usuario: ");
    int codigo = 0;
    if (!leerEntero("Codigo del videojuego solicitado: ", codigo)) {
        codigo = 0;   // la funcion lo rechaza como "Solicitud incompleta"
    }
    insertarPorElFinal(frente, final, catalogo, usuario, codigo);
}

// ==========================================================
// 9. PROGRAMA PRINCIPAL
// ==========================================================

int main() {
    Nodo* catalogo = NULL;          // Lista enlazada
    NodoPila* historial = NULL;     // Pila (cima)
    NodoCola* frenteCola = NULL;    // Cola (frente)
    NodoCola* finalCola = NULL;     // Cola (final)

    // Recupera los datos guardados en la ejecucion anterior
    cargarTodo(catalogo, frenteCola, finalCola, historial);

    int opcion = -1;

    do {
        cout << "\n=======================================================\n";
        cout << "  SISTEMA DE GESTION DE TIENDA DIGITAL DE VIDEOJUEGOS  \n";
        cout << "=======================================================\n";
        cout << "1. Registrar Videojuego (Lista)\n";
        cout << "2. Mostrar Catalogo (Lista)\n";
        cout << "3. Buscar Videojuego (Lista)\n";
        cout << "4. Actualizar Videojuego (Lista)\n";
        cout << "5. Eliminar Videojuego (Lista)\n";
        cout << "-------------------------------------------------------\n";
        cout << "6. Registrar Solicitud de Compra (Cola - Enqueue)\n";
        cout << "7. Procesar Solicitud de Compra (Cola - Dequeue)\n";
        cout << "8. Consultar Solicitudes Pendientes (Cola)\n";
        cout << "-------------------------------------------------------\n";
        cout << "9. Ver Historial de Acciones (Pila)\n";
        cout << "10. Retirar Ultima Accion del Historial (Pila - Pop)\n";
        cout << "0. Salir\n";
        cout << "=======================================================\n";

        if (!leerEntero("Ingrese una opcion: ", opcion)) {
            opcion = -1;   // texto no numerico -> cae en "opcion invalida"
        }

        switch (opcion) {
            case 1:  opcionRegistrar(catalogo, historial); break;
            case 2:  mostrarVideojuegos(catalogo); break;
            case 3:  opcionBuscar(catalogo); break;
            case 4:  opcionActualizar(catalogo, historial); break;
            case 5:  opcionEliminar(catalogo, frenteCola, historial); break;
            case 6:  opcionSolicitud(frenteCola, finalCola, catalogo); break;
            case 7:  eliminarPorElFrente(frenteCola, finalCola); break;
            case 8:  mostrarSolicitudes(frenteCola); break;
            case 9:  mostrarHistorial(historial); break;
            case 10: eliminarDeCima(historial); break;
            case 0:  cout << "\nSaliendo del sistema... Hasta luego!\n"; break;
            default: cout << "\nOpcion invalida. Intente de nuevo.\n";
        }

        // Persistencia: despues de toda opcion que puede modificar datos
        if (opcion == 1 || opcion == 4 || opcion == 5 ||
            opcion == 6 || opcion == 7 || opcion == 10) {
            guardarTodo(catalogo, frenteCola, historial);
        }
    } while (opcion != 0);

    // Se libera toda la memoria dinamica antes de terminar
    liberarLista(catalogo);
    liberarPila(historial);
    liberarCola(frenteCola, finalCola);

    return 0;
}
