import mysql.connector

class Database:
    def __init__(self, host="localhost", user="root", password="", database="chatdb"):
        self.conn = mysql.connector.connect(
            host=host,
            user=user,
            password=password,
            database=database
        )
        self.cursor = self.conn.cursor()

    def cadastrar_usuario(self, nome):
        try:
            self.cursor.execute("INSERT INTO usuarios (nome) VALUES (%s)", (nome,))
            self.conn.commit()
            return True
        except mysql.connector.IntegrityError:
            return False

    def listar_usuarios(self):
        self.cursor.execute("SELECT nome FROM usuarios")
        return [row[0] for row in self.cursor.fetchall()]
    
    def salvar_mensagem_offline(self, remetente, destinatario, conteudo):
        self.cursor.execute(
            "INSERT INTO mensagens (remetente, destinatario, conteudo) VALUES (%s, %s, %s)",
            (remetente, destinatario, conteudo)
        )
        self.conn.commit()

    def buscar_mensagens_offline(self, destinatario):
        self.cursor.execute(
            "SELECT remetente, conteudo FROM mensagens WHERE destinatario = %s ORDER BY timestamp",
            (destinatario,)
        )
        return self.cursor.fetchall()

    def apagar_mensagens_offline(self, destinatario):
        self.cursor.execute(
            "DELETE FROM mensagens WHERE destinatario = %s",
            (destinatario,)
        )
        self.conn.commit()


# Teste manual
if __name__ == "__main__":
    db = Database(password="1234")  # ajuste a senha se necessário
    sucesso = db.cadastrar_usuario("testeuser")
    print("Cadastro bem-sucedido?", sucesso)
    print("Usuários:", db.listar_usuarios())


