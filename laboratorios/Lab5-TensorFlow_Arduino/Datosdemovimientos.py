#!/usr/bin/python3
import serial

def leer_datos_puerto_serie(nombre_archivo):
    try:
        # Configuración para el puerto serie.
        serialRX = serial.Serial( #Vista en clase
            port='COM1',
            baudrate=9600,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS,
            timeout=0
        )
        print("Se establecio conexion con el puerto serial")

        # Abre el archivo en modo escritura.
        file = open(nombre_archivo,  mode='w', newline='')
        print("Se Inicio captura de datos en ", nombre_archivo)
        
        file.write("# Inicio de la captura de datos\n") #Escribe un titulo para el csv
        # Ciclo infinito para leer datos del puerto serie.
        contador=0
        while (contador < 32000): #el codigo Ino forma el formato cvs, asi que solo se estima un numero de pruebas para 5-15s, el contador representa el numero de caracter. 
            # Lee un byte del puerto serie.
            for data in serialRX.read():

                
                # Convierte el byte a carácter.
                character = chr(data)

                # Imprime el carácter en la consola.
                print(character, end="")

                # Escribe el carácter en el archivo.
                file.write(character)
                contador= contador + 1
            

    except KeyboardInterrupt:
        # Interrupción por teclado (Ctrl + C).
        print("\nPrograma terminado. Datos guardados en", nombre_archivo)

    except Exception as e:
        # Manejo de errores generales.
        print(f"Error: {e}")
        print("Programa terminado. Datos guardados en", nombre_archivo)


    finally:
        # Cierra las conexiones (puerto serie y archivo) al finalizar.
 
        serialRX.close()
        print("Puerto serie cerrado.")
        file.close()
        print("Programa finalizado.")

# Ejemplo de uso de la función
leer_datos_puerto_serie('Flexion.csv')#se modifica la etiqueta del nombre del movimiento
