#include <iostream>
#include <string>
#include <random>

using namespace std;

// Gera uma chave aleatória do mesmo tamanho da mensagem
string gerarChaveAleatoria(size_t tamanho) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, 255); // valores de byte

    string chave;
    for (size_t i = 0; i < tamanho; ++i) {
        chave += static_cast<char>(dist(gen));
    }

    return chave;
}

// Função para cifrar/decifrar usando XOR
string aplicarXOR(const string& texto, const string& chave) {
    string resultado;
    for (size_t i = 0; i < texto.size(); ++i) {
        resultado += texto[i] ^ chave[i];
    }
    return resultado;
}

// Mostra os bytes em formato numérico
void mostrarBytes(const string& texto) {
    for (unsigned char c : texto) {
        cout << static_cast<int>(c) << " ";
    }
    cout << endl;
}

int main() {
    string mensagem;

    cout << "Digite a mensagem: ";
    getline(cin, mensagem);

    // Gera a chave aleatória do mesmo tamanho da mensagem
    string chave = gerarChaveAleatoria(mensagem.size());

    // Cifra usando XOR
    string cifrada = aplicarXOR(mensagem, chave);

    cout << "\nChave (bytes): ";
    mostrarBytes(chave);

    cout << "Mensagem cifrada (bytes): ";
    mostrarBytes(cifrada);

    // Decifra (XOR de novo com a mesma chave)
    string decifrada = aplicarXOR(cifrada, chave);

    cout << "Mensagem decifrada: " << decifrada << endl;

    return 0;
}
