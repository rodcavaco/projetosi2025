#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

string removerDuplicatas(string chave) {
    string resultado = "";
    for (char c : chave) {
        c = toupper(c);
        if (c == 'J') c = 'I';
        if (resultado.find(c) == string::npos)
            resultado += c;
    }
    return resultado;
}

void gerarMatriz(string chave, char matriz[5][5]) {
    string alfabeto = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
    chave = removerDuplicatas(chave);

    string chaveCompleta = chave;
    for (char c : alfabeto) {
        if (chaveCompleta.find(c) == string::npos)
            chaveCompleta += c;
    }

    int k = 0;
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            matriz[i][j] = chaveCompleta[k++];
}

void encontrarPosicao(char matriz[5][5], char c, int &linha, int &coluna) {
    if (c == 'J') c = 'I';
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (matriz[i][j] == c) {
                linha = i;
                coluna = j;
                return;
            }
}

string prepararTexto(string texto) {
    string preparado = "";
    for (char c : texto) {
        if (isalpha(c)) {
            c = toupper(c);
            if (c == 'J') c = 'I';
            preparado += c;
        }
    }

    string resultado = "";
    for (size_t i = 0; i < preparado.length(); i++) {
        resultado += preparado[i];
        if (i + 1 < preparado.length()) {
            if (preparado[i] == preparado[i + 1]) {
                resultado += 'X';
            }
        }
    }
    if (resultado.length() % 2 != 0)
        resultado += 'X';

    return resultado;
}

string cifraPlayfair(string texto, char matriz[5][5], bool cifrar) {
    string preparado = prepararTexto(texto);
    string resultado = "";

    int direcao = cifrar ? 1 : 4;  // +1 para cifrar, -1 (equivalente a +4 mod 5) para decifrar

    for (size_t i = 0; i < preparado.length(); i += 2) {
        char a = preparado[i];
        char b = preparado[i + 1];
        int linha1, coluna1, linha2, coluna2;

        encontrarPosicao(matriz, a, linha1, coluna1);
        encontrarPosicao(matriz, b, linha2, coluna2);

        if (linha1 == linha2) {  // Mesma linha
            resultado += matriz[linha1][(coluna1 + direcao) % 5];
            resultado += matriz[linha2][(coluna2 + direcao) % 5];
        } else if (coluna1 == coluna2) {  // Mesma coluna
            resultado += matriz[(linha1 + direcao) % 5][coluna1];
            resultado += matriz[(linha2 + direcao) % 5][coluna2];
        } else {  // Retângulo
            resultado += matriz[linha1][coluna2];
            resultado += matriz[linha2][coluna1];
        }
    }

    return resultado;
}

int main() {
    string chave = "EXEMPLO";
    string texto1 = "ygor eh professor";
    string texto2 = "professor";

    char matriz[5][5];
    gerarMatriz(chave, matriz);

    cout << "Matriz Playfair:\n";
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++)
            cout << matriz[i][j] << ' ';
        cout << endl;
    }

    cout << "\nTexto 1: " << texto1 << endl;
    string cifrado1 = cifraPlayfair(texto1, matriz, true);
    cout << "Cifrado: " << cifrado1 << endl;
    string decifrado1 = cifraPlayfair(cifrado1, matriz, false);
    cout << "Decifrado: " << decifrado1 << endl;

    cout << "\nTexto 2: " << texto2 << endl;
    string cifrado2 = cifraPlayfair(texto2, matriz, true);
    cout << "Cifrado: " << cifrado2 << endl;
    string decifrado2 = cifraPlayfair(cifrado2, matriz, false);
    cout << "Decifrado: " << decifrado2 << endl;

    return 0;
}
