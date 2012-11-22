#include "Stdafx.h"

#include "headers.h"
#include "IntSensors.h"


int sensor_layer::ImageInit(char *FileName){
  FILE *F;
  int i;
  if ((F = fopen(FileName, "rb")) == NULL) return 0;
  //Перемещаемся в bmp на нужную позицию, считываем ширину и длинну
  fseek(F, 18, SEEK_SET);
  fread(&(pict.W), 2, 1, F);
  fseek(F, 2, SEEK_CUR);
  fread(&(pict.H), 2, 1, F);
  //printf("%d x %d\n", pict.W, pict.H);


  if ((pict.Image = (unsigned char *) malloc(sizeof(unsigned char)*3*pict.W * pict.H)) == NULL){
	//MessageBox(NULL, L"error reading image", NULL, MB_OK);
	fclose(F);
	return 0;
  }

  // Считываем изображение в память по 3 бита (RGB для каждого пикселя)
  fseek(F, 30, SEEK_CUR);
  fread(pict.Image, 3, pict.W * pict.H, F);
  fclose(F);
  //printf("image ok\n");
  return 1;
}

int sensor_layer::GetHeight(int x, int y){ //высота от 0 до 255
  return  (pict.Image[(y*pict.W+x)*3] + pict.Image[(y*pict.W+x)*3 + 1] + pict.Image[(y*pict.W+x)*3 + 2])/3;

}



sensor* sensor_layer::GetSensor(int n){
  sensor*tmp=Head;
  while(n>0){
	if (tmp==NULL) return NULL;
	tmp=tmp->next;
	n--;
  }
  return tmp;
}



void sensor_layer::Translate(int oldx, int oldy, int*newx, int*newy, int xt, int yt, double cos_angle, double sin_angle){
 *newx = xt + (int)((double)oldx * cos_angle  +  (double)oldy * sin_angle) ;
 *newy = yt + (int)((double)oldx * sin_angle  -  (double)oldy * cos_angle) ;
}

int sensor_layer::AverageHeight(sensor A, int xt, int yt, double cos_angle, double sin_angle){
  const int width = abs(A.x1 - A.x2);
  const int height = abs(A.y1 - A.y2);
  const double dx = (double) width / (double) A.N;
  const double dy = (double) height / (double) A.N;

  double line, column;
  double sum=0;

  int tempx, tempy;
  if(width==0 || height == 0) return 0;
  printf("AVG: w=%d h=%d dx=%lf dy=%lf N=%lf\n", width, height, dx, dy, (double) A.N);
  for(line = (double)A.y2 + dy/2; line < (double)A.y1; line += dy){
	  printf("for %lf <= %lf\n", line, (double)A.y1);
	  getch();
	for(column = (double) A.x2 + dx/2; column <= (double)A.x1; column +=dx){
		//printf("translating %d %d %d %d %lf %lf\n", (int)column, (int)line, xt, yt, cos_angle, sin_angle);
	  Translate((int)column, (int)line, &tempx, &tempy, xt, yt, cos_angle, sin_angle);
		//printf("translate ok\n");
	 if(tempx > pict.W) tempx=pict.W; //если сенсор вылез за край карты
	 if(tempy > pict.H) tempy=pict.H;
	 if(tempx < 0) tempx=0;
	 if(tempy < 0) tempy=0;
	 //printf("getting height\n");
	 sum += GetHeight( tempx, tempy);
		//printf("GetHeight ok\n");
	}
  }
  //printf("stop\n");
  printf("returning %d\n", sum / (A.N * A.N));
  return sum / (A.N * A.N);
}


sensor_layer::sensor_layer(){
	S=(sensor*)malloc(sizeof(sensor));
	Head=NULL;
	i=0;

	in=fopen("./sensors/map.txt", "r");

	if(!ImageInit("./sensors/cover_hght.bmp")){
	  //MessageBox(NULL, "cannot open image", NULL, MB_OK);
	  return;
	}


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
	//printf("init ok\n");

  }
int sensor_layer::get_height(double speed, int angle, int xt, int yt, int xa, int ya, std::vector<int> heights){
	  int j=0;
	  //printf("GET HEIGHT\n");
	  //printf("reallocking %d\n", (i+1));
	  //*heights=(int*)realloc(*heights, sizeof(int)*(i+1));

	  heights.clear();
	  while(S->next!=NULL){ //обход сенсоров
		  //printf("j=%d\n", j);
		  //if (S==NULL) printf("NULL\n");
		  printf("avg hght: %d %d %lf %lf\n", xt, yt, cos((double)angle), sin((double)angle));
	  heights.push_back(AverageHeight( *S, xt, yt, cos((double)angle), sin((double)angle) ));
		printf("debug:sensors: height=%d\n", heights[j]);
	  S=S->next;
	  printf("next ok\n");
	  j++;
	}

	S=Head;
	//fprintf(out, "\n");
	//if(feof(tmp)) break;
	/*if(i!=j){
		printf("sensor number mistmach\n");
		return -1;
	} */
	printf("AH OK\n");
	return j;
  }

sensor_layer::~sensor_layer(){
  fclose(in);
  free(pict.Image);

  //освобождаем память
  S=Head;
  while(S!=NULL){
	del=S;
	free(del);
	S=S->next;
  }
}


