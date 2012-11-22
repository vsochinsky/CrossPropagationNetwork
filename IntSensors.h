#ifndef INT_SENSORS_H
#define INT_SENSORS_H

#include <vector>

struct _bmp
{
  int W;
  int H;
  unsigned char *Image;
};

typedef struct _sens{
  int x1, x2, y1, y2, N;
  struct _sens  *next;
} sensor;

class sensor_layer{
	private:
		struct _bmp pict;
		int ImageInit(char *FileName);
		int GetHeight(int x, int y); //высота от 0 до 255
		sensor*GetSensor(int n);
		void Translate(int oldx, int oldy, int*newx, int*newy, int xt, int yt, double cos_angle, double sin_angle);
		int AverageHeight(sensor A, int xt, int yt, double cos_angle, double sin_angle);
	public:
		FILE *in;
		int xa, ya; //цель
		int x1, x2, y1, y2, N;
		short i;
		sensor*S, *del, *Head;
		sensor_layer();
		int get_height(double speed, int angle, int xt, int yt, int xa, int ya, std::vector<int> heights);
		~sensor_layer();
};

#endif //INT_SENSORS_H