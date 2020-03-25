#include <stdio.h>
#include <stdlib.h>

int n;

int getsize(FILE *fp)
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
    return (m-1);
}

int prioridade(int tasks[2][n], int tam)
{
	int taskAux[2][n];
	for(int i = 0; i < n; i++)
	{
		taskAux[0][i] = tasks[0][i];
		taskAux[1][i] = tasks[1][i];
	}

	int aux = -1, ult = tam;
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
	return aux;
}


void deadLine(int tasks[2][n], char nome[n][n+1], int tam)
{
	FILE *fp = fopen("edf.txt", "w");
	fprintf(fp,"EXECUTION BY EDL\n\n");
	int taskAux[2][n], exec = 0, nheca, burstaux[n];
	int compl[n], kill[n], lost[n];
	for(int i = 0; i < n; i++)
	{
		taskAux[0][i] = tasks[0][i];
		taskAux[1][i] = tasks[1][i];
		compl[i] = 0;
		lost[i] = 0;
		kill[i] = 0;
	}
	nheca = prioridade(taskAux, tam);
	for (int t = 0; t < tam; t++)
	{
		int prio = prioridade(taskAux, tam);
		if (prio != -1)
		{
			if (nheca == -1)
			{
				fprintf(fp,"idle for %d units\n", exec);
				nheca = prio;
				exec = 0;
			}
			if (nheca != prio)
			{
				if (exec == tasks[1][nheca] || burstaux[nheca] + exec == tasks[1][nheca])
				{
					burstaux[nheca] = 0;
					fprintf(fp, "[%s] for %d units - F\n", nome[nheca], exec);
					compl[nheca]++;
				}
				else if(exec +burstaux[nheca] + 1 < tasks[1][nheca] && taskAux[0][nheca] <= 1)
				{
					fprintf(fp, "[%s] for %d units - L\n", nome[nheca], exec);
					lost[nheca]++;
				}
				else
				{
					burstaux[nheca] = exec;
					fprintf(fp,"[%s] for %d units - H\n", nome[nheca], exec);
				}
				nheca = prio;
				exec = 0;
			}
			else if (t+1 == tam)
			{
				exec++;
				if (exec == tasks[1][nheca] || burstaux[nheca] + exec == tasks[1][nheca])
				{
					burstaux[nheca] = 0;
					fprintf(fp,"[%s] for %d units - F\n", nome[nheca], exec);
					compl[nheca]++;
				}
				else fprintf(fp,"[%s] for %d units - K\n", nome[nheca], exec);
				for (int i = 0; i < n; ++i)
				{
					if (taskAux[1][i] != 1)
					{
						kill[i]++;
					}
				}

			}
			taskAux[1][prio]--;
			exec++;
		}

		else
		{
			if (nheca != prio)
			{
				fprintf(fp,"[%s] for %d units - F\n", nome[nheca], exec);
				compl[nheca]++;
				nheca = prio;
				exec = 0;
			}
			exec++;
			nheca = prio;
		}
		for(int j = 0; j < n; j++)
		{
			taskAux[0][j]--;
			if (taskAux[0][j] == 0)
			{
				taskAux[0][j] = tasks[0][j];
				taskAux[1][j] = tasks[1][j];
			}
		}
	}
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
		{
			fscanf(fp,"%s %d, %d",&f, &tam, &t);
			aux = 0;
		}
		else if(aux==n) break;
		else
		{
			fscanf(fp, "%s %d, %d", &nome[aux], &tasks[0][aux], &tasks[1][aux]);
			nome[aux][2] = NULL;
			aux++;
		}
	}
	fclose(fp);
	for (int i = 0; i < n; i++)
	{
		p[i] = tasks[0][i];	   
		int sum = 0;
		sum = (tasks[0][i]-tasks[1][i]);
		if (sum < 0)
		{
			return 1;
		}
	}
	deadLine(tasks, nome, tam);
	return 0;
}