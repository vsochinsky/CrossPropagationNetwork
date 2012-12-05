#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TEMPLOG "templog.txt"


struct _bmp
{
  int W;
  int H;
  unsigned char *Image;
}pict;

int ImageInit(char *FileName){
  FILE *F;
  int i;
  if ((F = fopen(FileName, "rb")) == NULL) return 0;
  //Перемещаемся в bmp на нужную позицию, считываем ширину и длинну
  fseek(F, 18, SEEK_SET);
  fread(&(pict.W), 2, 1, F);
  fseek(F, 2, SEEK_CUR);
  fread(&(pict.H), 2, 1, F);
  printf("%d x %d\n", pict.W, pict.H);


  if ((pict.Image = (unsigned char *) malloc(sizeof(unsigned char)*3*pict.W * pict.H)) == NULL){
    fclose(F);
    return 0;
  }

  // Считываем изображение в память по 3 бита (RGB для каждого пикселя)
  fseek(F, 30, SEEK_CUR);
  fread(pict.Image, 3, pict.W * pict.H, F);
  fclose(F);
  return 1;
}

int GetHeight(int x, int y){ //высота от 0 до 255
  return  (pict.Image[(y*pict.W+x)*3] + pict.Image[(y*pict.W+x)*3 + 1] + pict.Image[(y*pict.W+x)*3 + 2])/3;
  
}




typedef struct _sens{
  int x1, x2, y1, y2, N;
  struct _sens  *next;
} sensor;


sensor *Head=NULL;


sensor*GetSensor(int n){
  sensor*tmp=Head;
  while(n>0){
    if (tmp==NULL) return NULL;
    tmp=tmp->next;
    n--;
  }
  return tmp;
}

void Parse(char logname[256]){
  char c;
  FILE*in=fopen(logname, "r"), *out=fopen(TEMPLOG, "w");
  while( (c = fgetc(in)) !=EOF ){
    if(c!=';'){
      if(c==',') fputc('.', out);
      else fputc(c, out);
    }
  }
  fclose(out);
  fclose(in);
}
      

void Translate(int oldx, int oldy, int*newx, int*newy, int xt, int yt, double cos_angle, double sin_angle){
 *newx = xt + (int)((double)oldx * cos_angle  +  (double)oldy * sin_angle) ;
 *newy = yt + (int)((double)oldx * sin_angle  -  (double)oldy * cos_angle) ;
}

int AverageHeight(sensor A, int xt, int yt, double cos_angle, double sin_angle){
  const int width = A.x1 - A.x2;
  const int height = A.y1 - A.y2;
  const double dx = (double) width / (double) A.N;
  const double dy = (double) height / (double) A.N;

  double line, column;
  double sum=0;

  int tempx, tempy;

  for(line = (double)A.y2 + dy/2; line <= (double)A.y1; line += dy){
    for(column = (double) A.x2 + dx/2; column <= (double)A.x1; column +=dx){
      
      Translate((int)column, (int)line, &tempx, &tempy, xt, yt, cos_angle, sin_angle);
     
     if(tempx > pict.W) tempx=pict.W; //если сенсор вылез за край карты
     if(tempy > pict.H) tempy=pict.H;
     if(tempx < 0) tempx=0;
     if(tempy < 0) tempy=0;
      
     sum += GetHeight( tempx, tempy);

    }
  }
  return sum / (A.N * A.N);
}



void main(void){
  FILE*out, *tmp, *in;
  int xt, yt, height, angle; //танк
  double speed;
  int xa, ya; //цель
  int x1, x2, y1, y2, N;
  short i=0;
  int n=0; //количество обработанных строк лога
  sensor*S=(sensor*)malloc(sizeof(sensor));
 
  sensor*del;

  if(!ImageInit("./cover_hght.bmp")){
    printf("cannot open image\n");
    return;
  }

  out=fopen("out.txt", "w");
  tmp=fopen( TEMPLOG, "r");
  in=fopen("map.txt", "r");
  Parse("LogFile.log");
  
  while(fscanf(in, "%d %d %d %d %d\n", &x1, &y1, &x2, &y2, &N)!=EOF){ //считываем сенсоры
    if(i==0){
      i=1;
      Head=S;
    }
    
    S->next=(sensor*)malloc(sizeof(sensor));
    S->x1=x1;
    S->x2=x2;
    S->y1=y1;
    S->y2=y2;
    S->N=N;
    S=S->next;
  }
  
  if(i==0)
    S=NULL;
  else
    S->next=NULL;
  
  S=Head;


  while(fscanf(tmp, "%lf %d %d %d %d %d", &speed, &angle, &xt, &yt, &xa, &ya)){ //обход лога
    fprintf(out, "%lf; %d; %d; %d; %d; %d;", speed, angle, xt, yt, xa, ya);
    n++;
    
    while(S->next!=NULL){ //обход сенсоров
      
      
      fprintf(  out, " %d;", AverageHeight( *S, xt, yt, cos(angle), sin(angle) )  );
      S=S->next;
    
    }
    
    S=Head;
    fprintf(out, "\n");
    if(feof(tmp)) break;
  }
  
  printf("processed %d lines\n", n);
  fclose(tmp);
  fclose(out);
  fclose(in);
  free(pict.Image);
  
  //освобождаем память
  S=Head;
  while(S!=NULL){
    del=S;
    free(del);
    S=S->next;
  }
  getchar();
}

