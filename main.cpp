#include <iostream>
#include <ctime>
#include <iomanip>
#include <cmath>
using namespace std;


struct Paint{
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
    int tamanho_mt = 10;
    unsigned seed;
    int tolerancia = 5;
    float tx_casas_vazias = 0.1;
};


void imprimir_matriz(int mt[], int tamanho_mt){
    // Imprime na tela a matriz de bairro
    // param: int mt[] matriz a ser imprimida
    // param: int tamanho_mt tamanho total de array
    Paint p;
    const string RESET_COLOR = p.C_BLACK;

    cout << p.BOLD;
    for (int i = 0; i < pow(tamanho_mt, 2); i++){
        switch (mt[i]){
        case 1:
            cout << p.C_BLUE;
            break;
        case 2:
            cout << p.C_GREEN;
            break;
        case 0:
            cout << p.C_GRAY;
            break;
        default:
            cout << RESET_COLOR;
            break;
        }
        
        cout << setw(2) << mt[i];

        if ((i+1)%tamanho_mt == 0) {
            cout << RESET_COLOR << '\n';

        };
    }
    cout << RESET_COLOR;
}


void iniciar_matriz(int mt[], SegregacaoConfig conf){
    int i;
    // Inicia a matriz bairro de modo que todos os termos sejam randomizados

    for (i = 0; i < pow(conf.tamanho_mt, 2)*(1-conf.tx_casas_vazias); i+=2){
        mt[i] = 1;
        mt[i+1] = 2;
    }

    for (i; i < pow(conf.tamanho_mt, 2); i++){
        mt[i] = 0;
    }

    for (int i = 0; i < pow(conf.tamanho_mt, 2); i++){
        int i_rand = rand() % (conf.tamanho_mt*conf.tamanho_mt); 
        int aux = mt[i];

        mt[i] = mt[i_rand];
        mt[i_rand] = aux;
    } 
}


int main(){
    SegregacaoConfig conf;
    conf.seed = time(0);
    int bairro[conf.tamanho_mt*conf.tamanho_mt];
    
    srand(conf.seed);

    iniciar_matriz(bairro, conf);
    imprimir_matriz(bairro, conf.tamanho_mt);
}