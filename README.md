# La Cocina de Linguini

### 👨‍🍳 La Cocina de Linguini

Ayuda a Linguini a salvar el restaurante! Debés gestionar los pedidos con rapidez y precisión antes de que se agote el tiempo de la jornada laboral.

### 🎯 Objetivo

Recaudar un total de $150,000 antes de alcanzar el límite de 200 movimientos.

**Mecánicas Principales:**

- Gestión de Pedidos: Toma órdenes de las mesas y entrégalas antes de que los clientes pierdan su paciencia.

- Amenaza de Plagas: Las cucarachas (U) cerca de las mesas restan 3 puntos de paciencia por turno.

- Obstáculos: Si pisas un charco (H) sin una mopa, perderás todos los pedidos que lleves en la bandeja.

- Power-ups: * Monedas (M): Suman $1,000 directamente a tu recaudación.

  - Patines (P): Te permiten deslizarte hasta el extremo del restaurante de un solo movimiento.

### 🎮 Controles

- **W, A, S y D** para movernos hacia arriba, izquierda, abajo y derecha
- **O** para equipar la mopa
- **T** para tomar un pedido de alguna mesa
- **P** para equipar los patines

### 🛠 Compilación y ejecución

Para comenzar a jugar, deberemos seguir las siguientes instrucciones una vez descarguemos el **juego**.

Con el siguiente comando, compilaremos los **tres** archivos de C que estan en el repositorio (`restaurante.c`, `restaurante.h` y `juego.c`):

    gcc juego.c restaurant.c -o juego

Luego, ejecutaremos el ejecutable con la siguiente línea:

    ./juego

### 🛠️ Habilidades Técnicas Aplicadas

En el desarrollo de este proyecto se aplicaron conceptos fundamentales de ciencias de la computación y buenas prácticas de programación en C:

1. Gestión de Memoria Dinámica

    **Asignación Eficiente**: Implementación de redimensionamiento dinámico de memoria mediante realloc para gestionar vectores de pedidos en preparación y listos, optimizando el uso de recursos según la demanda del juego.

    **Ciclo de Vida**: Gestión del ciclo de vida de los datos, asegurando la liberación de memoria mediante una función de destrucción controlada para prevenir memory leaks.

2. Arquitectura de Datos y Modularidad

    **Estructuras Compuestas**: Diseño de tipos de datos abstractos (TDA) mediante structs anidados para representar entidades complejas como la cocina, el mozo y el estado global del juego.

    **Encapsulamiento**: Separación clara entre la lógica de negocio (restaurant.c) y la interfaz de usuario/bucle principal (juego.c), facilitando el mantenimiento y la escalabilidad del código.

3. Lógica Algorítmica y Geometría

    **Procesamiento de Matrices**: Manipulación y validación de una matriz de 20x20 para representar el terreno, gestionando colisiones y superposición de elementos en tiempo de ejecución.

    **Cálculos Espaciales**: Aplicación de la Distancia de Manhattan para determinar la proximidad entre Linguini, las mesas y los obstáculos, optimizando la toma de decisiones del motor de juego.

4. Desarrollo de Interfaces de Consola (CLI)

    **UX por Consola**: Creación de una interfaz visual dinámica utilizando códigos de escape ANSI para el manejo de colores y limpieza de pantalla, mejorando la experiencia del usuario en un entorno de texto.

    **Validación de Inputs**: Implementación de un sistema robusto de captura y filtrado de datos para asegurar la integridad del flujo de juego.

### 📥 Descarga

Hace **[click acá](https://github.com/jakimm7/cocina_linguini1/releases/download/rar/linguini_v1.0.zip)** para descargar el juego y comenzar a ayudar a linguini en la cocina!

### 🎓 Créditos

- Autor y desarrollador: **Santiago Jakim**

- Materia: **Fundamentos de la Programación**, 2C 2024

- **Universidad de Buenos Aires, Facultad de Ingeniería**
