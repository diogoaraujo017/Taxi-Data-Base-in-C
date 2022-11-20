#ifndef PARSING_RIDES_H
#define PARSING_RIDES_H

#define MAX_INFO 100
#define N_LINHASU 100000  
#define N_LINHASD 10000  

typedef struct{
    char *username;
    char *date;       //Data da viagem mais recente;
    double avaliacao_media;
    int numero_viagens;
    double total_gasto;  
}users_q1;

typedef struct drivers_q1{
    char *id;
    char *date;       //Data da viagem mais recente;
    double avaliacao_media;
    int numero_viagens;
    double total_auferido;      
}drivers_q1;

void read_store_rides();
void analisa_linha_rides(char *line);

#endif