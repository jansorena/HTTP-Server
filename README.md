# Laboratorio Evaluado 2

## Descripción

Este proyecto es una implementación en lenguaje C++ de un servidor HTTP básico utilizando OpenSSL. El servidor escucha en el puerto 10000 y espera peticiones GET de algún cliente o navegador.

El proyecto se divide en:

- `server.cpp`: Implementa el servidor que escucha en el puerto 10000 y espera peticiones de algún cliente o navegador.
- `client.py`: Implementa un cliente que realiza peticiones personalizadas.
- `tcp.cpp`: Implementa las funciones de creación del socket, bindear al puerto respectivo, escucha del servidor, conexión entre cliente y servidor, envío y recepción de datos a través de sockets TCP cifrados y configuración de los certificados y llaves privadas para OpenSSL.
- `http.cpp`: Implementa la generación de respuestas HTTP, envio de las respuestas al cliente, parsing y verificación de las peticiones recibidas.

## Instalación y Compilación

Para compilar este proyecto, se requiere tener instalado `CMake`, `GCC` y la biblioteca `openssl`. Los pasos para compilar son los siguientes:

1. Clonar el repositorio:
   
   ```bash
   git clone https://github.com/jansorena/Redes-Lab2.git
   ```

2. Crear un directorio `build`:
   
   ```bash
   mkdir build
   cd build
   ```

3. Ejecutar CMake para generar los archivos Makefile:
   
   ```bash
   cmake ..
   ```

4. Compilar el proyecto con el comando `make`:
   
   ```bash
   make
   ```

## Uso

### OpenSSL

Para configurar un entorno de prueba, se requiere generar una clave y un certificado para el uso de OpenSSL.

1. Dentro de la carpeta del proyecto
   
   ```bash
   mkdir ssl
   cd ssl
   ```

2. Generar la clave privada:
   
   ```bash
   openssl genpkey -algorithm RSA -out private.key
   ```

3. Crea una solicitud de certificado (CSR) usando la clave privada generada anteriormente:
   
   ```bash
   openssl req -new -key private.key -out certificate.csr
   ```

4. Genera un certificado autofirmado válido por 365 días a partir de la solicitud de certificado:
   
   ```bash
   openssl x509 -req -days 365 -in certificate.csr -signkey private.key -out certificate.crt
   ```

Es importante tener en cuenta que el certificado generado es autofirmado, lo que significa que los navegadores mostrarán una advertencia de seguridad al acceder al servidor.

### Servidor

Para iniciar el servidor, ejecutar el archivo `server` compilado en el directorio `build`:

```bash
./server
```

El servidor escuchará en el puerto 10000 y esperará conexiones de clientes. Una vez recibido las peticiones, imprimirá por la terminal las peticiones recibidas.

### Navegador

De algún navegador realizar las peticiones accediendo a `https://localhost:10000/ruta/a/abrir`. 

Cabe destacar que las peticiones hechas a `http://localhost:10000` son rechazadas por el servidor por no utilizar SSL.

Las rutas posibles a acceder se encuentran o se pueden agregar en la carpeta HTTP.

### Cliente

Si se utiliza el script `cliente.py` se pueden realizar peticiones personalizadas modificando:

```py
...
with socket.create_connection((host, port)) as sock:
    with context.wrap_socket(sock, server_hostname=host) as s:
        request = 'PETICION PERSONALIZADA'.format(host)
        s.sendall(request.encode())
        response = s.recv(32000)
...
```

El header de respuesta es mostrado en la terminal y el contenido html es temporalmente copiado en `temp.html` y abierto en el navegador predeterminado.

> Solo muestra el contenido html.

## Demostración



---

Una versión sin SSL se encuentra disponible en `https://github.com/jansorena/Redes-Lab2/tree/no-ssl`

Para cambiar de rama:

```bash
git checkout no-ssl

git pull origin no-ssl
```

La instalación y compilación es la misma que para el proyecto principal.