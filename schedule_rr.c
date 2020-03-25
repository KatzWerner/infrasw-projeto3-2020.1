#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QT 10
#define SIZE    100

void schedule (int tamanho,int lprior[],int lburst[],char* lname[]){

    int i;
    int aux[tamanho],flag[tamanho],rt[tamanho];
    for (i = 0; i < tamanho; i++){
        aux[i] = lburst[i];
        rt[i] = -1;
        flag[i] = 0;
    }
    int turnaround,waiting,response; 
    float med_turnaround = 0,med_waiting = 0,med_response = 0;
    FILE *fp = fopen("rr2.txt","w");
    fprintf(fp,"%s\n\n","EXECUTION BY RR");

    int cont = 0,time = 0,falta = tamanho;
    while(falta != 0){
        if (aux[cont] == 0  && flag[cont] == 1){
            cont++;
            continue;
        }
        else if (aux[cont] <= QT && aux[cont] > 0){
            if (rt[cont] == -1){
                rt[cont] = time;
            }
            time += aux[cont];
            fprintf(fp,"[%s] %s %d %s\n",lname[cont],"for",aux[cont],"units");
            flag[cont] = 1;
            aux[cont] = 0;
            
        }
        else if (aux[cont] > QT){
            if (rt[cont] == -1){
                 rt[cont] = time;
            }
            fprintf(fp,"[%s] %s %d %s\n",lname[cont],"for",QT,"units");
            aux[cont] -= QT;
            time += QT;
        }

        if(aux[cont]== 0 && flag[cont]== 1){
            falta--;
            /*fprintf(fp,"\n[%s]\n",lname[cont]);
            fprintf(fp,"%s %s %d\n","Turnaround","time:",time);
            fprintf(fp,"%s %s %d\n","Waiting","time:",time - lburst[cont]);
            fprintf(fp,"%s %s %d\n","Response","time:",rt[cont]);*/
            med_waiting += time - lburst[cont]; 
            med_turnaround += time; 
            flag[cont] = 1; 
        }

        if(cont == tamanho-1){
            cont = 0; 
        }
        else{ 
            cont++;
        } 
    }
    for(i = 0; i < tamanho; i++){
        med_response += rt[i];
        aux[i] = lburst[i];
        rt[i] = -1;
        flag[i] = 0;
    }

    fprintf(fp,"\n%s\n","METRICS");
    
    cont = 0,time = 0,falta = tamanho;
    while(falta != 0){
        if (aux[cont] == 0  && flag[cont] == 1){
            cont++;
            continue;
        }
        else if (aux[cont] <= QT && aux[cont] > 0){
            if (rt[cont] == -1){
                rt[cont] = time;
            }
            time += aux[cont];
            /*fprintf(fp,"[%s] %s %d %s\n",lname[cont],"for",aux[cont],"units");*/
            flag[cont] = 1;
            aux[cont] = 0;
            
        }
        else if (aux[cont] > QT){
            if (rt[cont] == -1){
                 rt[cont] = time;
            }
            /*fprintf(fp,"[%s] %s %d %s\n",lname[cont],"for",aux[cont],"units");*/
            aux[cont] -= QT;
            time += QT;
        }

        if(aux[cont]== 0 && flag[cont]== 1){
            falta--;
            fprintf(fp,"\n[%s]\n",lname[cont]);
            fprintf(fp,"%s %s %d\n","Turnaround","time:",time);
            fprintf(fp,"%s %s %d\n","Waiting","time:",time - lburst[cont]);
            fprintf(fp,"%s %s %d\n","Response","time:",rt[cont]);
            flag[cont] = 1; 
        }

        if(cont == tamanho-1){
            cont = 0; 
        }
        else{ 
            cont++;
        } 
    }

    med_turnaround = med_turnaround / tamanho;
    med_waiting = med_waiting / tamanho;
    med_response = med_response / tamanho;

    fprintf(fp,"\n%s %s %s %s %.2f\n","Average","turnaround","time","=",med_turnaround);
    fprintf(fp,"%s %s %s %s %.2f\n","Average","waiting","time","=",med_waiting);
    fprintf(fp,"%s %s %s %s %.2f","Average","response","time","=",med_response);
    fclose(fp);
}


int main(int argc, char *argv[])
{
    FILE *in;
    char *temp;
    char task[SIZE];

    char *name;
    int priority;
    int burst;
    int i = 0;
    int lprior[SIZE];
    int lburst[SIZE];
    char *lname[SIZE];

    in = fopen(argv[1],"r");
    
    while (fgets(task,SIZE,in) != NULL) {
        temp = strdup(task);
        name = strsep(&temp,",");
        priority = atoi(strsep(&temp,","));
        burst = atoi(strsep(&temp,","));

        lprior[i] = priority;
        lburst[i] = burst;
        lname[i] = name; 

        i++;
        free(temp);
    }

    fclose(in);

    schedule(i,lprior,lburst,lname);


    return 0;
}
