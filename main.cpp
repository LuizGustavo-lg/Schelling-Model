#include <iostream>
#include <ctime>
#include <iomanip>
#include <cmath>

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
    float tx_casas_vazias = 0.2;
    int total_casas;

    
};

void print_matrix(int mt[], int tamanho_mt);
void set_matrix(int mt[], SegregacaoConfig conf);
void set_matrix_null_map(int mt_first[], int mt_null_map[], int tamanho_mt);
int  verify_eight(int mt[], int indice, int tamanho_mt);
void schering_model(int mt[], int null_map[], SegregacaoConfig conf);


int main(){
    SegregacaoConfig conf;
    cout << "Informe o tamanho da matrix que vc deseja: ";
    cin >> conf.tamanho_mt;
    
    float r;
    cout << "Informe o nivel de tolerancia: ";
    cin >> r;
    conf.tolerancia = (r*8)/100;
    
    cout << "Informe a porcentagem, de casas vazias: ";
    cin >> r;
    conf.tx_casas_vazias = r/100;
    
    
    int bairro[conf.tamanho_mt*conf.tamanho_mt];
    int null_map[(int) ((conf.tamanho_mt*conf.tamanho_mt) * conf.tx_casas_vazias)];
    
    conf.total_casas = pow(conf.tamanho_mt, 2);
    conf.seed = time(0);
    srand(conf.seed);


    set_matrix(bairro, conf);
    set_matrix_null_map(bairro, null_map, conf.tamanho_mt);

    schering_model(bairro, null_map, conf);

}


void print_matrix(int mt[], SegregacaoConfig conf){
    // Imprime na tela a matriz de principal

    Paint p;

    cout << "\n\t";
    for (int i = 0; i < conf.total_casas; i++){
        cout << p.BOLD;
        switch (mt[i]){
        case 1:
            cout << p.C_MAGENTA << p.BG_MAGENTA;
            break;
        case 2:
            cout << p.C_CYAN << p.BG_CYAN;
            break;
        case 0:
            cout << p.C_GRAY << p.BG_GRAY;
            break;
        default:
            cout << p.RESET;
            break;
        }
        
        cout << setw(2) << mt[i];

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
        int i_rand = rand() % (conf.tamanho_mt*conf.tamanho_mt); 
        int aux = mt[i];

        mt[i] = mt[i_rand];
        mt[i_rand] = aux;
    } 

}


void set_matrix_null_map(int mt_first[], int mt_null_map[], int tamanho_mt){
    //Inicia o vetor null_map
    //Contem todos os indices das casas vazias da matriz principal

    int j = 0;
    for (int i = 0; i < tamanho_mt*tamanho_mt; i++) {
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


void schering_model(int mt[], int null_map[], SegregacaoConfig conf){
    //Verifica o grau de satisfação de cada elemento
    //Se o elemento se encontrar insadisfeiro ele busca aleatoriamente uma casa vazia e se muda
    //Enquanto ouver mudanca, os elementos continuam se alterando 

    int total_vazias = conf.total_casas * conf.tx_casas_vazias;
    int alterou = 0;
    int rodada = 0;

    do {
        alterou = 0;
        ++rodada;

        for (int i = 0; i < pow(conf.tamanho_mt, 2); i++) {
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

        print_matrix(mt, conf);
        cout << "Rodada: " << rodada << '\n';
        cout << "\tSatisfação geral: " << 100-(alterou*100)/conf.total_casas << "% \n";;
    } while (alterou);
}
