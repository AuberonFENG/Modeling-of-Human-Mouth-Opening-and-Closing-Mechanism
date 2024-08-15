
#define GRAY    1
#define COLOR   2


typedef struct Image
{
  int Width;
  int Height;
  int Type;  // Gray=1, Color=2;
  unsigned short *data;
  char comments[15][100];
  int num_comment_lines;
}Image;

typedef struct Image8
{
	int Width;
	int Height;
	int Type;  // Gray=1, Color=2;
	unsigned char *data;
	char comments[15][100];
	int num_comment_lines;
}Image8;

typedef struct DFT
{
	int Width;
	int Height;
	int Type;  // Gray=1, Color=2;
	float *data_real;
	float *data_imag;
	char comments[15][100];
	int num_comment_lines;
}DFT;

