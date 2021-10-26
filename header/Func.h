#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cctype>
#include <list>

#define MAX_CHAR_ROTULO 50
#define MAX_CHAR_VARIAVEL 50
#define MAX_CHAR_OPERACAO 8

typedef struct operandos {

    char operandos[2][MAX_CHAR_VARIAVEL] {""};
    bool flag;

} operandos_t;

std::string operator * (std::string, unsigned int);

int montar(std::string, std::string); // Realiza a montagem.
std::string stringToUpper(std::string);
std::string findLabel(int, std::string); // Retorna o rótulo da linha. Se não houver, retorna string vazia.
std::string particionaString(std::string, int); // Obtém pedaço da string a partir do ponto inicial até a posição definida.
std::string removeWhitespace(std::string);
std::string findOperation(std::string, int);
std::string deleteComment(std::string); // substitui o caracter ';' que inicia o comentário por um '\0', efetivamente excluindo o comentário da linha.
operandos_t findOperands(int, std::string, std::string);
void geraMsgErro(int, std::string, std::string);
int getJump(char[MAX_CHAR_VARIAVEL]);
int getJump(std::string);
void eraseAllSubStr(std::string&, const std::string&);
void printErros();

int executar(std::string);

int getTamanho(std::string);
bool encontraWhitespace(std::string);
void preencheEnderecos(std::string, std::vector<std::vector<int>>&);
void preencheEnderecos(std::string, std::vector<std::list<int>>&);
int carregar(std::vector<int>, std::vector<int>, std::vector<std::string>);
int soma(std::vector<int>);
std::string preencheArquivo(std::vector<int>, std::vector<int>);

#endif