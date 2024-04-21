#ifndef CONTA_H_INCLUDED
#define CONTA_H_INCLUDED

using namespace std;

typedef struct{
    string agencia = "0001";//4 numeros 0001
    string nome = "0";
    string cpf = "0";
    int nConta;//6 numeros
    string senha = "0";//4 numeros
    double saldo = 0.0;
}conta; // Estrutura dos Dados

#endif // CONTA_H_INCLUDED
