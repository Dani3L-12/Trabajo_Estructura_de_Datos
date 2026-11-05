
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;

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

// Inserta una accion en la cima
void insertarEnCima(NodoPila*& cima, string accion) {
    NodoPila* nuevo = new NodoPila(accion);
    nuevo->siguiente = cima;
    cima = nuevo;
    cout << "[Historial] Accion registrada.\n";
}

// Retira la accion de la cima
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

// Inserta una solicitud por el final de la cola.
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

// Atiende y procesa la solicitud mas antigua (la del frente)
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

// ==========================================================
// 7. PERSISTENCIA (ARCHIVOS)
//    Los datos se guardan en archivos de texto en la carpeta
//    del ejecutable y se recuperan al iniciar el programa.
//    Formato de cada archivo:
//      catalogo.txt    : codigo|nombre|genero|precio
//      solicitudes.txt : usuario|codigoJuego (frente -> final)
//      historial.txt   : una accion por linea (cima -> base)
// ==========================================================

// Nombres de los archivos de texto
const char* ARCHIVO_CATALOGO = "catalogo.txt";
const char* ARCHIVO_SOLICITUDES = "solicitudes.txt";
const char* ARCHIVO_HISTORIAL = "historial.txt";

// Convierte texto a entero sin imprimir nada (para la carga)
bool convertirEntero(const string& texto, int& valor) {
    istringstream ss(texto);
    char extra;
    return (ss >> valor) && !(ss >> extra);
}

// Convierte texto a decimal sin imprimir nada (para la carga)
bool convertirDecimal(const string& texto, float& valor) {
    istringstream ss(texto);
    char extra;
    return (ss >> valor) && !(ss >> extra);
}

// Separa una linea por el caracter '|'. Devuelve false si hay
// mas campos de los permitidos. "cantidad" recibe cuantos salieron.
bool separarCampos(const string& linea, string campos[], int maximo, int& cantidad) {
    cantidad = 0;
    size_t inicio = 0;

    size_t pos = linea.find('|');
    while (pos != string::npos) {
        if (cantidad == maximo) {
            return false;
        }
        campos[cantidad] = recortar(linea.substr(inicio, pos - inicio));
        cantidad++;
        inicio = pos + 1;
        pos = linea.find('|', inicio);
    }

    if (cantidad == maximo) {
        return false;
    }
    campos[cantidad] = recortar(linea.substr(inicio));
    cantidad++;
    return true;
}

// Auxiliar de carga: enlaza un juego al final de la lista sin imprimir
void enlazarVideojuego(Nodo*& inicio, const Videojuego& juego) {
    Nodo* nuevo = new Nodo(juego);
    if (inicio == NULL) {
        inicio = nuevo;
        return;
    }
    Nodo* temp = inicio;
    while (temp->siguiente != NULL) {
        temp = temp->siguiente;
    }
    temp->siguiente = nuevo;
}

// Auxiliar de carga: enlaza una solicitud al final de la cola sin imprimir
void enlazarSolicitud(NodoCola*& frente, NodoCola*& final, const Solicitud& solicitud) {
    NodoCola* nuevo = new NodoCola(solicitud);
    if (frente == NULL) {
        frente = nuevo;
        final = nuevo;
        return;
    }
    final->siguiente = nuevo;
    final = nuevo;
}

// Auxiliar de carga: inserta una accion en la cima sin imprimir
void enlazarAccion(NodoPila*& cima, const string& accion) {
    NodoPila* nuevo = new NodoPila(accion);
    nuevo->siguiente = cima;
    cima = nuevo;
}

// Guarda el catalogo, un juego por linea
void guardarCatalogo(Nodo* inicio) {
    ofstream archivo(ARCHIVO_CATALOGO);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir " << ARCHIVO_CATALOGO << " para guardar\n";
        return;
    }

    Nodo* temp = inicio;
    while (temp != NULL) {
        archivo << temp->dato.codigo << "|"
                << temp->dato.nombre << "|"
                << temp->dato.genero << "|"
                << fixed << setprecision(2) << temp->dato.precio << "\n";
        temp = temp->siguiente;
    }
}

// Guarda las solicitudes, de frente a final
void guardarSolicitudes(NodoCola* frente) {
    ofstream archivo(ARCHIVO_SOLICITUDES);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir " << ARCHIVO_SOLICITUDES << " para guardar\n";
        return;
    }

    NodoCola* temp = frente;
    while (temp != NULL) {
        archivo << temp->dato.usuario << "|" << temp->dato.codigoJuego << "\n";
        temp = temp->siguiente;
    }
}

// Guarda el historial, de cima a base
void guardarHistorial(NodoPila* cima) {
    ofstream archivo(ARCHIVO_HISTORIAL);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir " << ARCHIVO_HISTORIAL << " para guardar\n";
        return;
    }

    NodoPila* temp = cima;
    while (temp != NULL) {
        archivo << temp->dato << "\n";
        temp = temp->siguiente;
    }
}

// Carga el catalogo al final de la lista. Ignora lineas invalidas
// y codigos repetidos.
void cargarCatalogo(Nodo*& inicio, int& cantidadCargada) {
    cantidadCargada = 0;
    ifstream archivo(ARCHIVO_CATALOGO);
    if (!archivo.is_open()) {
        return;  // Primera ejecucion: el archivo no existe
    }

    string linea;
    string campos[4];
    int cantidad = 0;

    while (getline(archivo, linea)) {
        linea = recortar(linea);
        if (linea.empty()) {
            continue;
        }

        int codigo;
        float precio;
        if (!separarCampos(linea, campos, 4, cantidad) || cantidad != 4) {
            continue;
        }
        if (!convertirEntero(campos[0], codigo) || codigo <= 0) {
            continue;
        }
        if (campos[1].empty() || campos[2].empty()) {
            continue;
        }
        if (!convertirDecimal(campos[3], precio) || precio < 0) {
            continue;
        }
        if (buscarPorCodigo(inicio, codigo) != NULL) {
            continue;  // Codigo repetido: se ignora
        }

        Videojuego juego;
        juego.codigo = codigo;
        juego.nombre = campos[1];
        juego.genero = campos[2];
        juego.precio = precio;
        enlazarVideojuego(inicio, juego);
        cantidadCargada++;
    }
}

// Carga las solicitudes en el mismo orden del archivo (frente -> final)
void cargarSolicitudes(NodoCola*& frente, NodoCola*& final, int& cantidadCargada) {
    cantidadCargada = 0;
    ifstream archivo(ARCHIVO_SOLICITUDES);
    if (!archivo.is_open()) {
        return;
    }

    string linea;
    string campos[2];
    int cantidad = 0;

    while (getline(archivo, linea)) {
        linea = recortar(linea);
        if (linea.empty()) {
            continue;
        }

        int codigoJuego;
        if (!separarCampos(linea, campos, 2, cantidad) || cantidad != 2) {
            continue;
        }
        if (campos[0].empty()) {
            continue;
        }
        if (!convertirEntero(campos[1], codigoJuego) || codigoJuego <= 0) {
            continue;
        }

        Solicitud solicitud;
        solicitud.usuario = campos[0];
        solicitud.codigoJuego = codigoJuego;
        enlazarSolicitud(frente, final, solicitud);
        cantidadCargada++;
    }
}

// Carga el historial. El archivo va de cima a base, por eso se
// apila primero en una pila auxiliar y despues se traslada a la
// pila real: la primera linea del archivo queda en la cima.
void cargarHistorial(NodoPila*& cima, int& cantidadCargada) {
    cantidadCargada = 0;
    ifstream archivo(ARCHIVO_HISTORIAL);
    if (!archivo.is_open()) {
        return;
    }

    NodoPila* auxiliar = NULL;
    string linea;

    while (getline(archivo, linea)) {
        linea = recortar(linea);
        if (linea.empty()) {
            continue;
        }
        enlazarAccion(auxiliar, linea);
        cantidadCargada++;
    }

    // En la auxiliar la accion mas antigua quedo en la cima.
    // Al pasarla a la pila real, la mas reciente queda arriba.
    while (auxiliar != NULL) {
        NodoPila* nodo = auxiliar;
        auxiliar = auxiliar->siguiente;
        nodo->siguiente = cima;
        cima = nodo;
    }
}

// Agrupadora: guarda las tres estructuras
void guardarDatos(Nodo* inicio, NodoCola* frente, NodoPila* cima) {
    guardarCatalogo(inicio);
    guardarSolicitudes(frente);
    guardarHistorial(cima);
}

// Agrupadora: carga las tres estructuras y muestra un solo resumen
void cargarDatos(Nodo*& inicio, NodoCola*& frente, NodoCola*& final, NodoPila*& cima) {
    int juegos = 0;
    int solicitudes = 0;
    int acciones = 0;

    cargarCatalogo(inicio, juegos);
    cargarSolicitudes(frente, final, solicitudes);
    cargarHistorial(cima, acciones);

    cout << "\nDatos cargados: " << juegos << " juegos, "
         << solicitudes << " solicitudes, " << acciones << " acciones\n";
}

// ==========================================================
// 8. OPCIONES DEL MENU
//    Cada opcion pide los datos, llama a las funciones de las
//    partes anteriores y registra la accion en el historial.
// ==========================================================

// ---------- Opciones del catalogo ----------

void opcionRegistrarVideojuego(Nodo*& inicio, NodoPila*& cima) {
    int codigo;
    float precio;

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

    if (insertarVideojuego(inicio, codigo, nombre, genero, precio)) {
        insertarEnCima(cima, "Registro del videojuego " + aTexto(codigo) + " (" + nombre + ")");
    }
}

void opcionBuscarVideojuego(Nodo* inicio) {
    cout << "Buscar por:\n";
    cout << "1. Codigo\n";
    cout << "2. Nombre\n";

    int opcion;
    if (!leerEntero("Seleccione una opcion: ", opcion)) {
        cout << "Opcion invalida\n";
        return;
    }

    if (opcion == 1) {
        int codigo;
        if (!leerEntero("Codigo a buscar: ", codigo)) {
            cout << "Codigo invalido\n";
            return;
        }
        buscarVideojuego(inicio, "codigo", aTexto(codigo));
    } else if (opcion == 2) {
        string nombre = leerLinea("Nombre a buscar: ");
        buscarVideojuego(inicio, "nombre", nombre);
    } else {
        cout << "Opcion invalida\n";
    }
}

void opcionActualizarVideojuego(Nodo* inicio, NodoPila*& cima) {
    int codigo;
    if (!leerEntero("Codigo del videojuego a actualizar: ", codigo)) {
        cout << "Codigo invalido\n";
        return;
    }
    if (actualizarVideojuego(inicio, codigo)) {
        insertarEnCima(cima, "Actualizacion del videojuego " + aTexto(codigo));
    }
}

void opcionEliminarVideojuego(Nodo*& inicio, NodoPila*& cima) {
    int codigo;
    if (!leerEntero("Codigo del videojuego a eliminar: ", codigo)) {
        cout << "Codigo invalido\n";
        return;
    }
    if (eliminarVideojuego(inicio, codigo)) {
        insertarEnCima(cima, "Eliminacion del videojuego " + aTexto(codigo));
    }
}

// ---------- Opciones de solicitudes de compra ----------

void opcionRegistrarSolicitud(Nodo* catalogo, NodoCola*& frente, NodoCola*& final, NodoPila*& cima) {
    string usuario = leerLinea("Nombre del usuario: ");
    int codigoJuego;

    if (!leerEntero("Codigo del juego a comprar: ", codigoJuego)) {
        cout << "Codigo invalido\n";
        return;
    }
    if (buscarPorCodigo(catalogo, codigoJuego) == NULL) {
        cout << "El videojuego no existe en el catalogo\n";
        return;
    }

    if (insertarPorElFinal(frente, final, usuario, codigoJuego)) {
        insertarEnCima(cima, "Solicitud registrada: " + usuario + " - juego " + aTexto(codigoJuego));
    }
}

void opcionProcesarSolicitud(NodoCola*& frente, NodoCola*& final, NodoPila*& cima) {
    if (frente == NULL) {
        cout << "La cola esta vacia\n";
        return;
    }

    // Se guardan los datos antes de que el nodo sea eliminado
    string usuario = frente->dato.usuario;
    int codigoJuego = frente->dato.codigoJuego;

    eliminarPorElFrente(frente, final);
    insertarEnCima(cima, "Solicitud procesada: " + usuario + " - juego " + aTexto(codigoJuego));
}

// ---------- Submenus ----------

void menuCatalogo(Nodo*& catalogo, NodoPila*& historial) {
    int opcion;
    do {
        cout << "\n----- CATALOGO DE VIDEOJUEGOS -----\n";
        cout << "1. Registrar videojuego\n";
        cout << "2. Mostrar catalogo\n";
        cout << "3. Buscar videojuego\n";
        cout << "4. Actualizar videojuego\n";
        cout << "5. Eliminar videojuego\n";
        cout << "0. Volver al menu principal\n";

        if (!leerEntero("Seleccione una opcion: ", opcion)) {
            opcion = -1;
        }

        switch (opcion) {
            case 1: opcionRegistrarVideojuego(catalogo, historial); break;
            case 2: mostrarVideojuegos(catalogo); break;
            case 3: opcionBuscarVideojuego(catalogo); break;
            case 4: opcionActualizarVideojuego(catalogo, historial); break;
            case 5: opcionEliminarVideojuego(catalogo, historial); break;
            case 0: break;
            default: cout << "Opcion invalida\n";
        }
    } while (opcion != 0);
}

void menuSolicitudes(Nodo* catalogo, NodoCola*& frente, NodoCola*& final, NodoPila*& historial) {
    int opcion;
    do {
        cout << "\n----- SOLICITUDES DE COMPRA -----\n";
        cout << "1. Registrar solicitud\n";
        cout << "2. Procesar siguiente solicitud\n";
        cout << "3. Consultar solicitudes pendientes\n";
        cout << "0. Volver al menu principal\n";

        if (!leerEntero("Seleccione una opcion: ", opcion)) {
            opcion = -1;
        }

        switch (opcion) {
            case 1: opcionRegistrarSolicitud(catalogo, frente, final, historial); break;
            case 2: opcionProcesarSolicitud(frente, final, historial); break;
            case 3: consultarSolicitudes(frente); break;
            case 0: break;
            default: cout << "Opcion invalida\n";
        }
    } while (opcion != 0);
}

void menuHistorial(NodoPila*& historial) {
    int opcion;
    do {
        cout << "\n----- HISTORIAL DE ACCIONES -----\n";
        cout << "1. Mostrar historial\n";
        cout << "2. Retirar la ultima accion\n";
        cout << "0. Volver al menu principal\n";

        if (!leerEntero("Seleccione una opcion: ", opcion)) {
            opcion = -1;
        }

        switch (opcion) {
            case 1: mostrarHistorial(historial); break;
            case 2: eliminarDeCima(historial); break;
            case 0: break;
            default: cout << "Opcion invalida\n";
        }
    } while (opcion != 0);
}

// ==========================================================
// 9. FUNCION PRINCIPAL
// ==========================================================

int main() {
    Nodo* catalogo = NULL;       // Lista enlazada
    NodoCola* frente = NULL;     // Cola: frente
    NodoCola* final = NULL;      // Cola: final
    NodoPila* historial = NULL;  // Pila: cima

    // Recuperar los datos guardados en la ejecucion anterior
    cargarDatos(catalogo, frente, final, historial);

    int opcion;
    do {
        cout << "\n==========================================\n";
        cout << "   TIENDA DIGITAL DE VIDEOJUEGOS\n";
        cout << "==========================================\n";
        cout << "1. Catalogo de videojuegos\n";
        cout << "2. Solicitudes de compra\n";
        cout << "3. Historial de acciones\n";
        cout << "0. Salir\n";

        if (!leerEntero("Seleccione una opcion: ", opcion)) {
            opcion = -1;
        }

        switch (opcion) {
            case 1: menuCatalogo(catalogo, historial); break;
            case 2: menuSolicitudes(catalogo, frente, final, historial); break;
            case 3: menuHistorial(historial); break;
            case 0: cout << "\nSaliendo del sistema...\n"; break;
            default: cout << "Opcion invalida\n";
        }

        // Guardar en disco al final de cada vuelta del menu
        guardarDatos(catalogo, frente, historial);
    } while (opcion != 0);

    // Liberar toda la memoria dinamica antes de terminar
    liberarCatalogo(catalogo);
    liberarCola(frente, final);
    liberarPila(historial);

    return 0;
}
