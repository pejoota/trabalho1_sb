#include "src/Func.cpp"
#include "src/Instrucoes.cpp"

#define FALHA_CARREGAMENTO 0
#define FALHA_ABERTURA 1
#define SUCESSO 2

int main(int argc, char const *argv[]){

	int
		resposta,
		numChunks;

	std::vector<int>
		tamanhoChunks,
		enderecoChunks;

	std::vector<std::string>
		programas;

	for (int i = 1; i < argc; ++i){

		try{
			numChunks = std::stoi(argv[i]);

			for(int j = 1; j <= numChunks; j++){

				tamanhoChunks.push_back(std::stoi(argv[i + j]));
				enderecoChunks.push_back(std::stoi(argv[i + j + numChunks]));
			}

			break;
		}

		catch(const std::invalid_argument & err){
			programas.push_back(argv[i]);
		}
	}

	resposta = carregar(tamanhoChunks, enderecoChunks, programas);

	return 0;
}

int carregar(std::vector<int> tamanhoChunks, std::vector<int> enderecoChunks, std::vector<std::string> programas){

	int
		auxiliar = 0,
		numProgramas = programas.size(),
		tamanhoDisponivel = 0,
		tamanhoNecessario = 0,
		chunkCounter = 0,
		sizeCounter = 0;

	std::string
		linha[numProgramas],
		nomePrograma[numProgramas];

	std::vector<int>
		tamanhoProgramas,
		enderecoAux,
		enderecosCarregados(numProgramas,0);
	
	std::vector<std::vector<int>>
		text,
		enderecos;

	std::vector<std::list<int>>
		realocacao;

	std::ifstream
		entrada[numProgramas];

	std::ofstream
		saida[numProgramas];

	bool
		programaCarregado = false;


	for(int i = 0; i < numProgramas; i++){

		entrada[i].open(programas[i]);

		eraseAllSubStr(programas[i], "obj");
		saida[i].open(programas[i] + "saida");

		if(!entrada[i].is_open() || !saida[i].is_open())
			return FALHA_ABERTURA;
		
		getline(entrada[i], linha[i]);
		nomePrograma[i] = getNome(linha[i]);

		getline(entrada[i], linha[i]);
		tamanhoProgramas.push_back(getTamanho(linha[i]));

		getline(entrada[i], linha[i]);
		preencheEnderecos(linha[i], realocacao);

		getline(entrada[i], linha[i]);
		preencheEnderecos(linha[i], text);
	}

	for(int i = 0; i < tamanhoChunks.size(); i++)
		tamanhoDisponivel += tamanhoChunks[i];

	for(int i = 0; i < tamanhoProgramas.size(); i++)
		tamanhoNecessario += tamanhoProgramas[i];

	if(tamanhoDisponivel < tamanhoNecessario){

		for (int i = 0; i < numProgramas; ++i){

			entrada[i].close();
			saida[i].close();
		}

		return FALHA_CARREGAMENTO;
	}

	for(int i = 0; i < numProgramas; i++){
		for (int chunksUsados = 1; chunkCounter < tamanhoChunks.size(); chunkCounter++, chunksUsados++){
			for(sizeCounter = 0; sizeCounter < tamanhoChunks[chunkCounter]; sizeCounter++, enderecosCarregados[i]++){					

				enderecoAux.push_back(enderecoChunks[chunkCounter] + sizeCounter + auxiliar);

				if(enderecosCarregados[i] == tamanhoProgramas[i]){

					programaCarregado = true;
					break;
				}
			}

			if(programaCarregado){

				auxiliar += sizeCounter + 1;
				enderecos.push_back(enderecoAux);
				enderecoAux.clear();
				std::cout << nomePrograma[i] << " utilizando " << chunksUsados << " CHUNKS. Endereço(s) inicial(is): ";

				for (int count = 0; count < chunksUsados; count++){
					std::cout << enderecoChunks[count] << " ";
				}

				std::cout << std::endl;

				break;
			}
		}

		programaCarregado = false;

		for (int j = 0; j < enderecos[i].size(); ++j){
		
			if(j == *(realocacao[i].begin())){

				text[i][j] = enderecoAux[text[i][j]];
				realocacao[i].pop_front();
			}
		}

		saida[i] << preencheArquivo(text[i], enderecos[i]);

		entrada[i].close();
		saida[i].close();
	}

	return SUCESSO;
}

