Para la implementación se utilizó CLion de JetBrains. Se intentó compilar de otras maneras, pero al final lo mejor es utilizar el propio ambiente para hacerlo. 

CLion es de pago, pero al inscribirse con el correo de la universidad se puede utilizar gratis. La tarea se puede compilar y ejecutar desde el mismo programa con el botón "Run" en la esquina superior derecha. Alternativamente, se puede utilizar sólo para crear el ejecutable con el botón "Build" al lado de "Run. El ejecutable se encuentra en la carpeta "cmake-build-debug".

La tarea se probó en Windows, Mac y Linux. Hemos visto comportamientos muy variados dependiendo del hardware y del sistema operativo. Pero para resumir:

-En Windows CP funciona bien y SS funciona hasta 2^14 puntos
-En Mac funcionan ambos bien
-En Linux (máquina virtual) CP funciona hasta 2^24. SS no se pudo probar bien porque congela la máquina virtual 

A continuación se muestra cómo configurar CLion:

##### Incluir configuración en mac #####

--- Configuración Windows ---

-Descargar CLion e instalar para Windows desde la página oficial

-Abrir la tarea

-En File >> Settings >> Build, Execution, Deployment >> Toolchains, asegurarse de tener la configuración por defecto. Es decir, que el toolchain sea MinGW, CMake sea "bundled", la build tool sea "ninja.exe" del ambiente local de CLion, y el compilador de C sea "gcc.exe" del ambiente local de Clion.

OJO!!!!: la tarea NO funciona con las herramientas de compilado y construcción de VisualStudio.

--- Configuración Linux ---
   --> Esto se probó con Kali Linux

-Descargar CLion para Linux desde la página oficial

-Extraer el archivo y abrir el directorio resultante en la consola. Ejecutar $sh bin/clion.sh para abrirlo. Es posible que la primera vez se tenga que correr el comando dos veces.

-La configuración por defecto debería ser suficiente.
