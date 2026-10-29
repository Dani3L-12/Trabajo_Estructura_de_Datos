# Sistema de Gestión de Tienda Digital de Videojuegos

Proyecto del curso **Estructura de Datos** (Entregable 2), desarrollado en **C++**.
Simula el funcionamiento básico de una tienda digital de videojuegos, tomando como referencia plataformas como Steam, mediante **estructuras de datos dinámicas lineales**: listas, pilas y colas, implementadas con nodos y punteros.

## Información académica

| | |
|---|---|
| **Universidad** | Universidad Continental |
| **Facultad** | Ingeniería |
| **Curso** | Estructura de Datos (NRC 39597) |
| **Docente** | Yesenia Concha Ramos |
| **Semestre** | 2026-20 |
| **Sede** | Cusco, Perú |

## Integrantes

| N° | Apellidos y nombres | E. Profesional |
|---|---|---|
| 1 | Illapuma Yupanqui, Jerson Reyder | Ing. de Sistemas e Informática |
| 2 | Condori Ciprian, Daniel Anderson | Ing. de Sistemas e Informática |
| 3 | Pilares Aguilar, Manuel Alonso | Ing. de Sistemas e Informática |
| 4 | Yucra Alvarado, Jeremy Salvador | Ing. de Sistemas e Informática |

## Descripción

La aplicación permite administrar un catálogo de videojuegos, registrar solicitudes de compra de los usuarios, procesarlas en el orden en que llegaron y mantener un historial de las acciones administrativas realizadas.

## Estructuras de datos utilizadas

| Estructura | Uso en el sistema | Principio |
|---|---|---|
| **Lista enlazada simple** | Catálogo de videojuegos (código, nombre, género y precio) | Inserción, búsqueda y eliminación en cualquier posición |
| **Cola** | Solicitudes de compra pendientes (usuario y código del juego) | FIFO: la primera solicitud en llegar es la primera en procesarse |
| **Pila** | Historial de acciones administrativas (registro, actualización y eliminación) | LIFO: la última acción registrada es la primera en retirarse |

Las tres estructuras usan memoria dinámica (`new` y `delete`), por lo que crecen y disminuyen según las operaciones del usuario, sin un tamaño máximo definido de antemano.

## Funcionalidades

Menú principal del programa:

| Opción | Función | Estructura |
|---|---|---|
| 1 | Registrar videojuego | Lista |
| 2 | Mostrar catálogo | Lista |
| 3 | Buscar videojuego (por código o por nombre) | Lista |
| 4 | Actualizar videojuego | Lista |
| 5 | Eliminar videojuego | Lista |
| 6 | Registrar solicitud de compra | Cola |
| 7 | Procesar solicitud de compra | Cola |
| 8 | Consultar solicitudes pendientes | Cola |
| 9 | Ver historial de acciones | Pila |
| 10 | Retirar la última acción del historial | Pila |
| 0 | Salir | — |

### Validaciones incluidas

- El código del videojuego debe ser mayor que cero y no puede repetirse.
- El precio no puede ser negativo.
- El nombre y el género no pueden estar vacíos.
- Una solicitud debe tener usuario y un código de videojuego que exista en el catálogo.
- Antes de eliminar o consultar, se comprueba que la lista, la pila o la cola no estén vacías.
- Las entradas numéricas incorrectas (por ejemplo, letras) no detienen el programa.

## Cómo compilar y ejecutar

### Con Dev-C++

1. Abrir `tienda_videojuegos.cpp`.
2. Compilar y ejecutar con **F11**.

### Con g++ (línea de comandos)

```
g++ tienda_videojuegos.cpp -o tienda_videojuegos
./tienda_videojuegos
```

El código es compatible con C++98 y C++11.

## Organización del código

El archivo `tienda_videojuegos.cpp` está dividido en bloques:

1. **Estructuras de datos y nodos:** registros `Videojuego` y `Solicitud`, y los nodos de la lista, la pila y la cola.
2. **Funciones auxiliares de entrada:** lectura segura de texto, enteros y decimales.
3. **Pila del historial:** insertar en la cima, eliminar de la cima y mostrar.
4. **Lista del catálogo:** insertar, mostrar, buscar, actualizar y eliminar.
5. **Cola de solicitudes:** insertar por el final, eliminar por el frente y mostrar.
6. **Liberación de memoria** de las tres estructuras.
7. **Opciones del menú y `main`.**
