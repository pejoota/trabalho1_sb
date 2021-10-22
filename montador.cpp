#include "src/Func.cpp"
#include "src/Instrucoes.cpp"

int main (int argc, char **argv){

    char escolha;
    int ctrl = argc;

    switch (ctrl){

        case 3:
        std::cout << "Digite o nome do programa .asm a ser montado:\n>> ";
        std::cin >> argv[1];

        case 4:

        switch (montar(argv[3], argv[2])){
            case 0:
            std::cout << "Programa montado com sucesso!" << std::endl;
            break;

            default:
            std::cout << "Não foi possível montar o programa. Tente novamente." << std::endl;
            break;
        }

        break;

        default:
        std::cout << "Execute o montador especificando o modo de realocação e o nome do programa a ser montado.\n Exemplo: ./montador -r 0 programa.asm\n\n" << std::endl;
        break;
    }

    return 0;
}

int montar(std::string nomeArquivo, std::string infoRealocacao){

    if(infoRealocacao != "0" && infoRealocacao != "1")
        return 1;

    std::ifstream entrada;
    std::string nomeSaida = nomeArquivo, mapaDeBits = "", listaEnderecos = "";
    std::string lineEntrada, lineAux, label, labelAux, operation, lineSaida, saidaAux = "T: ";
    std::vector<std::string> entradaAux;
    int endereco = 0, lineAuxSize, labelSize, tamanho = 0, opcode = 0;
    int contadorLinha = 1, contadorPosicao = 0, valor = 0, jump = 0;
    int jump_int[2];
    bool labelAnterior = false, flag = true, antesTexto;
    operandos_t operands;

    eraseAllSubStr(nomeSaida, ".asm");

    std::ofstream saida (nomeSaida + ".obj");
    saida << "H: " + nomeSaida;

    Operacoes *tabelaOperacoes = new Operacoes();
    Controlador *controle = new Controlador();

    entrada.open(nomeArquivo);

    if (!entrada.is_open() || !saida.is_open())
        return 1;
    
    for (int i = 0; std::getline(entrada, lineEntrada);){

        lineEntrada = stringToUpper(lineEntrada);
        lineEntrada = deleteComment(lineEntrada);

        if(lineEntrada == "SECTION DATA"){

            if(flag)
                antesTexto = true;
            else
                antesTexto = false;

            flag = true;

        } else if(lineEntrada == "SECTION TEXT")
        flag = false;

        if(antesTexto && flag)
            contadorLinha++;
        
        if (flag)
            entradaAux.push_back(lineEntrada);
        else{

            entradaAux.emplace(entradaAux.begin()+i, lineEntrada);
            i++;
        }
    }

    for(std::vector<std::string>::iterator linha = entradaAux.begin(); linha != entradaAux.end(); linha++, contadorLinha++){

        lineAux = removeWhitespace(*linha); //lineAux é *linha sem whitespace para auxiliar a encontrar as linhas em que só há rótulos.
        label = findLabel(contadorLinha, *linha);
        lineAuxSize = lineAux.size();
        labelSize = label.size();

        if(labelAnterior)
            label = labelAux;

        if(lineAuxSize != labelSize + 1){       //se a linha não for apenas um rótulo

            operation = findOperation(*linha, labelSize);
            operands = findOperands(contadorLinha, *linha, operation);

            if(operation == "SECTION")
                continue;

            if(label != ""){

                if(operation == "CONST"){

                    if(operands.operandos[0][0] == 0){

                        geraMsgErro(contadorLinha, "sintatico", "A diretiva CONST exige um operando");
                        continue;
                    }

                    saidaAux += operands.operandos[0];
                    saidaAux += ' ';
                    saidaAux = controle->insereSimboloRotulo(label, contadorPosicao, contadorLinha, saidaAux);
                    mapaDeBits += "0";
                    contadorPosicao++; 

                    continue;

                } else if(operation == "SPACE"){    

                    jump = ((operands.operandos[0][0] == 0) ? 0 : std::stoi(operands.operandos[0])) + 1;
                    std::string temp = "0 ";
                    temp = temp*jump;
                    saidaAux += temp;
                    saidaAux = controle->insereSimboloRotulo(label, contadorPosicao, contadorLinha, saidaAux);

                    for (int i = 0; i < jump; i++)
                        mapaDeBits += "0";

                    contadorPosicao += jump;

                    continue;

                } else
                    saidaAux = controle->insereSimboloRotulo(label, contadorPosicao, contadorLinha, saidaAux);
            }

            opcode = tabelaOperacoes->getOpcode(operation);

            if(opcode == 15){

                geraMsgErro(contadorLinha, "léxico", "Operação não encontrada.");
                continue;

            } else if(opcode == 14){

                contadorPosicao += tabelaOperacoes->validaOperandos(opcode, operands, contadorLinha);
                mapaDeBits += "0";

            } else if(opcode == 9){

                contadorPosicao += tabelaOperacoes->validaOperandos(opcode, operands, contadorLinha);
                mapaDeBits += "011";
                listaEnderecos += std::to_string(contadorPosicao - 2) + std::to_string(contadorPosicao - 1) + " ";

            } else {

                contadorPosicao += tabelaOperacoes->validaOperandos(opcode, operands, contadorLinha);
                mapaDeBits += "01";
                listaEnderecos += std::to_string(contadorPosicao - 1) + " ";
            }

            jump_int[0] = getJump(operands.operandos[0]);
            jump_int[1] = getJump(operands.operandos[1]);

            if(strlen(operands.operandos[0]) != 0){
                if(strlen(operands.operandos[1]) != 0){
                    controle->insereSimboloOperando(operands.operandos[0], contadorPosicao-2, contadorLinha, jump_int[0]);
                    controle->insereSimboloOperando(operands.operandos[1], contadorPosicao-1, contadorLinha, jump_int[1]);

                } else
                controle->insereSimboloOperando(operands.operandos[0], contadorPosicao-1, contadorLinha, jump_int[0]);
            }
            
        } else {

            labelAux = label;
            labelAnterior = true;
        }

        lineSaida = controle->geraInstrucao(opcode, operands, jump_int);
        saidaAux += lineSaida;
    }

    controle->finalizaMontagem(contadorLinha);

    if(msgErro.begin() != msgErro.end())
        return 1;
    saida << "\nH: " + contadorPosicao;

    if(infoRealocacao == "0")
        saida << "\nH: " + mapaDeBits + "\n";
    else
        saida << "\nH: " + listaEnderecos + "\n";

    saida << saidaAux;
    entrada.close();
    saida.close();
    return 0;
}
