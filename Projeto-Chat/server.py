from socket import socket, AF_INET, SOCK_STREAM
from threading import Thread
from database import Database


# O servidor poderá conversar com vários clientes simultaneamente
# Cada cliente irá se identificar com um nome logo no início
# Cada cliente poderá mandar mensagem para outro cliente
# Ou seja, o cliente irá informar o nome do destinatário e a mensagem
# O servidor irá receber a mensagem e repassar para o cliente correto

class Servidor:
    def __init__(self, host='0.0.0.0', port=12345):
        self.db = Database()
        self.host = host
        self.port = port
        # dicionário para armazenar os clientes conectados
        # a chave será o nome do cliente e o valor será o socket do cliente
        self.clients = {}

    def handle_client(self, client_socket):
    # Autenticação / Registro
        while True:
            client_name = client_socket.recv(1024).decode('utf-8')

            if client_name in self.clients:
                client_socket.send("Nome já está em uso (conectado). Tente outro.".encode('utf-8'))
                continue

            # Permite reusar nome já cadastrado no banco
            self.db.cadastrar_usuario(client_name)  # Ignora erro se já existir
            self.clients[client_name] = client_socket

            print(f"[*] Cliente {client_name} conectado")
            client_socket.send("Nome aceito".encode('utf-8'))

            # Envia lista de contatos
            contatos = self.db.listar_usuarios()
            client_socket.send(f"Contatos: {', '.join(contatos)}".encode('utf-8'))

            # Envia mensagens offline
            mensagens = self.db.buscar_mensagens_offline(client_name)
            for remetente, conteudo in mensagens:
                client_socket.send(f"[Offline de {remetente}] {conteudo}".encode('utf-8'))
            self.db.apagar_mensagens_offline(client_name)

            break

        # Loop de mensagens
        while True:
            try:
                message = client_socket.recv(1024).decode('utf-8')
                if message == "/sair":
                    print(f"[*] Cliente {client_name} encerrou a conexão.")
                    del self.clients[client_name]
                    client_socket.close()
                    break

                if not message:
                    print(f"[*] Cliente {client_name} desconectado")
                    del self.clients[client_name]
                    client_socket.close()
                    break

                # Formato: "destinatario:mensagem"
                recipient_name, msg = message.split(':', 1)

                if recipient_name in self.clients:
                    self.clients[recipient_name].send(f"{client_name}: {msg}".encode('utf-8'))
                else:
                    self.db.salvar_mensagem_offline(client_name, recipient_name, msg)
                    client_socket.send(f"\n[Servidor] {recipient_name} está offline. Mensagem salva.\n".encode('utf-8'))

            except Exception as e:
                print(f"Erro: {e}")
                if client_name in self.clients:
                    del self.clients[client_name]
                client_socket.close()
                break


    def start(self):
        # cria o socket do servidor
        server = socket(AF_INET, SOCK_STREAM)
        server.bind((self.host, self.port))
        server.listen()
        print(f"[*] Ouvindo em {self.host}:{self.port}")

        # enquanto o servidor estiver rodando, ele irá aceitar conexões
        # de clientes e criar uma thread secundária para cada cliente
        while True:
            # aceita a conexão do cliente
            client_socket, addr = server.accept()
            
            # cria uma thread secundária para lidar com o cliente atual
            print(f"[*] Conexão aceita com {addr}")
            client_handler = Thread(target=self.handle_client, args=(client_socket,))
            client_handler.start()
        

if __name__ == "__main__":
    # Inicia o servidor
    servidor = Servidor()
    servidor.start()