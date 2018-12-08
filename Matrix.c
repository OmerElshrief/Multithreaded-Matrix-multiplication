#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

int **matrixA;
int **matrixB;
int ** matrixResult;
int c1,r1,c2,r2;

typedef struct {
  int i;
  int j;
}Data;
pthread_mutex_t mutex;

void readInput(char* url);
void* getCell(void* args);
void* getRow(void* args);
void writeToOutput(const char*url,double time);

void main(void)
{

  printf("HELLOOO");
  readInput("input.txt");
  pthread_t threadsForRows[r1];
  pthread_t threadsForcells[r1][c2];
  int arg[c2][2];
  int arg2[r1];
    double time;
    clock_t start;
    clock_t end;

  int choice;

  while(1)
  {
    printf("\n**Please Enter 1 for Thread per Cell 2 for Thread per row, 3 for exit**\n >>");
    scanf("%d",&choice);
    switch(choice)
  {
    case 1:
    {
      int i,j;
       start = clock();
      for( i=0;i<r1;i++)
      {
            for( j=0;j<c2;j++)
            {
                Data data = {i,j};
                arg[j][0] = i;
                arg[j][1]=j;
            //    printf("\nBefore, r:%d c:%d",i,j);
                pthread_create(&threadsForcells[i][j],NULL,getCell,&arg[j]);


            }
      }

      for(i=0;i<r1;i++)
        for(j=0;j<c2;j++)
          pthread_join(threadsForcells[i][j],NULL);

       end = clock();
       time =(double)( end-start)/CLOCKS_PER_SEC;;
      writeToOutput("output",time);
      break;
    }

    case 2:
    {
       start = clock();
      for(int i=0;i<r1;i++)
      {
        arg2[i]=i;
        pthread_create(&threadsForRows[i],NULL,getRow,&arg2[i]);
      }

       end = clock();
       time =(double) (end-start)/CLOCKS_PER_SEC;;
      writeToOutput("output",time);
      break;

    }
    case 3:
    return;


  }

}





}
void readInput(char* url)
{
  FILE *input = fopen(url,"r");


  fscanf(input,"%d %d",&r1,&c1);


int i,j;

//Dynamically inisialize the 2 Matrices;
  matrixA = (int**)malloc(sizeof(int*)*r1);
  for( i=0;i<r1;i++)
  matrixA[i] = (int*)malloc(c1*sizeof(int));

  for(i=0;i<r1;i++)
  {
    for(j=0;j<c1;j++)
        fscanf(input,"%d",&matrixA[i][j]);

  }

  fscanf(input,"%d %d",&r2,&c2);
  matrixB = (int**)malloc(sizeof(int*)*r2);
  for(int i=0;i<r2;i++)
  matrixB[i] = (int*)malloc(c2*sizeof(int));

  for(i=0;i<r2;i++)
  {
    for(j=0;j<c2;j++)
        fscanf(input,"%d",&matrixB[i][j]);

  }
  printf("\n r1=%d , c1= %d \nr2= %d , c2= %d\n",r1,c1,r2,c2);
//For the Result Matrix
  matrixResult= (int**) malloc(sizeof(int*) * c1);
  for(int i=0;i<c1;i++)
  matrixResult[i] = (int*)malloc(sizeof(int)*r2);

fclose(input);
}


void writeToOutput(const char*url,double time)
{
  FILE *output = fopen(url,"a");
//  printf("\nFor Writing:> \nrows = %d\n columns = %d",r1,c2);
fprintf(output,"\n");
  for(int i=0;i<r1;i++)
   {
    for(int j=0;j<c2;j++)
     {
      fprintf(output,"%d ",matrixResult[i][j]);
     }
     fprintf(output,"\n");
  }

  fprintf(output,"\n");
  fprintf(output, "Time Elasped: %lf\n",time);
  fclose(output);
}

void* getCell(void* args)
{

  pthread_mutex_lock(&mutex);
  int *arguments = (int*)args;
  int r = (int)arguments[0];
  int c = (int)arguments[1];
  pthread_mutex_unlock(&mutex);

  matrixResult[r][c]=0;

  for(int i=0;i<c1;i++)
  {
    matrixResult[r][c]+=matrixA[r][i] * matrixB[i][c];
  }
  pthread_exit(NULL);
  //printf("\nrow: %d, column: %d => %d",r,c,matrixResult[r][c]);
}


void* getRow(void* args)
{
  pthread_mutex_t mutex;
  pthread_mutex_lock(&mutex);
  int *arguments = (int*)args;
  int r= (int)arguments[0];
  pthread_mutex_unlock(&mutex);
  for(int k =0;k<c2;k++)
  {
      matrixResult[r][k]=0;
      for(int i=0;i<c1;i++)
      {
            matrixResult[r][k]+=matrixA[r][i] * matrixB[i][k];
      }

  }
  pthread_exit(NULL);
}
