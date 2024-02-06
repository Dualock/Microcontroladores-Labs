import json
import serial
import time
import paho.mqtt.client as mqtt

# Función para conectarse al broker MQTT
def connect_True(client, userdata, flags, rc):
    if rc == 0:
        print("Conexión establecida con el broker MQTT")
    else:
        print(f"Error en la conexión. Código de retorno: {rc}")

# Función para manejar los mensajes recibidos
def on_message(client, userdata, message):
    print(f"Mensaje recibido: {message.payload.decode()}")
# Configuración del puerto serial
serial_connection =  serial.Serial(
    port="COM4", 
    baudrate=115200, 
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1)
# Configuración del cliente MQTT
broker = "iot.eie.ucr.ac.cr"
port = 1883  
token = "5pavshof7n9xx2kx8zee"  # Token de autenticación
username = "Sismo_STM32"
topic = "v1/devices/me/telemetry"
 
data = dict()
# Crear instancia del cliente MQTT
client = mqtt.Client()
client.username_pw_set(device)

client.on_message = on_message

# Conectar al broker MQTT
client.connect(broker, port, 60)

# Iniciar el bucle de mensajes MQTT
client.loop_start()

# Conexión al puerto serial
try:
    Valid_Con = connect_True
    print("Conexión serial establecida")

    while True:
        if serial_connection.in_waiting > 0:
            line = serial_connection.readline().decode('utf-8').strip()
            components = line.split(',')  # Divide la línea en sus componentes individuales

            # Inicializa el diccionario de datos
            if len(data_captured) >= 3:
                data['x'] = data_captured[0]
                data['y'] = data_captured[1]
                data['z'] = data_captured[2]
                 

            data['Battery']= data_captured[3]
            # Publica los datos en el topic deseado
            output = json.dumps(data)
            client.publish("v1/devices/me/telemetry", json.dumps(data), qos=1)
            time.sleep(1)
except serial.SerialException as e:
    print(f"Error en la conexión serial: {str(e)}")
except KeyboardInterrupt:
    # Interrupción por teclado (Ctrl + C).
    print("\nPrograma terminado. Datos guardados de", username)
finally:
    # Detener el bucle de mensajes MQTT y cerrar la conexión serial
    client.loop_stop()
    serial_connection.close()
