#include <stdio.h>
#include <string.h>
#include "queries.h"
#include "drivers_structure.h"
#include "rides_structure.h"
#include "users_structure.h"
#include <dirent.h>
#include <unistd.h>


void querie1(char *line,char *file){

    drivers *d;
    users *u;   
    chdir("trabalho-pratico");
    chdir("Resultados/");
    
    FILE * NewFile;
    NewFile = fopen(file, "w");
    

    if(line[0]=='0') {        
        d = procura_hash_drivers(line);
        if(d!=NULL && (strcmp((converte(d->account_status)),"active")==0)){
            drivers_q1 *d1 = procura_rides_drivers(line);
            d1->avaliacao_media = d1->avaliacao_media/d1->numero_viagens;
            fprintf(NewFile,"%s;%c;%d;%.3f;%d;%.3f\n",d->name,d->gender,calculaIdade(d->birth_day),d1->avaliacao_media,d1->numero_viagens,d1->total_auferido);
        }
    }
    else {                      
        u = procura_hash_users(line);
        if(u!=NULL && (strcmp((converte(u->account_status)),"active")==0)){
            users_q1 *u1 = procura_rides_users(line);
            u1->avaliacao_media = u1->avaliacao_media/u1->numero_viagens;
            fprintf(NewFile,"%s;%c;%d;%.3f;%d;%.3f\n",u->name,u->gender,calculaIdade(u->birth_day),u1->avaliacao_media,u1->numero_viagens,u1->total_gasto);
        }
    }     
    fclose(NewFile);
    chdir("trabalho-pratico");
}

void querie2(char *line,char *file){}

void querie3(char *line,char *file){}

void read_exe_queries(const char *file){

    FILE* File1;
    File1 = fopen(file,"r");
    char line[150],line2[150];
    char querie;
    int i,x=0;
    
    while(fgets(line, 150, File1)!=NULL){
        x++;
        char buffer [50];
        snprintf(buffer, sizeof (buffer), "command%d_output.txt",x);

        querie = line[0];
    
        for(i=2;line[i]!='\0';i++){
            line2[i-2] = line[i];
        }

        line2[i-3] = '\0';

        switch (querie)
        {
        case '1':
           querie1(line2,buffer);
           break;

        case '2':
           querie2(line2,buffer);
           break;

        case '3':
           querie3(line2,buffer);
           break;

        default:
           break;
        }
  
    }
    fclose(File1);

}