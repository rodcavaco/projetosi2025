import tkinter as tk
from tkinter import simpledialog, scrolledtext, messagebox
from threading import Thread
from socket import socket, AF_INET, SOCK_STREAM


class ChatClient:
    def __init__(self, master):
        self.master = master
        self.master.title("Cliente de Chat")
        self.socket = socket(AF_INET, SOCK_STREAM)
        self.socket.connect(('localhost', 12345))


        # Login
        self.nome = simpledialog.askstring("Login", "Digite seu nome", parent=master)
        if not self.nome:
            master.destroy()
            return

        self.socket.send(self.nome.encode('utf-8'))
        resposta = self.socket.recv(1024).decode('utf-8')

        if resposta != "Nome aceito":
            messagebox.showerror("Erro", resposta)
            master.destroy()
            return

        # Recebe lista de contatos
        contatos = self.socket.recv(1024).decode('utf-8')
        lista_nomes = contatos.replace("Contatos: ", "").split(", ")
        lista_nomes = [n for n in lista_nomes if n != self.nome]  # remove o próprio nome

        self.destinatario = None

        # Layout da interface
        self.frame_contatos = tk.Frame(master)
        self.frame_contatos.pack(side=tk.LEFT, fill=tk.Y)

        self.lista_contatos = tk.Listbox(self.frame_contatos)
        for nome in lista_nomes:
            self.lista_contatos.insert(tk.END, nome)
        self.lista_contatos.pack(padx=5, pady=5)
        self.lista_contatos.bind("<<ListboxSelect>>", self.selecionar_destinatario)

        self.frame_chat = tk.Frame(master)
        self.frame_chat.pack(side=tk.RIGHT, expand=True, fill=tk.BOTH)

        self.label_dest = tk.Label(self.frame_chat, text="Nenhum destinatário selecionado")
        self.label_dest.pack()

        self.text_area = scrolledtext.ScrolledText(self.frame_chat, state='disabled')
        self.text_area.pack(padx=10, pady=10, expand=True, fill=tk.BOTH)
        self.text_area.tag_config("enviado", foreground="blue")
        self.text_area.tag_config("recebido", foreground="green")


        self.entry = tk.Entry(self.frame_chat)
        self.entry.pack(fill=tk.X, padx=10)

        self.send_btn = tk.Button(self.frame_chat, text="Enviar", command=self.enviar_mensagem)
        self.send_btn.pack(pady=5)

        # Inicia recebimento
        Thread(target=self.receber_mensagens, daemon=True).start()

    def selecionar_destinatario(self, event):
        selecionado = self.lista_contatos.curselection()
        if selecionado:
            self.destinatario = self.lista_contatos.get(selecionado)
            self.label_dest.config(text=f"Conversando com: {self.destinatario}")

    def enviar_mensagem(self):
        msg = self.entry.get()
        if msg and self.destinatario:
            full_msg = f"{self.nome}: {msg}"
            self.socket.send(f"{self.destinatario}:{msg}".encode('utf-8'))
            self.entry.delete(0, tk.END)

            # Exibe a própria mensagem no chat
            self.text_area.configure(state='normal')
            self.text_area.insert(tk.END, f"Você: {msg}\n", "enviado")
            self.text_area.configure(state='disabled')
            self.text_area.see(tk.END)


        elif not self.destinatario:
            messagebox.showwarning("Aviso", "Selecione um destinatário antes de enviar.")


    def receber_mensagens(self):
        while True:
            try:
                msg = self.socket.recv(1024).decode('utf-8')
                if msg:
                    self.text_area.configure(state='normal')
                    self.text_area.insert(tk.END, msg + "\n", "recebido")
                    self.text_area.configure(state='disabled')
                    self.text_area.see(tk.END)
            except:
                break


if __name__ == "__main__":
    root = tk.Tk()
    client = ChatClient(root)
    root.mainloop()
