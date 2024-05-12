INSTRUCCIONES PARA EJECUTAR EL CODIGO

IMPORTANTE! ANTES DE EJECUTAR EL CODIGO VERIFICAR QUE EL ARCHIVO CMakeLists.txt CONTENGA LO SIGUIENTE:
cmake_minimum_required(VERSION 3.25)
project(TareaLog C)

set(CMAKE_C_STANDARD 23)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O1")

add_executable(TareaLog main.c
        arrays/array.h
        arrays/array.c
        arrays/matrix.h
        structures.h
        ciaccia_patella/cp_algorithm.c
        trees/2dTrees.h
        trees/2dTrees.c
        random/random.c
        random/custom_random.h
        utils.h
        utils.c
        arrays/matrix.c
        arrays/matrix.c
        sexton_swinbank/ss_algorithm.c
        sexton_swinbank/ss_utils.h
        query/query.c
        query/query.h
        query/query.c)

El codigo está hecho en c y se debe tener algun compilador de c instalado en el dispositivo en el que se pruebe, además de tener github para poder clonar el repositorio localmente y ejecutarlo. Se recomienda ejecutar el codigo en Clion el cual se puede descargar en https://www.jetbrains.com/clion/, para instrucciones de como clonar el repositorio ver [https://docs.github.com/en/desktop/adding-and-cloning-repositories/cloning-and-forking-repositories-from-github-desktop](https://docs.github.com/es/repositories/creating-and-managing-repositories/cloning-a-repository) una vez clonado abrir el código en el IDE de preferencia, dentro del código se encontará un archivo main el cual se debe ejecutar. Al ejecutar el main el código empezara a correr, primero se crearan los input de puntos necesarios, luego aparecerá un mensaje "Escribe 0 para experimentar con arboles CP y 1 para hacerlo con arboles SS" tal como se indica al escribir 0 en la terminal y presionar enter se empezarán a ejecutar los experimentos para Ciaccia-Patella, mientras que al escribir 1 y presionar enter se empezarán a ejecutar los experimentos para Sexton-Swinkbank. Al terminar de correr cualquiera de los experimentos aparecerá el mensaje "Escribe 0 para limpiar resultados y volver a seleccionar. Escribe cualquier otra cosa para salir." al escribir 0 y presionar enter se volverá al mensaje inicial y al escribir cualquier otra tecla y presionar enter se saldrá de la ejecucción 

Cada experimentacion mostrará mensajes como el siguiente para cada valor de n probado:

Utilizando 1024 puntos
Arbol de tamanno 2 construido con Ciaccia-Patella.
Tiempo de construccion: 0.001412 segundos.

Resultados de 100 queries:
Cantidad de accessos promedio: 6
Cantidad de puntos promedio: 1
Porcentaje de cobertura promedio: 0.125000

