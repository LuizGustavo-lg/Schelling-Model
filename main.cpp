#include <iostream>
#include <ctime>
#include <iomanip>
#include <cmath>
#include <unistd.h>

using namespace std;

struct Paint{
    //Estrutura de cores e estilos
    const string RESET      = "\033[0m";
    const string BOLD       = "\033[1m";

    const string C_BLACK    = "\033[30m";
    const string C_RED      = "\033[31m";
    const string C_GREEN    = "\033[32m";
    const string C_YELLOW   = "\033[33m";
    const string C_BLUE     = "\033[34m";
    const string C_MAGENTA  = "\033[35m";
    const string C_CYAN     = "\033[36m";
    const string C_GRAY     = "\033[37m";
    
    const string BG_BLACK   = "\033[40m";
    const string BG_RED     = "\033[41m";
    const string BG_GREEN   = "\033[42m";
    const string BG_YELLOW  = "\033[43m";
    const string BG_BLUE    = "\033[44m";
    const string BG_MAGENTA = "\033[45m";
    const string BG_CYAN    = "\033[46m";
    const string BG_GRAY    = "\033[47m";
};


struct SegregacaoConfig{
    //Configuracão da matriz
    int tamanho_mt = 20;
    unsigned seed;
    int tolerancia = 4;
    float tx_casas_vazias = 0.1;
    int delay_print = 0;
    int total_casas;

    
};

void print_matrix(int mt[], int tamanho_mt);
void set_matrix(int mt[], SegregacaoConfig conf);
void set_matrix_null_map(int mt_first[], int mt_null_map[], SegregacaoConfig conf);
int  verify_eight(int mt[], int indice, int tamanho_mt);
void schelling_model(int mt[], int null_map[], SegregacaoConfig conf);


int main(int argc, char* argv[]){
    SegregacaoConfig conf;

    for (int i = 1; i < argc; i++){
        string op = argv[i];
        if ( op == "--size" || op == "-s" ) {
            conf.tamanho_mt = atoi(argv[i+1]);

        } else if( op == "--tolerance" || op == "-t" ){
            int t = atoi(argv[i+1]);
            conf.tolerancia = (t*8)/100;

        } else if( op == "--empty" || op == "-e" ){
            int e = atoi(argv[i+1]);
            conf.tx_casas_vazias = (float) e/100;

        } else if( op == "--delay" || op == "-d" ){
            int d = atoi(argv[i+1]);
            conf.delay_print = d*1000;
        
        } else if( op == "--help" || op == "-h"){

            cout 
            << "\nUSAGE:"
            << "\n\t SchellingModel [OPTIONS]\n"

            << "\nOPTIONS:"
            << "\n\t -s, --size\n\t\tTamanho da matriz. <numeric> default=20\n"
            << "\n\t -t, --tolerance\n\t\tNivel de tolerancia da populacao [porcent]. <numeric> default=50\n"
            << "\n\t -e, --empty\n\t\tPorcentagem de casas vazias [porcent]. <numeric> default=10\n"
            << "\n\t -d, --delay\n\t\tTempo para imprimir uma nova matriz e excluir a anterior [milisegundo]. <numeric> default=0\n"
            << "\n\t -h, --help\n\t\tExibe mensagem de ajuda.\n\n";

            exit(0);
        }
    }
    
    
    conf.total_casas = pow(conf.tamanho_mt, 2);

    int bairro[conf.total_casas];
    int null_map[(int) (conf.total_casas * conf.tx_casas_vazias)];
    
    conf.seed = time(0);
    srand(conf.seed);


    set_matrix(bairro, conf);
    set_matrix_null_map(bairro, null_map, conf);

    schelling_model(bairro, null_map, conf);

}


void print_matrix(int mt[], SegregacaoConfig conf){
    // Imprime na tela a matriz de principal

    Paint p;

    cout << "\n\t";
    for (int i = 0; i < conf.total_casas; i++){
        cout << p.BOLD;
        switch (mt[i]){
        case 1:
            cout << p.C_BLUE;
            cout << setw(4) << "●";
            break;
        case 2:
            cout << p.C_RED;
            cout << setw(4) << "●";
            break;
        case 0:
            cout << p.C_GRAY;
            cout << setw(2) << "";
            break;
        default:
            cout << p.RESET;
            break;
        }
        

        if ((i+1)%conf.tamanho_mt == 0) {
            cout << p.RESET << '\n' << '\t';

        };
    }
    cout << p.RESET;
}


void set_matrix(int mt[], SegregacaoConfig conf){
    // Inicia a matriz principal de modo que todos os termos sejam randomizados
    
    int i;
    int total_ocupadas = conf.total_casas * (1 - conf.tx_casas_vazias);

    for (i = 0; i < total_ocupadas; i+=2){
        mt[i] = 1;
        mt[i+1] = 2;
    }

    for (i; i < conf.total_casas; i++){
        mt[i] = 0;
    }

    for (int i = 0; i < conf.total_casas; i++){
        int i_rand = rand() % (conf.total_casas); 
        int aux = mt[i];

        mt[i] = mt[i_rand];
        mt[i_rand] = aux;
    } 

}


void set_matrix_null_map(int mt_first[], int mt_null_map[], SegregacaoConfig conf){
    //Inicia o vetor null_map
    //Contem todos os indices das casas vazias da matriz principal

    int j = 0;
    for (int i = 0; i < conf.total_casas; i++) {
        if (mt_first[i] == 0){
            mt_null_map[j] = i;
            j++;
        }
    }
}


int verify_eight(int mt[], int indice, int tamanho_mt) {
    //Verifica os oito lados de um elemento de uma matriz unidimencial
    //Retorna a quantidade de elementos diferentes do indice especificado

    if (mt[indice] == 0) return 0;
    
    int pts = 0;

    int row = indice / tamanho_mt;
    int col = indice % tamanho_mt;

    int dRow[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dCol[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < 8; i++) {
        int newRow = row + dRow[i];
        int newCol = col + dCol[i];
        int newIndex = newRow * tamanho_mt + newCol;

        if (newRow >= 0 && newRow < tamanho_mt && newCol >= 0 && newCol < tamanho_mt) {
            if (mt[indice] != mt[newIndex] && mt[newIndex] != 0) {
                ++pts;
            }
        }
    }

    return pts;
}


void schelling_model(int mt[], int null_map[], SegregacaoConfig conf){
    //Verifica o grau de satisfação de cada elemento
    //Se o elemento se encontrar insadisfeiro ele busca aleatoriamente uma casa vazia e se muda
    //Enquanto ouver mudanca, os elementos continuam se alterando 

    int total_vazias = conf.total_casas * conf.tx_casas_vazias;
    int alterou = 0;
    int rodada = 0;

    do {
        alterou = 0;
        ++rodada;

        for (int i = 0; i < conf.total_casas; i++) {
            int pts = verify_eight(mt, i, conf.tamanho_mt);
            
            if (pts > conf.tolerancia) {
                int indice_null_map = rand() % total_vazias;
                int null_pos = null_map[indice_null_map];

                mt[null_pos] = mt[i];
                mt[i] = 0;
                null_map[indice_null_map] = i;
                ++alterou;

            }
        }

        if (conf.delay_print){
            usleep(conf.delay_print);
            system("clear");
        }
        print_matrix(mt, conf);
        cout << "Rodada: " << rodada << '\n';
        cout << "\tSatisfação geral: " << 100-(alterou*100)/conf.total_casas << "% \n";

    } while (alterou);

    cout << "\nTotal de casas: " << conf.total_casas << " [" << conf.tamanho_mt << "/" << conf.tamanho_mt << "]"
    << "\nTolerancia: " << conf.tolerancia << "/8" 
    << "\nTaxa de casas vazias: " << conf.tx_casas_vazias*100 << "%\n";
}
