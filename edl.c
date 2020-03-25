#include <stdio.h>
#include <stdlib.h>

int n;

int getsize(FILE *fp)//ver quantas tasks foram chamadas
{
    int m = 0;

    if(fp == NULL)
    {
        printf("Unable to open file.\n");
        return -1;
    }

    rewind(fp);
    for(char ch = fgetc(fp); ch != EOF; ch = fgetc(fp))
    {
        if(ch == '\n')
        {
            m++;
        }
    }
    rewind(fp);
    n = m-1;
    return (m-1);//retorna (m-1) pq temos que desconsiderar a primeira linha que diz o tamanho
}

int prioridade(int tasks[2][n], int tam)
{
	int taskAux[2][n];
	for(int i = 0; i < n; i++)
	{
		taskAux[0][i] = tasks[0][i];//cria uma copia do array principal para mechermos nele
		taskAux[1][i] = tasks[1][i];
	}

	int aux = -1, ult = tam;//define ccomo -1 pq todos os periodos estiverem zerados ja retorna -1
	for (int i = 0; i < n; i++)
	{
		if (taskAux[1][i] != 0)
		{
			if (ult > taskAux[0][i])
			{
				aux = i;
				ult = taskAux[0][i];
			}
		}
	}
	return aux;//retorna o index da task que eh prioridade (a que tiver menor periodo)
}


void deadLine(int tasks[2][n], char nome[n][n+1], int tam)
{
	FILE *fp = fopen("edf.txt", "w");
	fprintf(fp,"EXECUTION BY EDL\n\n");
	int taskAux[2][n], exec = 0, past, burstaux[n];
	int compl[n], kill[n], lost[n];
	for(int i = 0; i < n; i++)
	{
		taskAux[0][i] = tasks[0][i];
		taskAux[1][i] = tasks[1][i];
		//contadores parasaber quantas tasks passaram por cada estado
		compl[i] = 0;//contador das tasks completas
		lost[i] = 0;//contador das tasks perdidas
		kill[i] = 0;//contador das tasks 'mortas'
	}
	past = prioridade(taskAux, tam);
	for (int t = 0; t < tam; t++)
	{//nesse for sempre se compara o estado passado para facilitar a gerencia doq está acontecendo
		int prio = prioridade(taskAux, tam);
		if (prio != -1)
		{
			if (past == -1)
			{
				fprintf(fp,"idle for %d units\n", exec);
				past = prio;
				exec = 0;
			}
			if (past != prio)
			{
				if (exec == tasks[1][past] || burstaux[past] + exec == tasks[1][past])
				{
					burstaux[past] = 0;//task finalizada de maneira direta ou separada por um 'hold'
					fprintf(fp, "[%s] for %d units - F\n", nome[past], exec);
					compl[past]++;
				}
				else if(exec +burstaux[past] + 1 < tasks[1][past] && taskAux[0][past] <= 1)
				{//task n conseguiu ser terminada antes de seu periodo acabar
					fprintf(fp, "[%s] for %d units - L\n", nome[past], exec);
					lost[past]++;
				}
				else
				{//salva o processo ja feito no burstaux e troca para outra task de maior prioridade
					burstaux[past] = exec;
					fprintf(fp,"[%s] for %d units - H\n", nome[past], exec);
				}
				past = prio;
				exec = 0;
			}
			else if (t+1 == tam)
			{//oq fazer quando acabar todo o tamanho
				exec++;
				if (exec == tasks[1][past] || burstaux[past] + exec == tasks[1][past])
				{//caso o exec esteja do tamanho do burst original printar com o 'f'
					burstaux[past] = 0;
					fprintf(fp,"[%s] for %d units - F\n", nome[past], exec);
					compl[past]++;
				}//se estiver menor a tarefa foi morta e se printa com o 'k'
				else fprintf(fp,"[%s] for %d units - K\n", nome[past], exec);
				for (int i = 0; i < n; ++i)
				{
					if (taskAux[1][i] != 1)
					{
						kill[i]++;
					}
				}

			}
			taskAux[1][prio]--;//diminui o burst da tarefa em questao no arrray auxiliar
			exec++;
		}

		else
		{
			if (past != prio)
			{//printar ultima tarefa que estava sendo feita antes de se ir idle
				fprintf(fp,"[%s] for %d units - F\n", nome[past], exec);
				compl[past]++;
				past = prio;
				exec = 0;
			}
			exec++;
			past = prio;
		}
		for(int j = 0; j < n; j++)
		{//diminui o periodo de TODAS as tarefas
			taskAux[0][j]--;
			if (taskAux[0][j] == 0)
			{
				taskAux[0][j] = tasks[0][j];
				taskAux[1][j] = tasks[1][j];
			}
		}
	}//printa as tarefas que passaram por cada estado
	fprintf(fp,"\nLOST DEADLINES\n");
	for (int i = 0; i < n; ++i)
	{
		fprintf(fp,"[%s]: %d\n", nome[i], lost[i]);
	}
	fprintf(fp,"\nCOMPLETE EXECUTION\n");
	for (int i = 0; i < n; ++i)
	{
		fprintf(fp,"[%s]: %d\n", nome[i], compl[i]);
	}
	fprintf(fp,"\nKILLED\n");
	for (int i = 0; i < n; ++i)
	{
		fprintf(fp,"[%s]: %d\n", nome[i], kill[i]);
	}
	fclose(fp);
}


int main(int argc, char const *argv[])
{
	FILE *fp = fopen(argv[1], "r");
	int n, tam, aux, t;
	n = getsize(fp);
	aux = n;
	int tasks[2][n], taskAux[2][n], p[n];
	char nome[n][n+1], *f;
	aux = -1;
	while(!feof(fp))
	{
		if (aux == -1)
		{//le o tamanho
			fscanf(fp,"%s %d, %d",&f, &tam, &t);
			aux = 0;
		}
		else if(aux==n) break;
		else
		{//le as tarefas e suas especificações uma por uma
			fscanf(fp, "%s %d, %d", &nome[aux], &tasks[0][aux], &tasks[1][aux]);
			nome[aux][2] = NULL;
			aux++;
		}
	}
	fclose(fp);
	for (int i = 0; i < n; i++)
	{//verifica se eh possivel fazer as tasks  
		int sum = 0;
		sum = (tasks[0][i]-tasks[1][i]);
		if (sum < 0)
		{
			return 1;
		}
	}
	//faz o schedule
	deadLine(tasks, nome, tam);
	return 0;
}