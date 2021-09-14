#include "../header/Func.h"
#include "../header/Instrucoes.h"

std::list<std::string> msgErro;

std::string stringToUpper(std::string entrada){
    char saida[entrada.size() + 1];

    for(int i = 0; i < entrada.size(); i++)
        saida[i] = std::toupper(entrada[i]);
    
    saida[entrada.size()] = '\0';

    return saida;
}

std::string deleteComment(std::string linha){

    for (int i = 0; i < linha.size(); i++){

        if (linha[i] == ';')
            linha[i] = '\0';
    }

    return linha;
}

std::string findLabel(int contadorLinha, std::string linha){

    int tam = linha.size();
    std::string labelErro, labelAux;

    for (int i = 0; i < tam; i++){

        if (linha[i] == ':'){

            labelErro = findLabel(contadorLinha, linha.substr(i+2));

            if (labelErro != "")
                geraMsgErro(contadorLinha, "sintático", "Dois rótulos na mesma linha.");

            labelAux = particionaString(linha, i);

            if(!isalpha(labelAux[0]))
                geraMsgErro(contadorLinha, "Léxico", "Token inválido");
            
            return labelAux;
        }
    }

    return "";
}

std::string findOperation(std::string linha, int labelSize){

    int i = 0, j;
    char retorno[MAX_CHAR_OPERACAO] = "";

    switch (labelSize){
        case 0:
            j = 0;
            break;

        default:
            j = labelSize + 2; // + 2 para pular o ':' e o ' '.
            break;
    }

    while(linha[j] != ' ' && linha[j] != '\0' && linha[j] != '+') {

         retorno[i] = linha[j];
         i++;
         j++;
    }  

    return retorno;
}

operandos_t findOperands(int contadorLinha, std::string linha, std::string operador){

    operandos_t retorno;

    int i = 0, operando_aux = 0;
    int posicao = linha.find(operador) + operador.size();

    retorno.flag = false;

    while (linha[posicao] != '\0'){

        if (operando_aux == 0 && linha[posicao] == ','){
            operando_aux++;
            i = 0;

        } else if (operando_aux == 1 && linha[posicao + 1] == ',')
            retorno.flag = true;
        
        retorno.operandos[operando_aux][i] = linha[posicao + 1];

        if(retorno.operandos[operando_aux][i] == ',')
            retorno.operandos[operando_aux][i] = '\0';

        i++;
        posicao++;
    }

    if((retorno.operandos[0][0] != '\0' && operador != "CONST" && operador != "SPACE" &&!isalpha(retorno.operandos[0][0])) || (retorno.operandos[1][0] != '\0' && !isalpha(retorno.operandos[1][0])))
        geraMsgErro(contadorLinha, "Léxico", "Token inválido");
    
    return retorno;
}

std::string particionaString(std::string linha, int fim){

    char retorno[MAX_CHAR_ROTULO] = "";

    for (int i = 0; i < fim; i++)
        retorno[i] = linha[i];
    
    return retorno;
}

std::string removeWhitespace(std::string str){

    str.erase(std::remove_if(str.begin(), 
                              str.end(),
                              [](unsigned char x){return std::isspace(x);}),
                str.end());

    return str;
}

void geraMsgErro(int contadorLinha, std::string tipoErro, std::string erro){

    msgErro.push_back("Erro " + tipoErro + " na linha " + std::to_string(contadorLinha) + ": " + erro);
    return;
}

void printErros(){

    for(std::list<std::string>::iterator it = msgErro.begin(); it != msgErro.end(); it++)
        std::cout << *it << std::endl;
}

int getJump(char* operando){

    int jump_int, aux;
    char jump[100] = "";
    bool flag = false;

    for(int i = 0; i < strlen(operando); i++){

        if(flag)
            jump[i - (strlen(operando) - 1)] = operando[i];

        if (operando[i] == '+'){

            aux = i;
            flag = true;
        }
    }
    
    if(operando[0] == 0 || !flag)
        return 0;

    operando[aux] = '\0';

    jump_int = std::stoi(jump);
    return jump_int;
}

int getJump(std::string operando){

    int jump_int, aux;
    char jump[100] = "";
    bool flag = false;

    for(int i = 0; i < operando.size(); i++){

        if(flag)
            jump[i - (operando.size() - 1)] = operando[i];

        if (operando[i] == '+'){

            aux = i;
            flag = true;
        }
    }
    
    if(operando[0] == 0 || !flag)
        return 0;

    operando[aux] = '\0';

    jump_int = std::stoi(jump);
    return jump_int;
}

void eraseAllSubStr(std::string & mainStr, const std::string & toErase) {

    size_t pos = std::string::npos;

    while ((pos = mainStr.find(toErase) )!= std::string::npos)
        mainStr.erase(pos, toErase.length());
}

std::string operator * (std::string a, unsigned int b) {
    std::string output = "";
    while (b--) {
        output += a;
    }
    return output;
}