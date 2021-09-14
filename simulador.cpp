#include "src/Func.cpp"
#include "src/Instrucoes.cpp"

int main (int argc, char **argv){

    char escolha;
    int ctrl = argc;

    do{
        
        switch (ctrl){
    
            case 1:
                std::cout << "Digite o nome do programa .obj a ser executado:\n>> ";
                std::cin >> argv[1];
    
            case 2:
    
                switch (executar(argv[1])){
                    case 0:
                        std::cout << "Programa executado com sucesso!" << std::endl;
                        break;
    
                    default:
                        std::cout << "Não foi possível executar o programa. Tente novamente." << std::endl;
                        break;
                }
    
                break;
    
            default:
                std::cout << "Selecione apenas um programa para ser executado de cada vez!" << std::endl;
                break;
        }

        ctrl = 1;
        std::cout << "Deseja executar outro programa? (S/n)" << std::endl;
        std::cin >> escolha;

    } while (escolha != 'n' && escolha != 'N');

    return 0;
}

int executar(std::string nomeDoArquivo){

	std::ifstream entrada;
	std::string programa;
    Enderecamento* endereco = new Enderecamento();

	entrada.open(nomeDoArquivo);

	if(!entrada.is_open())
		return 1;

	std::getline(entrada, programa);
    endereco->preencheEnderecos(programa);
    endereco->percorreEExecuta();

    entrada.close();
    return 0;
}