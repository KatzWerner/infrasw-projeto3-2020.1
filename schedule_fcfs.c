#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE    100

void schedule (int tamanho,int lprior[],int lburst[],char* lname[]){

    int i,turnaround,waiting,response;
    float med_turnaround = 0,med_waiting = 0,med_response = 0;
    FILE *fp = fopen("fcfs2.txt","w");
    fprintf(fp,"%s\n\n","EXECUTION BY FCFS");
    for(i = 0;i < tamanho;i++){
    fprintf(fp,"[%s] %s %d %s\n",lname[i],"for",lburst[i],"units");
    }
    fprintf(fp,"\n%s\n","METRICS");
    turnaround = 0;
    waiting = 0;
    response = 0;
    for(i = 0;i < tamanho;i++){
        turnaround += lburst[i];
        waiting = turnaround - lburst[i];
        response = waiting;
        fprintf(fp,"\n[%s]\n",lname[i]);
        fprintf(fp,"%s %s %d\n","Turnaround","time:",turnaround);
        fprintf(fp,"%s %s %d\n","Waiting","time:",waiting);
        fprintf(fp,"%s %s %d\n","Response","time:",response);

        med_turnaround += turnaround;
        med_waiting += waiting;
        med_response += response;
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
