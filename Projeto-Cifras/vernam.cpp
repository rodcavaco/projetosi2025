#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// Função que repete a chave até o tamanho da mensagem
string expandirChave(const string& chave, size_t tamanhoMensagem) {
    string chaveExpandida;
    size_t chaveLen = chave.length();

    for (size_t i = 0; i < tamanhoMensagem; ++i) {
        chaveExpandida += chave[i % chaveLen];
    }

    return chaveExpandida;
}

// Função para cifrar/decifrar usando XOR
string vernamXOR(const string& mensagem, const string& chave) {
    string resultado;

    for (size_t i = 0; i < mensagem.size(); ++i) {
        resultado += mensagem[i] ^ chave[i]; // XOR entre os caracteres
    }

    return resultado;
}

// Função para mostrar os bytes da string como inteiros
void mostrarBytes(const string& texto) {
    for (unsigned char c : texto) {
        cout << static_cast<int>(c) << " ";
    }
    cout << endl;
}

int main() {
    system("chcp 65001 > nul");
    string mensagem, chave;

    cout << "Digite a mensagem: ";
    getline(cin, mensagem);

    cout << "Digite a chave (ela será repetida se for menor): ";
    getline(cin, chave);

    // Expande a chave para o mesmo tamanho da mensagem
    string chaveExpandida = expandirChave(chave, mensagem.size());

    // Cifra a mensagem
    string cifrada = vernamXOR(mensagem, chaveExpandida);

    cout << "\nMensagem cifrada (como caracteres): " << cifrada << endl;

    cout << "Mensagem cifrada (em bytes): ";
    mostrarBytes(cifrada);

    // Decifra a mensagem
    string decifrada = vernamXOR(cifrada, chaveExpandida);
    cout << "Mensagem decifrada: " << decifrada << endl;

    return 0;
}
