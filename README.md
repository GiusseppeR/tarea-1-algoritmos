INSTRUCCIONES PARA EJECUTAR EL CODIGO

El codigo está hecho en c y se debe tener algun compilador de c instalado en el dispositivo en el que se pruebe, además de tener github para poder clonar el repositorio localmente y ejecutarlo. Se recomienda ejecutar el codigo en Clion el cual se puede descargar en https://www.jetbrains.com/clion/, para instrucciones de como clonar el repositorio ver [https://docs.github.com/en/desktop/adding-and-cloning-repositories/cloning-and-forking-repositories-from-github-desktop](https://docs.github.com/es/repositories/creating-and-managing-repositories/cloning-a-repository) una vez clonado abrir el código en el IDE de preferencia, dentro del código se encontará un archivo main el cual se debe ejecutar. Al ejecutar el main el código empezara a correr, primero se mostraran en la terminal los resultados de las 100 queries para el algoritmo Ciaccia-Patella para valores de n ={1024,2048,4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432} posteriormente se mostraran los resultados en el mismo estilo pero para el algoritmo Sexton-Swinbank para este algoritmo se ejecutara para valores de n = {1024,2048,4096, 8192, 16384, 32768}.

Cada query es del estilo:
Query n finished:
accesses = 346
points = 91
point percentage = 0.112915, además despues de la última query en cada valor de n aparece el tiempo de ejecución.
