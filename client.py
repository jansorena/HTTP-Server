import socket
import ssl
import webbrowser

host = socket.gethostname()
port = 10000

context = ssl.create_default_context()
context.check_hostname = False
context.verify_mode = ssl.CERT_NONE


with socket.create_connection((host, port)) as sock:
    with context.wrap_socket(sock, server_hostname=host) as s:
        request = 'GET /recetas HTTP/1.1'.format(host)
        s.sendall(request.encode())
        response = s.recv(32000)

response = response.decode()

header, content = response.split('\r\n\r\n', 1)

print(header)

temp_file = 'temp.html'
with open(temp_file, 'w') as f:
    f.write(content)

webbrowser.open(temp_file)