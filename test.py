import requests
import webbrowser
import os

url = 'http://localhost:10000/pagina'
headers = {'Content-Type': 'application/json'}


#from http.client import HTTPConnection
#HTTPConnection._http_vsn_str = 'HTTP/1.0'

# Enviar una solicitud con el método GET
response = requests.request('GET', url, headers=headers)
print(response.status_code)
print(response.text)

# Guardar la respuesta en un archivo temporal
temp_file = 'temp.html'
with open(temp_file, 'w') as f:
    f.write(response.text)

# Abrir el archivo en el navegador predeterminado
webbrowser.open(temp_file)