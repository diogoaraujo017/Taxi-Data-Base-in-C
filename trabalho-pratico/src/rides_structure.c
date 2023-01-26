#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rides_structure.h"
#include "users_structure.h"
#include "drivers_structure.h"
#include "parsing.h"
#include <stdint.h>
#include <stdbool.h>
#include "func_auxiliares.h"


// Hash Table das rides com a key das rides.
rides *hash_rides[N_LINHAS];

bool insert_hash_rides(char *id,char *dt,char *dr,char *user,char *c,int dist,double su,double sd,double tip){
    rides *r = malloc(sizeof(rides));
    r->id=id;
    r->date=dt;
    r->driver=dr;
    r->user=user;
    r->city=c;
    r->distance=dist;
    r->score_user=su;
    r->score_driver=sd;
    r->tip=tip;   
    
    hash_rides[atoi(id)-1] = r;
    return true;
}

// Procura um determinado username na hash table city. Esta função é bastante rápida a executar, mesmo
// existindo muitas linhas de hash, devido à existência de keys que estão associadas, uma a cada
// input colocado na hash table. Se encontrar a função dará return à linha da hash correspondente
// em forma de struct, caso contrário dará return a NULL.
rides *procura_rides(int id){
    return hash_rides[id];                                                                      
}


void free_hash_rides(){
    int i;
    for(i=0;i<N_LINHAS;i++){
        if(hash_rides[i]!=NULL){
            free(hash_rides[i]->id);
            free(hash_rides[i]->date);
            free(hash_rides[i]->driver);
            free(hash_rides[i]->user);
            free(hash_rides[i]->city);
        }
            free(hash_rides[i]);
    }
}


//// DRIVERS //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

rides_driver *hash_rides_drivers[N_LINHAS_DRIVERS];

bool insert_hash_rides_drivers(char *data,char *condutor,double avaliacao_media){

    int linha_hash = atoi (condutor)-1;

    if (hash_rides_drivers[linha_hash] == NULL){

        drivers *d =procura_hash_drivers(condutor);

        if(d==NULL || !strcmp(d->account_status,"inactive")) return true;

        rides_driver *rd = malloc(sizeof(rides_driver));
        rd->date = data;
        rd->driver = condutor;
        rd->score_driver = avaliacao_media;
        rd->numero_viagens = 1;
        rd->score_total=avaliacao_media;
        rd->nome = d->name;

        hash_rides_drivers[linha_hash] = rd;
        return true;
    }

    
            
    if((calculaData(data,hash_rides_drivers[linha_hash]->date)==0)) hash_rides_drivers[linha_hash]->date = data;
           
    hash_rides_drivers[linha_hash]->score_total += avaliacao_media;
    hash_rides_drivers[linha_hash]->numero_viagens ++;
    hash_rides_drivers[linha_hash]->score_driver = hash_rides_drivers[linha_hash]->score_total/hash_rides_drivers[linha_hash]->numero_viagens;

    return true;
        
}

rides_driver *procura_rides_driver(){
     double aval=0;
     char* data="00/00/0000";
     register int i,aux=0,id=0;
     for(i=0;i<N_LINHAS_DRIVERS;i++){
         if(hash_rides_drivers[i]!=NULL){
             if((hash_rides_drivers[i]->score_driver>aval) || (hash_rides_drivers[i]->score_driver==aval && calculaData(hash_rides_drivers[i]->date,data)==0) || (hash_rides_drivers[i]->score_driver==aval && calculaData(hash_rides_drivers[i]->date,data)==(-1) && (atoi(hash_rides_drivers[i]->driver)<id))){
                 data = hash_rides_drivers[i]->date;  
                 aval = hash_rides_drivers[i]->score_driver;
                 aux = i;
                 id = atoi(hash_rides_drivers[i]->driver);
             }
         }
     }
     if(hash_rides_drivers[aux]!=NULL) hash_rides_drivers[aux]->score_driver-=10;

     return hash_rides_drivers[aux];
 }

void restore_hash_rides_drivers(){

    register int i;
    for(i=0;i<N_LINHAS_DRIVERS;i++)
        if(hash_rides_drivers[i]!=NULL && hash_rides_drivers[i]->score_driver<0) hash_rides_drivers[i]->score_driver+=10;
}



void free_hash_rides_drivers(){
    int i;
    for(i=0;i<N_LINHAS_DRIVERS;i++){
        free(hash_rides_drivers[i]);
    }
}


//// USERS ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

rides_user *hash_rides_users[N_LINHAS_USERS];


bool insert_hash_rides_users(int distance,char *date,char *user){
    register int i, linha_hash=0;
    int aux = hash_users(user);
    users *u = procura_hash_users(user);

    if(u==NULL || strcmp(u->account_status,"inactive")==0) return true;

    for (i=0;i < N_LINHAS_USERS;i++){

        linha_hash = (i + aux) % N_LINHAS_USERS;

        if (hash_rides_users[linha_hash] == NULL){
            rides_user *ru = malloc(sizeof(rides_user));
            ru->distancia = distance;
            ru->data = date;
            ru->username = user;
            ru->nome = u->name;

            hash_rides_users[linha_hash] = ru;
            return true;
        }
        
        if(strcmp(hash_rides_users[linha_hash]->username,user)==0) {

            if((calculaData(date,hash_rides_users[linha_hash]->data)==0)) hash_rides_users[linha_hash]->data = date;

            hash_rides_users[linha_hash]->distancia += distance;

            return true;
        }
    }
    
    return false;
}

rides_user *procura_rides_users(){
    int dist=0;
    char* data="00/00/0000";
    register int i,aux=0;
    char *user="zzzz";
    for(i=0;i<N_LINHAS_USERS;i++){
        if(hash_rides_users[i]!=NULL && ((hash_rides_users[i]->distancia>dist) || (hash_rides_users[i]->distancia==dist && calculaData(hash_rides_users[i]->data,data)==0) || (hash_rides_users[i]->distancia==dist && calculaData(hash_rides_users[i]->data,data)==(-1) && strcmp(hash_rides_users[i]->username,user)<0))){
            dist=hash_rides_users[i]->distancia;
            data= hash_rides_users[i]->data;  
            user= hash_rides_users[i]->username;
            aux = i;
        }
    }
    if(hash_rides_users[aux]!=NULL) hash_rides_users[aux]->distancia-=1000;

    return hash_rides_users[aux];
}

void restore_hash_rides_users(){

    register int i;
    for(i=0;i<N_LINHAS_USERS;i++)
        if(hash_rides_users[i]!=NULL && hash_rides_users[i]->distancia<0) hash_rides_users[i]->distancia+=1000;
}


void free_hash_rides_users(){
    int i;
    for(i=0;i<N_LINHAS_USERS;i++){
        free(hash_rides_users[i]);
    }
}


//// DRIVERS CITY  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

rides_driver_city *hash_rides_drivers_city[N_LINHAS_DRIVERS];

bool insert_hash_rides_drivers_city(char* city){
    register int i;
    int id_driver;
    drivers *d;
    for(i=0;i<N_LINHAS;i++){
        if(hash_rides[i]!=NULL){

            d = procura_hash_drivers(hash_rides[i]->driver);

            if(d!=NULL){

            
                id_driver= atoi(d->id)-1;

                if(strcmp(d->account_status,"active")==0 && strcmp(hash_rides[i]->city,city)==0){

                    if(hash_rides_drivers_city[id_driver]==NULL){

                        rides_driver_city *rdc = malloc(sizeof(rides_driver_city));
                        rdc->numero_viagens=1;
                        rdc->id=d->id;
                        rdc->avaliacao_media=hash_rides[i]->score_driver;
                        rdc->nome=d->name;
                        rdc->avaliacao_total=hash_rides[i]->score_driver;
                        hash_rides_drivers_city[id_driver]=rdc;
                    }

                    else{
                        hash_rides_drivers_city[id_driver]->numero_viagens++;
                        hash_rides_drivers_city[id_driver]->avaliacao_total+=hash_rides[i]->score_driver;
                        hash_rides_drivers_city[id_driver]->avaliacao_media=hash_rides_drivers_city[id_driver]->avaliacao_total/hash_rides_drivers_city[id_driver]->numero_viagens;
                    }
                }
            }
        }
    }
    return true;
}

rides_driver_city *procura_rides_driver_city(){
    register int i,aux=0;
    double score_driver=0;
    int condutor=0;
    for(i=0;i<N_LINHAS_DRIVERS;i++){
        if(hash_rides_drivers_city[i]!=NULL && (hash_rides_drivers_city[i]->avaliacao_media>score_driver || (hash_rides_drivers_city[i]->avaliacao_media==score_driver && atoi(hash_rides_drivers_city[i]->id)>condutor))){
            condutor=atoi(hash_rides_drivers_city[i]->id);
            score_driver = hash_rides_drivers_city[i]->avaliacao_media;
            aux=i;
        }
    }

    if(hash_rides_drivers_city[aux]!=NULL) hash_rides_drivers_city[aux]->avaliacao_media-=10;

    return hash_rides_drivers_city[aux];
}



void free_hash_rides_driver_city(){
    register int i;
    for(i=0;i<N_LINHAS_DRIVERS;i++){
        if(hash_rides_drivers_city[i]!=NULL){
            free(hash_rides_drivers_city[i]);
            hash_rides_drivers_city[i]=NULL;
        }
    }
}


//// RIDES_GENDER /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

rides_gender *hash_rides_gender[N_LINHAS_GENDER];

bool insert_hash_rides_gender(char genero,int idade){
    register int i,j;
    drivers *d;
    users *u;
    for(i=0;i<N_LINHAS;i++){
        if(hash_rides[i]!=NULL){

            d = procura_hash_drivers(hash_rides[i]->driver);
            u = procura_hash_users(hash_rides[i]->user);
            if(d!=NULL && u!=NULL){
                if(strcmp(u->account_status,"active")==0 && strcmp(d->account_status,"active")==0 && d->gender[0]==genero && u->gender[0]==genero && calculaIdade(u->account_creation)>=idade && calculaIdade(d->account_creation)>=idade){

                    for(j=0;j<N_LINHAS_GENDER;j++){
                        if(hash_rides_gender[j]==NULL) break;
                    }
            

                    rides_gender *rg = malloc(sizeof(rides_gender));
                    rg->id_condutor=d->id;
                    rg->nome_condutor=d->name;
                    rg->username_utilizador=u->username;
                    rg->nome_utilizador=u->name;
                    rg->data_driver=d->account_creation;
                    rg->data_user=u->account_creation;
                    rg->isValid=1;
                    rg->id_viagem=hash_rides[i]->id;
                    hash_rides_gender[j]=rg;
                }
            }
        }
    }
    return true;
}

rides_gender *procura_rides_gender(){
    register int i,aux;
    char* data_atual_user="00/00/2222";
    char* data_atual_driver="00/00/2222";
    int data_registo_user,data_registo_driver;
    int end=0,id_atual=99999999;
    
    for(i=0;i<N_LINHAS_GENDER;i++){

        if(hash_rides_gender[i]!=NULL){

            data_registo_user=calculaData(hash_rides_gender[i]->data_user,data_atual_user);
            data_registo_driver=calculaData(hash_rides_gender[i]->data_driver,data_atual_driver);
        
            if (hash_rides_gender[i]->isValid==1 && (data_registo_driver==1 || (data_registo_driver==-1 && data_registo_user==1) || (data_registo_driver==-1 && data_registo_user==-1 && atoi(hash_rides_gender[i]->id_viagem)<id_atual))){
                end=1;
                data_atual_user=hash_rides_gender[i]->data_user;
                data_atual_driver=hash_rides_gender[i]->data_driver;
                id_atual=atoi(hash_rides_gender[i]->id_viagem);
                aux=i;
            }
        }
    }

    if(end==0) return NULL;

    hash_rides_gender[aux]->isValid=0;
    return hash_rides_gender[aux];
}


void free_hash_rides_gender(){
    int i;
    for(i=0;i<N_LINHAS_GENDER;i++){
        free(hash_rides_gender[i]);
    }
}


//// RIDES_DATE /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

rides_date *hash_rides_date[N_LINHAS_DRIVERS];

bool insert_hash_rides_date(char *date1,char *date2){
    register int i,j;
    
    for(i=0;i<N_LINHAS;i++){

        if(hash_rides[i]!=NULL && (hash_rides[i]->tip != 0.0) && calculaData(hash_rides[i]->date,date1)<=0 && calculaData(date2,hash_rides[i]->date)<=0){

         for(j=0;j<N_LINHAS_DRIVERS;j++){
                if(hash_rides_date[j]==NULL) break;
            }

                rides_date *rd = malloc(sizeof(rides_date));
                rd->distance=hash_rides[i]->distance;
                rd->date=hash_rides[i]->date;
                rd->id=hash_rides[i]->id;
                rd->city=hash_rides[i]->city;
                rd->tip=hash_rides[i]->tip;
                rd->id_viagem=hash_rides[i]->id;
                rd->isValid=1;

                hash_rides_date[j]=rd;
       }
    }
    return true;
}

rides_date *procura_rides_date(){
    register int i,aux;
    char* recent_date = "00/00/0000";
    int dist=0,end=0,id_atual=0;

    for(i=0;i<N_LINHAS_DRIVERS;i++){

        if(hash_rides_date[i]!=NULL && hash_rides_date[i]->isValid==1 && (dist<hash_rides_date[i]->distance || (dist==hash_rides_date[i]->distance && calculaData(hash_rides_date[i]->date,recent_date)<=0) || (dist==hash_rides_date[i]->distance && calculaData(hash_rides_date[i]->date,recent_date)==-1 && atoi(hash_rides_date[i]->id_viagem)>id_atual))){
                
                end=1;
                dist=hash_rides_date[i]->distance;
                recent_date=hash_rides_date[i]->date;
                id_atual=atoi(hash_rides_date[i]->id_viagem);
                aux=i;
        }
    }
 
    if(end==0) return NULL;

    hash_rides_date[aux]->isValid=0;
    return hash_rides_date[aux];
}


void free_hash_rides_date(){
    int i;
    for(i=0;i<N_LINHAS_DRIVERS;i++){
        free(hash_rides_date[i]);
    }
}




















