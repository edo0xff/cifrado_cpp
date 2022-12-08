# Cifrado simple de archivos con C++

## Acerca del algoritmo

El algoritmo de cifrado empleado es una sencilla operacion matematica:

    # Cifrado
    Cn = (On + Km) % 256

    # Decifrado
    On = (Cn - Km) % 256

El cifrado produce un Byte cifrado *Cn* a partir de la suma de el Byte original *On* con el Byte clave *Km*. La operacion modular (256) es para asegurar que el resultado no sobrepase el tamaño maximo de un Byte.

El decifrado es el proceso inverso (resta).

## Acerca del programa

El programa implementa el algoritmo antes descrito de forma optimizada para poder trabajar con cualquier tipo de archivo y de cualquier tamaño. Puede cifrar incluso archivos de varios Gigabytes sin problemas de rendimiento.

## Requisitos

- GIT
- Compilador de C++ (MinGW o MSVC en windows)

## Instalacion

Simplemente clonamos el repositorio:

    git clone https://github.com/edo0xff/cifrado_cpp.git

## Compilacion

### Windows

    g++ -c main.cpp -o main.o
    g++ main.o -o main.exe

## Ejemplos de uso

### Cifrado de un archivo

    main.exe -c file.png file.cpng

Lo anterior cifra el archivo `file.png` y lo guarda dentro de `file.cpng`, el nombre del archivo de salida es hasta cierto punto es irrelevante pero nos puede ayudar a recordar que tipo de archivo era antes de cifrarlo.

### Decifrado de un archivo

    main.exe -d file.cpng file.png

Lee el archivo cifrado `file.cpng`, lo decifra y lo guarda en el archivo `file.png` aqui el archivo de salida si es importante, al menos la extension, ya que esta debe coincidir con el archivo original (previo al cifrado) para que el sistema operativo lo pueda interpretar correctamente.

> Nota: La clave en el proceso de decifrado debe coincidir con la clave que se uso en el cifrado, si no es asi, el archivo decifrado producido no sera el original.

## Licencia 

MIT