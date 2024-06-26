#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <limits>
#include <random>
#include <ctime>
#include <chrono>
#include <locale.h>
#include "Conta.h"

using namespace std;


default_random_engine generator(time(0));  // "Semente dos segundos do computador" para o GerarNumeroConta()


conta contAtual;  // Variável global feita a partir da biblioteca Conta.h


//                       Prototipações
//------------------------------------------------------------------------

void CriarArqText();
void Controle(int o);
void MenuPrincipal();
int TelaEscolhaMenuPrincipal();


//------------------------------ Logins-----------------------------------

bool Login();
void Sacar();   // Sobrecarga @
void Extrato();
void MenuLogin();
void Depositar(); // Sobrecarga @
void FecharConta();
bool Transferencia();
void Sacar(double valor);  // Sobrecarga @
void TelaMenuLogin(int &opcao);
void ImportarDados(string cpf); 
void Depositar(string nConta, double valor); // Sobrecarga @


//------------------------------ Cadastros--------------------------------

void GravarConta();


//------------------------------ Conta -----------------------------------

int GerarNumeroConta();
string VerificaNConta();
string VerificaNcontaArq();


//------------------------------ Nome ------------------------------------

string VerificaNomeCadastro();


//------------------------------ CPF -------------------------------------
string VerificaCPFLogin();
string VerificaCPFCadastro();
bool verificarCpfNoArquivo(string cpf);


//------------------------------ Senha -----------------------------------

string VerificaSenhaLogin();
string VerificaSenhaCadastro();
bool verificarCPFSenha(string cpf, string senha);


//---------------------------Gráficas-------------------------------------

void LimparCin();
void SetColor(int ForgC);
void Letreiro();
void LimparTela();
void InformacoesCliente();
void TelaMenuLogin();
void LimparTelaLogin();
void TelaMenuCadastro();
void MostrarInformacaoCadastro();


//-------------------------Controles--------------------------------------

int ComandosTecladoMenu(const char* op[]);
int ComandosTecladoMenuPrincipal( const char* op[]);


//                      Main
//------------------------------------------------------------------------

int main() {

    setlocale(LC_ALL,"portuguese");
    CriarArqText();

    MenuPrincipal();

    return 0;

}


//                     Definições das funções
//------------------------------------------------------------------------


/* Está função serve para criar os arquivos que guardam as informações
 * das contas dos clientes e criar os arquivos das movimentações (Extrato), 
 * que cada  cliente fez como Sacar, Depositar e transferir.*/

void CriarArqText() {

    fstream arquivo, Extrato;
    arquivo.open("Contas.txt", ios::app); // Abre o arquivo em modo de anexação.
    Extrato.open("Extrato.txt", ios::app); // Abre o arquivo em modo de anexação.

    if (Extrato.fail()) {// Verifica se foi aberto com sucesso.
        cout<<"\nFALHA AO ABRIR O BANCO DE DADOS!"<<endl;
    }

    Extrato.close();// Fecha o arquivo do Extrato.

    if (arquivo.fail()) {// Verifica se foi aberto com sucesso.
        cout<<"\nFALHA AO ABRIR O BANCO DE DADOS!"<<endl;
    }

    arquivo.close();// Fecha o arquivo das contas.

}


/* Esta função de fato vai controlar as chamadas de todas as funções necessárias
 * para fazer um cadastro ou login correto.*/

void Controle(int o) {

    conta c; // Uma variavel do tipo conta é criada.

    switch(o) {

        /* A variavel "o" foi mandada pela função MenuPrincipal apartir de uma das
         * 3 opções que ele pode escolher da função TelaEscolhaMenuPrincipal,
         * "NOVO CADASTRO", "LOGAR NA CONTA", "SAIR DO APP".*/

        case 0: // Cadastro

            TelaMenuCadastro();

            contAtual.nome = VerificaNomeCadastro();// A conta global recebe o nome do cliente.

            contAtual.cpf = VerificaCPFCadastro();// A conta global recebe o CPF do cliente.

            contAtual.senha = VerificaSenhaCadastro();

            contAtual.nConta = GerarNumeroConta();

            contAtual.saldo = c.saldo;

            GravarConta(); // GravarConta() serve para gravar os dados de cadastro aprovados no arquivo.

            MostrarInformacaoCadastro();

            LimparTela();

            break;

        case 1: // Login

            TelaMenuLogin();

            bool prossiga;

            do {
                prossiga = Login();

                if (!prossiga) {
                    cout<<"\nNOME OU SENHA INCORRETO(S)!"<<endl;
                } else {
                    cout<<"\nLOGOU COM SUCESSO!"<<endl;
                }
            } while (!prossiga);

            Sleep(2000);
            system("cls");
            MenuLogin();
            break;

    }

}


/* Está função serve para pedir a opção desejada do usuário dentro do menu principal, que 
 * tem as opções de "NOVO CADASTRO", "LOGAR NA CONTA", "SAIR DO APP", assim a função
 * fica chamando a função Controle para acessar a opção escolhida até que ele deseje sair.*/

void MenuPrincipal() {

    int opcao;

    do {

        /* "opcao" vai receber algum número das 3 opções disponiveis
         * "NOVO CADASTRO", "LOGAR NA CONTA", "SAIR DO APP".*/

        opcao = TelaEscolhaMenuPrincipal();
        Sleep(300);
        system("cls");

        switch(opcao) {//3 tipos de opçoes podem estar na "opcao", recebida pela chamada de função anteriormente a essa.

            case 0: // Novo Cadastro.

                Controle(0);//Essa função vai cuidar de todas as verificações necessárias ao novo cadastro.
                break;

            case 1:

                Controle(1);
                break;

            case 2:

                Sleep(2000);
                cout << "\nAPLICATIVO FINALIZADO COM SUCESSO!\n" << endl;
                Sleep(2000);

        }

    }while(opcao != 2);

}


/* Está função serve para o cliente escolher ao entrar no menu principal o que ele quer fazer,
 * "NOVO CADASTRO", "LOGAR NA CONTA", "SAIR DO APP", retornando para quem chamou o número da
 * opção escolhida. A função que captura a opção escolhida é a ComandosTecladoMenuPrincipal.*/

int TelaEscolhaMenuPrincipal() {

    int* option = new int;// Para guardar uma dessas opções que estão nesse vetor abaixo.
    const char *opcoes[] = {"NOVO CADASTRO", "LOGAR NA CONTA", "SAIR DO APP"};// Essas são as opções que pode se escolher.

    Letreiro();

    // Passa o nome das opções para a função e ela devolve a escolhida para "option".
    *option = ComandosTecladoMenuPrincipal(opcoes);

    return *option;// Retorna para quem chamou o número de uma das 3 opções.

}


//------------------------------ Login -----------------------------------


/* Está função serve para pedir ao usuário para digitar a senha da sua conta e o CPF, se faz
 * isso para verificar no banco de dados (arquivo de texto), se o CPF e a senha já existem
 * e se são realmente do usuário que digitou as informações.*/

bool Login() {

    string cpf, senha;
    bool correto;

    do {

        cpf = VerificaCPFLogin();
        contAtual.cpf = cpf;

        senha = VerificaSenhaLogin();
        contAtual.senha = senha;

        if (verificarCPFSenha(cpf,senha)) {
            correto = true;
            ImportarDados(cpf);
        } else {
            correto = false;

            cout<<"\nSENHA INCORRETA!"<<endl;

            LimparTela();
            TelaMenuLogin();
        }
        
    }while(!correto);


    if(correto){
        return true;
    }else{
        return false;
    }

}


/* Está função serve para sacar um valor da conta atual que está logada no aplicativo (a que está
 * dentro da variavel global contAtual), pedindo o valor que deseja sacar e subtraindo da variavel 
 * e de dentro do banco de dados (arquivo de texto), atualizando ele ao final da função e 
 * registrando essa movimentação no Extrato da conta.*/

void Sacar() {   // Sobrecarga @

    ifstream arquivo("Contas.txt");
    fstream Extrato("Extrato.txt");
    ofstream temp("temp.txt");

    string linha, nconta = to_string(contAtual.nConta); // Converte um int para string.
    int numeroDaLinhaConta = 0, linhaSaldo;
    double valorSaque;

    do {
        LimparTelaLogin();
        InformacoesCliente();

        cout<<"\nDESEJA SACAR QUAL VALOR? R$ ";
        cin>>valorSaque;
        LimparCin();

        if ((valorSaque <= 0) || (valorSaque > contAtual.saldo)) {
            cout<<"\nVALOR DE SAQUE INVALIDO!\nTENTE NOVAMENTE\n";
            Sleep(2000);
        }

    }while((valorSaque <= 0) || (valorSaque > contAtual.saldo));//Aqui foi feito o tratamento para ver se e possivel Sacar.

    if ((arquivo.is_open()) && (temp.is_open())) {

        while (getline(arquivo, linha)) {
            numeroDaLinhaConta++;

            if (linha == nconta) { // Verifica se a linha corresponde a conta.
                linhaSaldo = numeroDaLinhaConta + 2;
            }

            if (numeroDaLinhaConta == linhaSaldo) {
                double saldoAtual = stod(linha); // Converte o saldo atual para double(a linha do arquivo com o saldo antigo sem saque).
                double novoSaldo = saldoAtual - valorSaque; // Adiciona o valor do saque ao saldo atual.
                contAtual.saldo = novoSaldo;
                temp<<novoSaldo << endl; // Escreve o novo saldo.
            } else {
                temp << linha << endl; // Copia a linha original.
            }

        }

        if (Extrato.is_open()) {
            Extrato.seekp(0, ios::end);
            auto Tempo_Atual = chrono::system_clock::now();

            time_t Tempo_AtualSec = chrono::system_clock::to_time_t(Tempo_Atual);

            Extrato << nconta <<" RETIROU O VALOR DE "<< valorSaque << "R$. HORARIO: " << ctime(&Tempo_AtualSec);

            Extrato.close();
        }

        arquivo.close();
        temp.close();
        remove("Contas.txt"); // Remove o arquivo original.
        rename("temp.txt", "Contas.txt"); // Renomeia o arquivo temporário.
    } else {
        cout << "FALHA AO ACESSAR BANCO DE DADOS!\n";
    }

    LimparTelaLogin();
    InformacoesCliente();

    cout << "\nSAQUE REALIZADO COM SUCESSO!" << endl;
    Sleep(2000);
    system("cls");

}


/* Está função serve para pesquisar todos as movimentações da conta que está logada no aplicativo
 * (a que está dentro da variavel global contAtual), e colocar todas essas movimentações que estão
 * guardadas no banco de dados (arquivo de texto), dentro de um vetor dinâmico de strings e depois
 * imprimindo na tela as movimentações do tipo saque, deposito, transferência.*/

void Extrato() {

    fstream Extrato("Extrato.txt");

    if (Extrato.is_open()){
        int Movi = 0, cont = 1;
        string linha;
        string *linha_T = new string;

        LimparTelaLogin();
        InformacoesCliente();

        cout << "\nEXTRATO - CONTA: " << contAtual.nConta << "\n" << endl;

        while (getline(Extrato, linha)) {
            string numerosLidos = linha.substr(0, 6); // Pega os 6 primeiros digitos.

            int numerosLido = stoi(numerosLidos); // Transforma os digitos em inteiro.
            if (numerosLido == contAtual.nConta) { // Compara eles com o Numero da conta Atual.
                Movi = 1;
                *linha_T = linha.substr(0);
                //Só para ficar mais organizado na hora de imprimir.
                cout <<"-------------------------------------------------------------------------------------------------"<<endl;
                cout << *linha_T << endl;
                cont++;
                linha_T = new string[cont];
            }
        }
        if (Movi != 1) {
            cout << "\nNAO HOUVE MOVIMENTACAO NESSA CONTA"<< endl;
            Sleep(2000);
        }
        Extrato.close();
    } else {
        cout << "\nERRO AO ABRIR O ARQUIVO" <<endl;
        Sleep(2000);
    }

    system("pause");
    system("cls");

}
 

/* Está função serve para o usuário navegar entre as opções do banco (coisas que ele pode ter
 * acesso depois de logar na sua conta), até que ele queira sair ou fechar a conta. A função
 * fica chamando a TelaMenuLogin() todas as repetições para saber o que deseja.*/

void MenuLogin() { 

    int op;

    do {
        TelaMenuLogin(op);// Foi passado por referencia a variavel "op".

        switch(op) {
            case 0: 
                Depositar();
                break;
            case 1:
                Sacar();
                break;
            case 2:
                Transferencia();
                break;
            case 3:
                Extrato();
                break;
            case 4:
                FecharConta();
                break;
            case 5:
                LimparTelaLogin();
                InformacoesCliente();
                cout << "\nVOLTANDO PARA O MENU PRINCIPAL" << endl;
                Sleep(2000);
                system("cls");
                break;
        }

    }while(op != 4 && op != 5);

}


/* Está função serve para depositar valores na conta atual que está
 * usando o sistema (que está dentro da variável global "contAtual"),
 * basicamente pede o valor que deseja e coloca no arquivo,
 * atualizando o valor da variavel global também.*/

void Depositar() {  // Sobrecarga @

    ifstream arquivo("Contas.txt");
    fstream Extrato("Extrato.txt");
    ofstream temp("temp.txt");

    string linha,  nconta = to_string(contAtual.nConta);// Converte um int para string.;
    int numeroDaLinhaConta = 0, linhaSaldo;
    double valorDeposito;

    do {
        LimparTelaLogin();
        InformacoesCliente();

        cout << "\nDIGITE O VALOR QUE DESEJA DEPOSITAR: R$ ";
        cin >> valorDeposito;
        LimparCin();

        if (valorDeposito <= 0) {
            cout << "\nVALOR DE DEPOSITO INVALIDO, TENTE NOVAMENTE!\n";
            Sleep(2000);
        }
        
    }while(valorDeposito <= 0);

    if (arquivo.is_open()&& temp.is_open()) {

        while (getline(arquivo, linha)) {
            numeroDaLinhaConta++;

            if (linha == nconta) { // Verifica se a linha corresponde a conta.
                linhaSaldo = numeroDaLinhaConta + 2;
            }

            if (numeroDaLinhaConta == linhaSaldo) {
                double saldoAtual = stod(linha); // Converte o saldo atual para double(a linha do arquivo com o saldo antigo sem deposito)
                double novoSaldo = saldoAtual + valorDeposito; // Adiciona o valor do dep sito ao saldo atual
                contAtual.saldo = novoSaldo;
                temp << novoSaldo << endl; // Escreve o novo saldo
            } else {
                temp << linha << endl; // Copia a linha original
            }

        }

        if (Extrato.is_open()) {
            Extrato.seekp(0, ios::end);
            auto Tempo_Atual = chrono::system_clock::now();

            time_t Tempo_AtualSec = chrono::system_clock::to_time_t(Tempo_Atual);

            Extrato << nconta <<" DEPOSITOU O VALOR DE "<< valorDeposito << "R$. HORARIO: " << ctime(&Tempo_AtualSec);

            Extrato.close();
        }

        arquivo.close();
        temp.close();
        remove("Contas.txt"); // Remove o arquivo original
        rename("temp.txt", "Contas.txt"); // Renomeia o arquivo temporário
    } else {
        cout << "FALHA AO ABRIR O ARQUIVO.\n";
        Sleep(2000);
    }

    LimparTelaLogin();
    InformacoesCliente();

    cout << "\nDEPOSITO REALIZADO COM SUCESSO!" << endl;
    Sleep(2000);

    system("cls");
}


/* Está função serve para apagar a conta do usuário que está utilizando
 * o aplicativo no momento, encontrando a conta do arquivo de texto (
 * banco de dados), em seguida reescrevendo ele sem a conta do usuário
 * atual.*/

void FecharConta() {

    ifstream arquivo("Contas.txt");
    ofstream temp("temp.txt");

    string linha;
    string nconta = to_string(contAtual.nConta); //converte um int para string.
    int continuar = 0;
    int contLinha = 0;//Contador de linhas.
    int linhaInicio = 0;//Para guardar apartir de onde eu apago as informacoes.
    int linhaFim = 0;//Para guardar aonde eu paro de apagar as informacoes.

    const char* opcoes[] = { "SIM", "NAO" };//Opções disponíveis para escolher.
    int opcao = 0;
    char tecla;

    cout <<endl<< "DESEJA REALMENTE CANCELAR A SUA CONTA?: "<<endl;

    do {

        for (int i = 0; i < 2; i++) {
            if (i == opcao)
                cout << "> "; // Imprime a seta.
            else
                cout << "  ";
                cout << opcoes[i] << "    ";
        }

        tecla = _getch(); // Captura a tecla pressionada.

        if (tecla == 75) { // Tecla de seta para a esquerda.
            if (opcao > 0)
                opcao--; // Opção decrementa.
        } else if (tecla == 77) { // Tecla de seta para a direita.
            if (opcao < 1)
                opcao++; // Opção incrementa.
        }

        if(tecla != 13){
            system("cls");
            InformacoesCliente();
            cout <<endl<< "DESEJA REALMENTE CANCELAR A SUA CONTA?: "<<endl;
        }

    }while (tecla != 13); // Repete até que a tecla Enter (13 na tabela ASCII) seja pressionada.

    LimparCin();
    if (opcao == 0) {

        if (arquivo.is_open()&& temp.is_open()) {

            while(getline(arquivo, linha)) {
                contLinha++;

                if (linha == nconta) { // Verifica se a linha corresponde a conta.
                    linhaInicio = contLinha - 3;
                    linhaFim = contLinha + 2;
                    break;
                }

            }

            arquivo.clear();
            arquivo.seekg(0, ios::beg); // Limpa o estado do fluxo e move o ponteiro do arquivo de volta para o inicio.
            
        } else {
            cout << "\nFALHA AO ACESSAR O BANCO DE DADOS!\n";
            Sleep(2000);
            return;
        }

        contLinha = 0;

        if (arquivo.is_open()&& temp.is_open()) {

            while (getline(arquivo, linha)) {
                contLinha++;

                if ((contLinha >= linhaInicio) && (contLinha <= linhaFim)) { // Verifica se ja chegou nas linhas de apagar.
                    continue;
                }

                temp<<linha<<endl;
            }

            arquivo.close();
            temp.close();
            remove("Contas.txt"); // Remove o arquivo original.
            rename("temp.txt", "Contas.txt"); // Renomeia o arquivo temporário.

        } else {
            cout << "\nFALHA AO ACESSAR O BANCO DE DADOS!\n";
            Sleep(2000);
            return;
        }

        Sleep(2000);
        cout << "\nCONTA FINALIZADA COM SUCESSO!" << endl;
        Sleep(2000);
        system("cls");

    }

    system("cls");

}


/* Está função serve para o usuário atual (o que está com as informações
 * dentro da variavel global "contAtual"), transfira um valor da sua conta
 * para uma outra que existe dentro do arquivo de texto (banco de dados).
 * Para isso vai ter que depositar na conta de outra pessoa com o auxílio
 * da função com sobrecarga que tem parâmetros Depositar(), pedindo o 
 * número da conta da pessoa e vendo se ela existe, depois é retirado 
 * esse valor depositado da conta atual com o auxílio da função 
 * Sacar() que está com sobrecarga e tem parâmetros.*/

bool Transferencia() {

    fstream Extrato("Extrato.txt");
    string nContaDepositar = VerificaNcontaArq();
 
    if (nContaDepositar == "Nao achou") {
        return false;// Ou ele desistiu de transferir ou a conta nao existe e retorna false.
    }

    double valor;

    do {

        LimparTelaLogin();
        InformacoesCliente();

        cout<<endl<<"DESEJA TRANSFERIR QUAL VALOR? R$ ";
        cin>>valor;
        LimparCin();

        if (valor <= 0) {
            cout << "\nVALOR DO DEPOSITO INVALIDO, TENTE NOVAMENTE!"<<endl;
            Sleep(2000);
        } else if (valor > contAtual.saldo || valor == 0) {
                cout<<"\nSALDO INSUFICIENTE!"<<endl;
                Sleep(2000);
        } else {
            cout<< "\nTRANSFERENCIA EFETUADA COM SUCESSO!" << endl;
            Sleep(2000);
        }

    } while ((valor <= 0) || (valor > contAtual.saldo));

    Sacar(valor);// Passa o valor a ser sacado da conta atual.
    Depositar(nContaDepositar,valor);// Passa o valor desejado para colocar na conta da outra pessoa e a conta.

    if (Extrato.is_open()) {
        Extrato.seekp(0, ios::end);
        auto Tempo_Atual = chrono::system_clock::now();
        time_t Tempo_AtualSec = chrono::system_clock::to_time_t(Tempo_Atual);

        Extrato<<contAtual.nConta<< " FEZ UMA TRANSFERENCIA DE "<< valor << "R$ PARA A CONTA "
        <<nContaDepositar << ". HORARIO: " << ctime(&Tempo_AtualSec);
        Extrato<<nContaDepositar<< " RECEBEU UMA TRANSFERENCIA DE "<<valor<< "R$ DA CONTA "<<
        contAtual.nConta<<". HORARIO: " << ctime(&Tempo_AtualSec);
        Extrato.close();
    }

    LimparTelaLogin();
    InformacoesCliente();
    system("cls");

}


/* Está função faz basicamente a mesma coisa da sacar() sem parâmetros,
 * só que em vez de sacar pedindo o valor dentro da função, aqui
 * passamos o valor como parâmetro. Está função serve como auxiliar
 * para a função Transferencia().*/

void Sacar(double valor){   // Sobrecarga @

    ifstream arquivo("Contas.txt");
    ofstream temp("temp.txt");

    string linha, nconta = to_string(contAtual.nConta); //converte um int para string
    int numeroDaLinhaConta = 0, linhaSaldo;

    if (arquivo.is_open()&& temp.is_open()) {

        while (getline(arquivo, linha)) {
            numeroDaLinhaConta++;

            if (linha == nconta) { // Verifica se a linha corresponde a conta
                linhaSaldo = numeroDaLinhaConta + 2;
            }

            if (numeroDaLinhaConta == linhaSaldo) { // Acha a linha do saldo da nossa conta
                contAtual.saldo = stod(linha) - valor; // Adiciona o valor do saque ao saldo atual
                temp << contAtual.saldo << endl; // Escreve o novo saldo
            } else {
                temp << linha << endl; // Copia a linha original
            }
        }

        arquivo.close();
        temp.close();
        remove("Contas.txt"); // Remove o arquivo original
        rename("temp.txt", "Contas.txt"); // Renomeia o arquivo temporario

    } else {
        cout << "FALHA AO ACESSAR BANCO DE DADOS!\n";
    }

    LimparTelaLogin();
    InformacoesCliente();

    cout << "\nSAQUE REALIZADO COM SUCESSO!" << endl;
    Sleep(2000);

}


/* Está função serve para gerar as opções de funções disponíveis do banco
 * ao logar na conta, chamando a função ComandosTecladoMenu() e passando
 * essas opções.*/

void TelaMenuLogin(int &opcao) {

    const char *opcoes[] = {"DEPOSITO", "SAQUE", "TRANSFERENCIA", "EXTRATO", "FECHAR CONTA", "SAIR DA CONTA"};

    Letreiro();

    opcao = ComandosTecladoMenu(opcoes);

}


/* Está função serve para ao logar em uma conta as informações com
 * base no CPF informado, seja pesquisado no arquivo as linhas que 
 * estão as informações da pessoa e assim passar elas para a variável
 * global "contAtual". Para assim ter acesso as funcionalidades do banco. */

void ImportarDados(string cpf) {

    ifstream arquivo("Contas.txt");
    string linha, aux;
    int numeroDaLinha = 0, numeroLinhaNome, aux2;
    double aux3;

    if (arquivo.is_open()) {

        while (getline(arquivo, linha)) {
            numeroDaLinha++;

            if((numeroDaLinha % 6 == 3) && (linha == cpf)){ // Verifica se a linha corresponde ao CPF.
                numeroLinhaNome = numeroDaLinha - 1;
                contAtual.agencia = "0001";
                contAtual.cpf = cpf;

                for(int c = 0;c < 3;c++){

                    getline(arquivo, linha);

                    switch(c){
                        case 0:
                            aux = linha;
                            aux2 = stoi(aux);
                            contAtual.nConta = aux2;
                            break;
                        case 1:
                            contAtual.senha = linha;
                            break;
                        case 2:
                            aux = linha;
                            aux3 = stod(aux);
                            contAtual.saldo = aux3;
                            break;
                        default:
                            break;
                    }

                }

            }

        }

        arquivo.close();
    } else {
        cout << "\nFALHA AO ABRIR O BANCO DE DADOS"<<endl;
    }

    arquivo.clear();
    arquivo.seekg(0, ios::beg);
    arquivo.open("Contas.txt");
    numeroDaLinha = 0;

    if (arquivo.is_open()) {

        while (getline(arquivo, linha)) {
            numeroDaLinha++;
            if(numeroLinhaNome == numeroDaLinha){
                contAtual.nome = linha;
            }
        }

        arquivo.close();
    }else{
        cout<<"\nFALHA AO ABRIR O BANCO DE DADOS"<<endl;
    }

}


/* Está função faz basicamente a mesma coisa que a função Depositar sem 
 * parâmetros, com a diferença de passar o número da conta e o valor
 * desejado como parâmetro, pois essa função auxília a função Transferencia(),
 * e assim precisa saber as informações passadas como parâmetros para saber
 * que conta depositar o valor transferido, o reconhecimento é feito através
 * do número da conta.*/

void Depositar(string nConta, double valor) { //Sobrecarga @

    ifstream arquivo("Contas.txt");
    ofstream temp("temp.txt");

    string linha;
    int numeroDaLinhaConta = 0, linhaSaldo;

    if (arquivo.is_open()&& temp.is_open()) {

        while (getline(arquivo, linha)) {

            numeroDaLinhaConta++;
            if (linha == nConta) { // Verifica se a linha corresponde a conta.
                linhaSaldo = numeroDaLinhaConta + 2;
            }

            if (numeroDaLinhaConta == linhaSaldo) {
                temp << (stod(linha) + valor) << endl; // Escreve o novo saldo
            } else {
                temp << linha << endl; // Copia a linha original
            }

        }

        arquivo.close();
        temp.close();
        remove("Contas.txt"); // Remove o arquivo original
        rename("temp.txt", "Contas.txt"); // Renomeia o arquivo temporário

    } else {
        cout << "FALHA AO ABRIR O ARQUIVO.\n";
        Sleep(2000);
    }

    LimparTelaLogin();
    InformacoesCliente();
    cout << "\nDEPOSITO REALIZADO COM SUCESSO!" << endl;
    Sleep(2000);

}


//------------------------------ Cadastro ---------------------------


/* Está função serve para passar todos os dados fornecidos no cadastro
 * para o arquivo de texto (banco de dados), e assim criar uma nova conta.*/

void GravarConta() { // Gravar a variável global "contAtual" no arquivo

    fstream arquivo;
    arquivo.open("Contas.txt",ios::app);

    if (arquivo.is_open()) {
        arquivo<<contAtual.agencia<<endl
        <<contAtual.nome<<endl
        <<contAtual.cpf<<endl
        <<contAtual.nConta<<endl
        <<contAtual.senha<<endl
        <<contAtual.saldo<<endl;
        arquivo.close();
    } else {
        cout << "\nFALHA AO ABRIR O ARQUIVO.\n";
    }

}


//------------------------------ Conta ------------------------------


/* Está função serve para gerar um número aleátorio e verificar
 * se ele já existe no arquivo de texto (banco de dados), se 
 * existir ele repete o processo novamente até achar um que
 * não tenha sido usado.*/

int GerarNumeroConta() {

    fstream arquivo;
    string linha, aux;
    uniform_int_distribution<int> distribution(1,999999);
    int NumeroAle;
    bool NaoRepetido;

    do {

        NumeroAle = distribution(generator);
        aux = to_string(NumeroAle);
        NaoRepetido = true;
        arquivo.open("Contas.txt",ios::in);

        while(getline(arquivo, linha)){

            if (linha == aux){
                NaoRepetido = false;
                cout<<"\nJA EXISTE A CONTA: "<<NumeroAle<<endl;
                break;
            }

        }

        arquivo.close();
    } while (!NaoRepetido);

    return NumeroAle;

}



/* Esta funcao serve de auxílio para a funcão Transferencia(), retornando
 * (se escrito dentro dos padrões estabelecidos), a string para quem 
 * chamou no caso Transferencia().*/

string VerificaNConta() {

    bool certo = false;
    string nConta;

    while (!certo) {

        LimparTelaLogin();
        InformacoesCliente();

        cout<<"DIGITE O NUMERO DA CONTA PARA A QUAL DESEJA TRANSFERIR: ";
        getline(cin, nConta);

        if (nConta.length() != 6) {
            cout<<"\nO NUMERO DA CONTA DEVE CONTER APENAS 6 DIGITOS!"<<endl;
            Sleep(2000);
            system("cls");
        } else {
            for (char c: nConta) {

                if (isdigit(c)) {
                    certo = true;
                } else {
                    cout<<"\nO NUMERO DA CONTA DEVE CONTER APENAS NUMEROS!"<<endl;
                    certo = false;
                    Sleep(2000);
                    break;
                }
            }
        }
    }

    return nConta;

}


/* Está funcão verifica no arquivo se a conta dessa pessoa existe. Se existir
 * vai imprimir na tela os dados da conta e perguntar para ver
 * se vai prosseguir, se sim, retorna true ou se não existir ou 
 * existir e ele desistir retorna false*/

string VerificaNcontaArq() {

    string auxConta = VerificaNConta();
    string linha;
    ifstream arquivo("Contas.txt");
    int numeroDaLinhaConta = 0, numeroDaLinhaNome;
    bool achou = false;

    if (arquivo.is_open()) {//Abre o arquivo

        while (getline(arquivo,linha)) {//Vai passando linha por linha ate o final do arquivo
            numeroDaLinhaConta++;//Para saber em que linha foi encontrado(retirar depois)

            if (linha == auxConta) {
                numeroDaLinhaNome = numeroDaLinhaConta - 2;
                achou = true;//Se achou a conta
            }
        }

        arquivo.close();//Fecha o arquivo
    } else {
        cout<<"\nFALHA AO ABRIR BANCO DE DADOS!"<<endl;
        system("cls");
        arquivo.close();
    }

    if (!achou) {//Verificando se foi encontrada a conta
        cout<<"\nESSA CONTA NAO FAZ PARTE DESTE BANCO"<<endl;
        Sleep(2000);
        system("cls");
        return auxConta = "Nao achou";//Retorna false se n o entrou dentro if da linha 774
    }

    arquivo.clear();
    arquivo.seekg(0, ios::beg);
    arquivo.open("Contas.txt");
    numeroDaLinhaConta = 0;

    if (arquivo.is_open()) {

        while (getline(arquivo, linha)) {

            LimparTelaLogin();
            InformacoesCliente();
            numeroDaLinhaConta++;

            if (numeroDaLinhaNome == numeroDaLinhaConta) {

                cout<<"DADOS DA CONTA QUE VOCE DESEJA TRANSFERIR\n"<<endl;
                cout<<"NOME: "<<linha<<endl
                <<"NUMERO DA CONTA: "<<auxConta<<endl;
                cout <<endl<< "DESEJA CONTINUAR?"<<endl;

                const char* opcoes[] = { "SIM", "NAO" };
                int opcao = 0;
                char tecla;

                do {

                    for (int i = 0; i < 2; i++) {

                        if (i == opcao) {
                            cout << "> "; // Imprime a seta
                        } else {
                            cout << "  ";
                        }
                        cout << opcoes[i] << "    ";

                    }

                    tecla = _getch(); // Captura a tecla pressionada

                    if (tecla == 75) { // Tecla de seta para a esquerda
                        if (opcao > 0)
                            opcao--; // Opção decrementa
                    } else if (tecla == 77) { // Tecla de seta para a direita
                            if (opcao < 1)
                                opcao++; // Opção incrementa
                    }

                    if (tecla != 13) {
                        system("cls");
                        InformacoesCliente();

                        cout<<endl<<"DADOS DA CONTA QUE VOCE DESEJA TRANSFERIR\n"<<endl;
                        cout<<"NOME: "<<linha<<endl
                        <<"NUMERO DA CONTA: "<<auxConta<<endl;
                        cout <<endl<< "DESEJA CONTINUAR?"<<endl;
                    }

                } while (tecla != 13); // Repete até que a tecla Enter (13 na tabela ASCII) seja pressionada

                LimparCin();
                if (opcao == 0) {
                    return auxConta;//Vai dar prosseguimento a transferencia
                } else {
                    system("cls");
                    return auxConta = "Nao achou";//Deistiu de transferir
                }
            }
        }

        arquivo.close();
    } else {
        cout<<"FALHA AO ABRIR BANCO DE DADOS"<<endl;
        Sleep(2000);
    }

}


//------------------------------ Nome -------------------------------

/* Está função serve para verificar se o usuario não digitou um nome com números, se não digitou
 * números no nome vai retornar o nome do cliente, se não vai ficar em um looping até que se
 * digite o nome correto (Está função não serve para olhar no arquivo (banco de dados) se existe o nome) */

string VerificaNomeCadastro() {

    bool correto = false;
    string nome;

    while (!correto) {

        cout<<"DIGITE O NOME: ";
        getline(cin, nome);

        for (char c: nome) { // Coloca cada caracter da string nesse char "c".
            if (isdigit(c)) { // Verifica letra por letra se não tem números.

                correto = false; //Foi digitado um número no nome.
                cout<<"\nNOME INVALIDO!"<<endl;
                LimparTela();
                TelaMenuCadastro(); // Chama novamente essa função para imprimir o Letreiro e o nome que fica em cima.
                break;

            } else {
                correto = true;
            }
        }
    }

    return nome; // Se chegou até aqui o nome está livre de números.

}


//------------------------------ CPF --------------------------------


/* Está função serve para pedir ao usuário o seu CPF para o login na sua conta, após isso
 * se verifica se está no formato correto (11 números somente e apenas números), repetindo
 * o looping até que ele digite algum CPF no formato correto. */
string VerificaCPFLogin() {
    
    bool verdade = false;
    string cpf;

    while (!verdade) {
        cout << "DIGITE O CPF: ";
        getline(cin, cpf);

        if (cpf.length() != 11) {
            cout << "\nCPF DEVE CONTER 11 NUMEROS!" << endl;
            LimparTela();
            TelaMenuLogin();
        } else {

            for (char c: cpf) {

                if (isdigit(c)) {
                    verdade = true;
                } else {
                    cout << "\nO CPF DEVE CONTER APENAS NUMEROS!" << endl;
                    verdade = false;

                    LimparTela();
                    TelaMenuLogin();
                    break;
                }
            }
        }
    }

    return cpf;

}


/* Está função serve para verificar se o cliente não colocou letras ou mais de
 * 11 números ao digitar o CPF(Está função não vai olhar no arquivo se existe 
 * já esse CPF, apenas chama outra função que ver isso)*/

string VerificaCPFCadastro() {

    bool correto = false;
    string cpf;

    while (!correto) {

        cout << "DIGITE O CPF: ";
        getline(cin, cpf);

        if (cpf.length() != 11) { // Verifica se tem exatamente 11 números.
            cout << "\nCPF DEVE CONTER 11 NUMEROS!" << endl;

            LimparTela();
            TelaMenuCadastro(); // Chama novamente essa função para imprimir o Letreiro e o nome que fica em cima.

            cout << "DIGITE O NOME: " << contAtual.nome << endl;

        } else {

            for (char c: cpf) { // Coloca cada caracter da string nesse char "c".

                if (isdigit(c)) { // Verifica se são números.
                    correto = true;
                } else {
                    cout << "\nO CPF DEVE CONTER APENAS NUMEROS!" << endl;
                    correto = false;

                    LimparTela();
                    TelaMenuCadastro();//Chama novamente essa função para imprimir o Letreiro e o nome que fica em cima.

                    cout << "DIGITE O NOME: " << contAtual.nome << endl;

                    break;
                }
            }
        }
    }

    do {

        if (!verificarCpfNoArquivo(cpf)) {// Verifica se aquele CPF digitado anteriormente se já existe no arquivo.
            cout << "\nCPF JÁ UTILIZADO!" << endl; // Se já tem alguem com esse CPF aparece a mensagem de já utiliazdo.

            LimparTela();
            TelaMenuCadastro();// Chama novamente essa função para imprimir o Letreiro e o nome que fica em cima.

            cout << "DIGITE O NOME: " << contAtual.nome << endl;

            // Chama está função que estamos agora dentro dessa função para repetir tudo do começo de novo.
            cpf = VerificaCPFCadastro();
        }

    } while (!verificarCpfNoArquivo(cpf));//Enquanto ele não digitar um CPF que não existe no arquivo vai continuar repetindo.

    return cpf;//Se ocorrer tudo certo irá retornar o cpf para quem chamou.

}

/* Está função vai abrir o arquivo e verificar se o CPF passado por parametro já existe no arquivo
 * retorna false se já existir, retorna true se não existir.*/

bool verificarCpfNoArquivo(string cpf) {

    ifstream arquivo("Contas.txt");
    string linha;

    if (arquivo.is_open()) { // Abre o arquivo.

        while (getline(arquivo, linha)) { // Coloca tudo que está em cada linha do arquivo na variável.

            if(linha == cpf) { // Se o CPF ja existir retorna false.
                arquivo.close(); // Fecha o arquivo.
                return false;
            }
        }

        arquivo.close();
    } else {
        cout << "Falha ao abrir banco de dados\n";
    }

    return true;

}


//------------------------------ Senha ------------------------------


/* Está função basicamente só pede para digitar a senha e verifica se está
 * no padrão certo, 4 números.*/

string VerificaSenhaLogin() {

    bool verdade = false;
    string senha;

    while (!verdade) {

        cout << "DIGITE A SENHA(4 NUMEROS): ";
        getline(cin, senha);

        if (senha.length() != 4) {
            cout << "\nA SENHA DEVE CONTER APENAS 4 NUMEROS"<<endl;

            LimparTela();
            TelaMenuLogin();

            cout << "DIGITE O CPF: " << contAtual.cpf << endl;

        } else {

            for (char c: senha) {

                if (isdigit(c)) {
                    verdade = true;
                } else {
                    cout<<"\nA SENHA DEVE CONTER APENAS NUMEROS"<<endl;
                    verdade = false;

                    LimparTela();
                    TelaMenuLogin();

                    cout << "DIGITE O CPF: " << contAtual.cpf << endl;
                    break;

                }
            }
        }
    }

    return senha;

}

/* Está funcao vai verificar se o usuario digitou uma senha correta,
 * está função é chamada na cadastro().*/

string VerificaSenhaCadastro() {

    bool verdade = false;
    string senha;

    while (!verdade){

        cout<<"DIGITE A SENHA(4 NUMEROS): ";
        getline(cin, senha);

        if (senha.length() != 4) {
            cout<<"\nA SENHA DEVE CONTER APENAS 4 NUMEROS"<<endl;

            LimparTela();
            TelaMenuCadastro();

            cout << "DIGITE O NOME: " << contAtual.nome << endl;
            cout << "DIGITE O CPF: " << contAtual.cpf << endl;

        } else {

            for (char c: senha) {
                if(isdigit(c)){
                    verdade = true;
                } else {
                    cout<<"\nA SENHA DEVE CONTER APENAS NUMEROS"<<endl;
                    verdade = false;

                    LimparTela();
                    TelaMenuCadastro();

                    cout << "DIGITE O NOME: " << contAtual.nome << endl;
                    cout << "DIGITE O CPF: " << contAtual.cpf << endl;

                    break;
                }
            }
        }
    }

    return senha;

}


/* Está função serve para verificar se o CPF e a Senha são da mesma pessoa
 * que foi passada na Login(). Entra no arquivo e chega até a linha do CPF 
 * e confirma se é o mesmo passado, depois pula 2 linhas e chega na senha
 * e confirma se bate com a que foi passada.*/

bool verificarCPFSenha(string cpf, string senha) {

    ifstream arquivo("Contas.txt");
    string linha;
    int numeroDaLinha = 0;

    if (arquivo.is_open()) {

        while (getline(arquivo, linha)) {
            numeroDaLinha++;
            if (linha == cpf) { // Verifica se a linha corresponde ao CPF.

                for (int c = 0;c < 2;c++) {
                    getline(arquivo, linha);
                        if(linha == senha){
                            arquivo.close();
                            return true;
                        }
                    }

                arquivo.close();
            }
        }

        arquivo.close();
    } else {
        cout << "Falha ao abrir banco de dados\n";
    }

    return false;

}


//---------------------------------Gráficas-----------------------------------------------


/* Deixa o Cin operacional novamente por meio da biblioteca "limits".*/

void LimparCin() {

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

}


/* Está funcao serve para se caso querermos mudar as 
 * cores das letras usa a biblioteca windows.h.*/

void SetColor(int ForgC) {

    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }

    return;

}


/* Meramente para colocar o nome "Vestido Bank" na tela e mudar a cor da letras,
 * pois chama a função SetColor(passando 13,roxo).*/

void Letreiro() {

    SetColor(13);
    cout<<" 888     888                   888    d8b      888               888888b.                     888   \n";
    cout<<" 888     888                   888    Y8P      888               888  88b                     888   \n";
    cout<<" 888     888                   888             888               888  .88P                    888   888\n";
    cout<<" Y88b   d88P  .d88b.  .d8888b  888888 888  .d88888   .d88b.      8888888K.   8888b.  88888b.  888  888\n";
    cout<<"  Y88b d88P  d8P  Y8b 88K      888    888 d88  888   d8888b      888  Y88b       88b 888  88b 888 .88P\n";
    cout<<"   Y88o88P   88888888 Y8888b.  888    888 888  888  888  888     888    888 .d888888 888  888 888888K\n";
    cout<<"    Y888P    Y8b.         X88  Y88b.  888 Y88b 888  Y88..88P     888   d88P 888  888 888  888 888 88b\n";
    cout<<"     Y8P      Y8888   88888P    Y888  888  Y88888     Y88P       8888888P   Y888888  888  888 888  888\n\n\n\n\n";

}


/* Só para aparecer o nome limpar a tela, pausar e limpar. */

void LimparTela() {

    cout << "\nLIMPAR TELA: ";
    system("pause");
    system("cls");

}


/* Vai servir para colocar as informações da "contAtual" na tela. */

void InformacoesCliente() {

    cout << "------------------------------------------------------------\n" << endl
         << "BEM-VINDO(A), " << contAtual.nome << "\n" << endl
         << "AGENCIA: " << contAtual.agencia << endl
         << "CONTA: " << contAtual.nConta << endl
         << "SALDO: R$" << contAtual.saldo << "\n" << endl
         << "------------------------------------------------------------\n" << endl;
         
}


/* Apenas para colocar o leetreiro na tela o nome "Login". */

void TelaMenuLogin() {   // Sobrecarga @

    Letreiro();

    cout << "-------------------- LOGIN -------------------------\n" << endl;

}


/* Apenas para limpar a tela e colocar o letreiro.*/

void LimparTelaLogin() {

    system("cls");
    Letreiro();

}


/*Apenas para mostrar o letreiro e o nome "CADASTRO DE CONTA" na tela*/

void TelaMenuCadastro() {

    Letreiro();

    cout << "-------------------- CADASTRO DE CONTA -------------------------" << endl;

}


/* Está função serve para na hora que cadastrar alguma conta apareça
 * todas as informações da nova conta na tela, pois a mesma está
 * na variável global "contAtual".*/

void MostrarInformacaoCadastro() {

    cout << "\nCONTA APROVADA!" << endl;

    LimparTela();
    TelaMenuCadastro();

    cout << "CONTA APROVADA!" << endl;

    cout << "-------------------- INFORMACOES DO USUARIO --------------------" << endl
         << "NOME: " << contAtual.nome << endl
         << "CPF: " << contAtual.cpf << endl
         << "-------------------- INFORMACOES DA CONTA   --------------------" << endl
         << "AGENCIA: " << contAtual.agencia << endl
         << "CONTA: " << contAtual.nConta << endl;

}


//-------------------------Controles------------------------------------


/* Está função dá suporte ao menu de funcionalidades do banco. Basicamente é
 * um looping até que alguma opção seja pressionada usando as teclas para
 * cima e para baixo do teclado, até que clique no enter e escolha alguma
 * opção e assim retorne essa opção para quem chamou.*/

int ComandosTecladoMenu(const char* op[]) {

    int x = 0, tecla = 13;

    InformacoesCliente();

    do {

        if (tecla != 13) {
            system("cls");
            Letreiro();
            InformacoesCliente();
        }

        for (int i = 0; i < 6; i++) {

            if (i == x)
                cout << ">";
            else {
                cout << " ";
            }
            cout << op[i] << "\n";
        }

        tecla = _getch(); // Captura a tecla pressionada

        if (tecla == 72) { // Tecla de seta para cima
            if (x > 0)
                x--;
        } else if (tecla == 80) { // Tecla de seta para baixo
            if (x < 5)
                x++;
        }
    } while (tecla != 13); // Repete ate que a tecla Enter seja pressionada

    return x;

}


/* Está função vai receber o vetor de characteres contendo as 3 opções do menu principal
 * "NOVO CADASTRO", "LOGAR NA CONTA", "SAIR DO APP", e dentro de um looping vai ficar 
 * imprimindo essas opçoes junto com a função Letreiro, ate o cliente escolher uma opção
 * valida pressionando o enter (13 na tabela ASCII). */

int ComandosTecladoMenuPrincipal(const char* op[]) {
        
    int opcao = 0,  tecla = 13; 

    do {
        //Enquanto não clicar no enter(13 na tabela ASCII) vai continuar limpando a tela e colocando o Letreiro
            
        if (tecla != 13) {
            system("cls");
            Letreiro();
        }

        for (int i = 0; i < 3; i++) {
            if (i == opcao) {
                cout << ">";//Imprimi a seta
            } else {
                cout << " ";
            }
            cout << op[i] << endl;//Imprimi as 3 opções 3 vezes dentro desse for
        }

        tecla = _getch(); // Captura a tecla pressionada

        if (tecla == 72) { // Tecla de seta para cima
            if (opcao > 0)
                opcao--;//Opção decrementa
        } else if (tecla == 80) { // Tecla de seta para baixo
            if (opcao < 2)
                opcao++;//Opção incrementa
        }
    } while (tecla != 13); // Repete ate que a tecla Enter(13 na tabela ASCII) não seja pressionada

    return opcao;//Retorna a opção escolhida

}
