#ifndef INSTRUCOES_H_INCLUDED
#define INSTRUCOES_H_INCLUDED

#include <iostream>
#include <list>
#include <algorithm>
#include <vector>

#define NUM_OPERACOES 14

typedef struct tabelaOperacoes {

	std::string mnemonico;
	int opcode;
	int tamanho;

} tabelaOperacoes_t;

typedef struct tabelaSimbolos {

	std::string label;
	std::string valor;
	std::list<int> pendencias;
	bool definido;
	int endereco;

} tabelaSimbolos_t;

class Controlador {
	public:
		Controlador();
		virtual ~Controlador();

		std::string insereSimboloRotulo(std::string, int, int, std::string);
		void insereSimboloOperando(std::string, int, int, int);
		std::string geraInstrucao(int, operandos_t, int[2]);
		void printTabela();
		void printPendencias();
		void finalizaMontagem(int);

	private:
		std::list<tabelaSimbolos_t> tabelaSimbolos;
		int simboloExiste(std::string);
		std::string resolvePendencias(int, std::list<int>, std::string);
};

class Operacoes { 

	public:
		Operacoes();
		virtual ~Operacoes(); 

		int getOpcode(std::string);
		int validaOperandos(int, operandos_t, int);

	private:
		const tabelaOperacoes_t operacao[NUM_OPERACOES] = {

	   /* MNEMONICO, OPCODE, TAMANHO */

			"ADD",	   1,		2,
			"SUB",	   2,  		2,
			"MUL",	   3,  		2,
			"DIV",	   4,  		2,
			"JMP",	   5,  		2,
			"JMPN",	   6,  		2,
			"JMPP",	   7,		2,
			"JMPZ",	   8,  		2,
			"COPY",	   9,  		3,
			"LOAD",	   10, 		2,
			"STORE",   11, 		2,
			"INPUT",   12, 		2,
			"OUTPUT",  13, 		2,
			"STOP",	   14, 		1
		};
};

class Enderecamento {

	public:
		Enderecamento();
		virtual ~Enderecamento();

		void preencheEnderecos(std::string);
		void percorreEExecuta();

	private:
		std::vector<int> endereco;
		std::vector<int> memoria;
		int memoriaAtiva = 0;
		
		int buscaOperando(int);
};

#endif