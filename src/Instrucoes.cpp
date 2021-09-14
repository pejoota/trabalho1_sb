#include "../header/Func.h"
#include "../header/Instrucoes.h"

#define NAO_EXISTE 0
#define EXISTE_E_DEFINIDO 1
#define EXISTE_N_DEFINIDO 2
#define OPCODE_N_ENCONTRADO 15

Controlador::Controlador(){ /* ctor */ }

Controlador::~Controlador(){ /* dtor  */ }

std::string Controlador::insereSimboloRotulo(std::string label, int endereco, int contadorLinha, std::string saidaAux){
	tabelaSimbolos_t novoSimbolo;

	switch (simboloExiste(label)){

		case NAO_EXISTE:
			novoSimbolo.label = label;
			novoSimbolo.valor = std::to_string(endereco);
			novoSimbolo.definido = true;
			novoSimbolo.pendencias.push_back(-1);

			tabelaSimbolos.push_back(novoSimbolo);
			break;

		case EXISTE_E_DEFINIDO:

			geraMsgErro(contadorLinha, "semantico", "Declaracao de rotulos repetidos");
			break;

		case EXISTE_N_DEFINIDO:

			for(std::list<tabelaSimbolos_t>::iterator it = tabelaSimbolos.begin(); it != tabelaSimbolos.end(); it++){
				if(it->label == label){

					saidaAux = resolvePendencias(endereco, it->pendencias, saidaAux);
					it->valor = std::to_string(endereco);
					it->definido = true;
					it->pendencias.erase(it->pendencias.begin(), it->pendencias.end());
					it->pendencias.push_back(-1);

					break;
				}
			}

			break;
	}
	
	return saidaAux;
}

void Controlador::insereSimboloOperando(std::string label, int valor, int contadorLinha, int jump){
	tabelaSimbolos_t novoSimbolo;
	std::string jump_str = std::to_string(jump);

	eraseAllSubStr(label, jump_str);
	eraseAllSubStr(label, "+");

	switch (simboloExiste(label)){

		case NAO_EXISTE:
			novoSimbolo.label = label;
			novoSimbolo.valor = "0";
			novoSimbolo.definido = false;
			novoSimbolo.pendencias.push_back(valor);
			novoSimbolo.endereco = contadorLinha;

			tabelaSimbolos.push_back(novoSimbolo);
			break;

		case EXISTE_E_DEFINIDO:

			for(std::list<tabelaSimbolos_t>::iterator it = tabelaSimbolos.begin(); it != tabelaSimbolos.end(); it++){
				if(it->label == label){

					valor = std::stoi(it->valor);
					it->endereco = contadorLinha;
				}
			}

			break;

		case EXISTE_N_DEFINIDO:

			for(std::list<tabelaSimbolos_t>::iterator it = tabelaSimbolos.begin(); it != tabelaSimbolos.end(); it++){
				if(it->label == label){

					it->pendencias.push_back(valor);
					it->endereco = contadorLinha;
					break;
				}
			}

			break;
	}
}

std::string Controlador::resolvePendencias(int endereco, std::list<int> pendencias, std::string saidaAux){
	int size = saidaAux.size();
	int count = 0;
	int aux = 0, jump;
	std::vector<int> pos;
	bool flag = true;
	
	for(std::list<int>::iterator it = pendencias.begin(); it != pendencias.end(); it++){

		for(int i = 0; i < size; i++){
	
			if(saidaAux[i] == ' '){

				count++;
				pos.push_back(i);
			}	

			if (count == *it + 1){

				aux = pos.size();
				jump = getJump(saidaAux.substr(pos[(aux-2)]+1, pos[(aux-1)]-pos[(aux-2)]-1));
				saidaAux.replace(pos[(aux-2)]+1, pos[(aux-1)]-pos[(aux-2)]-1, std::to_string(endereco + jump));

				break;
			}
		}

		pos.erase(pos.begin(), pos.end());
		count = 0;
	}

	return saidaAux;
}

int Controlador::simboloExiste(std::string label){

	for(std::list<tabelaSimbolos_t>::iterator it = tabelaSimbolos.begin(); it != tabelaSimbolos.end(); it++){

		if(it->label == label){
			if(it->definido)
				return EXISTE_E_DEFINIDO;
			else
				return EXISTE_N_DEFINIDO;
		}
	}

	return NAO_EXISTE;
}

std::string Controlador::geraInstrucao(int opcode, operandos_t operandos, int* jump_int){

	std::string operando1 = "";
	std::string operando2 = "";

	for(std::list<tabelaSimbolos_t>::iterator it = tabelaSimbolos.begin(); it != tabelaSimbolos.end(); it++){

		if(it->label == operandos.operandos[0]){
			if(it->definido)
				operando1 = std::to_string(std::stoi(it->valor) + jump_int[0]) + ' ' ;
			else
				operando1 = "XX+" + std::to_string(jump_int[0]) + ' ';
			
		} else if (it->label == operandos.operandos[1]){
			if(it->definido)
				operando2 = std::to_string(std::stoi(it->valor) + jump_int[1]) + ' ';
			else
				operando2 = "XX+" + std::to_string(jump_int[1]) + ' '; 
		}
	}	

	return std::to_string(opcode) + ' ' + operando1 + operando2;
}

void Controlador::finalizaMontagem(int contadorLinha){

	for (std::list<tabelaSimbolos_t>::iterator it = tabelaSimbolos.begin(); it != tabelaSimbolos.end(); it++){
		if (*(it->pendencias.begin()) != -1)
			geraMsgErro(it->endereco, "semântico", "Utilização de operando indefinido.");
	}

	printErros();
}

void Controlador::printTabela(){

	for(std::list<tabelaSimbolos_t>::iterator it = tabelaSimbolos.begin(); it != tabelaSimbolos.end(); it++){

		std::cout << it->label << " " << it->valor << " " << it->definido << std::endl;
	}

	std::cout << std::endl;
}

void Controlador::printPendencias(){

	for(std::list<tabelaSimbolos_t>::iterator it = tabelaSimbolos.begin(); it != tabelaSimbolos.end(); it++){

		std::cout << it->label << ":" << std::endl;

		for (std::list<int>::iterator it2 = it->pendencias.begin(); it2 != it->pendencias.end(); it2++)
			std::cout << *it2 << std::endl;
	}

	std::cout << std::endl;
}

Operacoes::Operacoes(){ /* ctor */ }

Operacoes::~Operacoes(){ /* dtor  */ }

int Operacoes::getOpcode(std::string operacao){

	for(int i = 0; i < NUM_OPERACOES; i++){
		
		if (this->operacao[i].mnemonico == operacao)
			return this->operacao[i].opcode;
	}

	return OPCODE_N_ENCONTRADO;
}

int Operacoes::validaOperandos(int opcode, operandos_t operandos, int contadorLinha){

	int tamanho = operacao[opcode - 1].tamanho;
	int numOperandos = (operandos.operandos[0][0] == 0 ? 0 : 1) + (operandos.operandos[1][0] == 0 ? 0 : 1);

	if (tamanho != numOperandos + 1 || operandos.flag){

		geraMsgErro(contadorLinha, "semantico", "Quantidade incorreta de operandos");
		return 0;
	}

	return tamanho;
}

Enderecamento::Enderecamento(){ /* ctor */ }

Enderecamento::~Enderecamento(){ /* dtor  */ }

void Enderecamento::preencheEnderecos(std::string programa){

	std::vector<int> pos;
    int aux;
    bool flag;

    for(int i = 0; programa[i] != '\0'; i++){

        if(programa[i] == ' '){

            pos.push_back(i);
            flag = true;

        } else if (programa[i+1] == '\0'){

            pos.push_back(i+1);
            flag = true;
        }

        aux = pos.size();

        if(aux == 1 && flag){

            endereco.push_back(std::stoi(programa.substr(0, pos[0])));
            flag = false;
        }
        else if(aux > 1 && flag){

            endereco.push_back(std::stoi(programa.substr(pos[(aux-2)]+1, pos[(aux-1)]-pos[(aux-2)]-1)));
            flag = false;
        }
    }
}

void Enderecamento::percorreEExecuta(){

	std::vector<int>::iterator it = endereco.begin();
	std::vector<int>::iterator aux;

	while (it != endereco.end()){

		std::cout << "PC: " << (it - endereco.begin()) <<std::endl;

		switch(*it){

			case 1:
				memoriaAtiva = memoriaAtiva + buscaOperando(*(it+1));
				std::cout << "ADD" << std::endl;
				it += 2;

				break;

			case 2:

				memoriaAtiva = memoriaAtiva - buscaOperando(*(it+1));
				std::cout << "SUB" << std::endl;
				it += 2;

				break;

			case 3:

				memoriaAtiva = memoriaAtiva * buscaOperando(*(it+1));
				std::cout << "MUL" << std::endl;
				it += 2;

				break;

			case 4:

				memoriaAtiva = memoriaAtiva / buscaOperando(*(it+1));
				std::cout << "DIV" << std::endl;
				it += 2;
	
				break;

			case 5:

				it = endereco.begin() + *(it+1);
				std::cout << "JMP" << std::endl;

				break;

			case 6:

				if(memoriaAtiva < 0)
					it = endereco.begin() + *(it+1);
				else
					it += 2;

				std::cout << "JMPN" << std::endl;

				break;

			case 7:

				if(memoriaAtiva > 0)
					it = endereco.begin() + *(it+1);
				else
					it += 2;

				std::cout << "JMPP" << std::endl;

				break;

			case 8:

				if(memoriaAtiva == 0)
					it = endereco.begin() + *(it+1);
				else
					it += 2;

				std::cout << "JMPZ" << std::endl;

				break;

			case 9:

				aux = endereco.begin() + *(it+2);
				*aux = *(endereco.begin() + *(it+1));
				std::cout << "COPY" << std::endl;
				it += 3;

				break;

			case 10:

				memoriaAtiva = buscaOperando(*(it+1));
				std::cout << "LOAD" << std::endl;
				it += 2;

				break;

			case 11:

				aux = endereco.begin() + *(it+1);
				*aux = memoriaAtiva;
				std::cout << "STORE" << std::endl;
				it += 2;

				break;

			case 12:

				aux = endereco.begin() + *(it+1);
				std::cout << "INPUT ";
				std::cin >> *aux;
				it += 2;
				
				break;

			case 13:

				std::cout << "OUTPUT " << buscaOperando(*(it+1)) << std::endl;
				std::cin;
				it += 2;
				break;

			case 14:
				std::cout << "STOP" << std::endl;
				std::cout << "~~~~~~~~~~~~~~~" << std::endl;
			
				return;
		}

		std::cout << "ACC: " << memoriaAtiva <<std::endl;
		std::cout << "~~~~~~~~~~~~~~~" << std::endl;
	}
}

int Enderecamento::buscaOperando(int valor){
	return endereco[valor];
}
