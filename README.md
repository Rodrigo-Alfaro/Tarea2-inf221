# Tarea2-inf221

## Ejecucion
Para ejecutar el archivo principal (main.cpp) el cual contiene los algoritmos, es necesario en primer lugar compilar: 

    g++ main.cpp -lpsapi -o resultados

Luego para ejecutar:

    resultados.exe

Para ejecutar el archivo que nos entrega los pasos (operaciones), primero compilamos:

    g++ pasos.cpp -o pasos

y luego ejecutamos:

    pasos.exe

Si queremos generar matrices de costos de 26x26:

    py matrix.py

Y se generara un archivo .txt llamado randommatrix con la matriz

Si queremos generar dos string de 1048 caracteres:

    py string.py

y se generaran los strings en inputdp.txt

Todo lo relacionado a los costos para el archivo principal (main.cpp) se encuentran en los archivos cost_operacion.txt, y el input en input.txt en donde la primeras dos lineas representan a las cadenas S1 y S2.