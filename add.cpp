#include <iostream>
#include<string.h>
#include<opencv/cv.h>
#include<opencv/cxcore.h>
#include<opencv/highgui.h>
#include"opencv2/opencv.hpp"
#include <math.h>
using namespace cv;
using namespace std;

void PaintSChinese(Mat& image, int x_offset, int y_offset, unsigned long offset);
void PaintSAscii(Mat& image,int x_offset, int y_offset, unsigned long offset);

void putTextToImage(int x_offset,int y_offset,String imagePath ,char* txtPath)
{
	//ͨ��ͼƬ·����ȡͼƬ
	Mat img = imread(imagePath);
	unsigned char qh, wh;
	unsigned long offset;	
	//���ڴ�ŴӼ��±���ȡ��ʮ������		
	unsigned char hexcode[30];          
	//�򿪰������ֵ�txt�ļ���תΪʮ�����ƣ�����hexcode��
	FILE* filetxt;
	if ((filetxt = fopen(txtPath, "rb")) == NULL){
		printf("Can't open txtfile,Please check the path!");
		//getch();
		exit(0);
	}
	fseek(filetxt, 0, SEEK_SET);			 /*���ļ�ָ���ƶ���ƫ������λ��*/
	fread(hexcode, 30, 1, filetxt);
	int x = x_offset, y = y_offset;//x,y:��ͼƬ�ϻ����ֵ���ʼ������
	for (int m = 0; m < 30; )
		if (hexcode[m] == 0x23)                /* ����ȡ����#����ʱ���� */
			break;
		//�жϸ߽�����ʮ�������������ڻ����b0�����ֵ�һ����b0a1���Ķ�Ӧ�ôӺ����ֿ�����
		else if (hexcode[m] > 0xaf)
		{
			qh = hexcode[m] - 0xaf;						/*��������*/
			wh = hexcode[m+1] - 0xa0;						/*����λ��*/
			offset = (94 * (qh - 1) + (wh - 1)) * 72L;/*����ú������ֿ���ƫ����*/
			PaintSChinese(img, x , y , offset);
			m = m + 2;                               //һ������ռ����char�����Լ�2                             						
			x += 24;      //һ��������ͼƬ��ռ24�����ص㣬����ˮƽ����ÿ��+24
		}
		else
		{
			wh = hexcode[m];
			offset = wh * 16L;   
			cout<<offset<<endl;                //��������Ӣ���ַ���ƫ����
			PaintSAscii(img, x, y, offset);
			m++;                                //һ��char
			//ԭ��Ӧ��8*16�����Ǻ�24*24��С�˵㣬���Ը�Ϊ16*32��ԭ����һ�����ص��������ĸ����ص�滭
			x += 16;
		}
		cv::imshow("image", img);
    cv::waitKey();				
}
void PaintSChinese(Mat& image, int x_offset, int y_offset, unsigned long offset){
	//ʵ����ͼƬ�ϻ��Ƶ����ص�����
	Point p;
	p.x = x_offset;
	p.y = y_offset;
	//��hzk24���ֿ��ļ�
	FILE *HZK;
	char buff[72];            //��ź�����Ĥ
	if ((HZK = fopen("HZKf2424.hz", "rb")) == NULL){
		printf("Can't open hzk24.hz,Please check the path!");
		//getch();
		exit(0);
	}
	fseek(HZK, offset, SEEK_SET);			 /*���ļ�ָ���ƶ���ƫ������λ��*/
	fread(buff, 72, 1, HZK);				 /*��ƫ������λ�ö�ȡ72���ֽڣ�ÿ������ռ72���ֽ�*/
	bool mat[24][24];                        /*����һ���µľ�����ת�ú��������Ĥ */
	int i, j, k;
	//ת�ú�����Ĥ������Ϊ������Ĥ�洢����װ�ú�����ݣ����ģ�
	for (i = 0; i<24; i++)                 /*24x24�����֣�һ����24��*/
	{
		for (j = 0; j<3; j++)                /*������3���ֽڣ�ѭ���ж�ÿ���ֽڵ�*/
			for (k = 0; k<8; k++)              /*ÿ���ֽ���8λ��ѭ���ж�ÿλ�Ƿ�Ϊ1*/
				if (buff[i * 3 + j] & (0x80 >> k))    /*���Ե�ǰλ�Ƿ�Ϊ1*/
				{
					mat[j * 8 + k][i] = true;          /*Ϊ1�Ĵ����µ���Ĥ��*/
				}
				else {
					mat[j * 8 + k][i] = false;
				}

	}
	for (i = 0; i < 24; i++)
	{
		p.x = x_offset;
		for (j = 0; j < 24; j++)
		{		
			if (mat[i][j])
				circle(image, p, 1, Scalar(255, 0, 0), -1);		  //д(�滻)���ص�
			p.x++;                                                //����һ�����ص�
		}
		p.y++;                                                    //����һ�����ص�
	}

}
void PaintSAscii(Mat& image, int x_offset, int y_offset, unsigned long offset){
	//���Ƶ��������
	Point p;
	p.x = x_offset;
	p.y = y_offset;
	 //���ascii��Ĥ
	char buff[16];           
	//��ascii�ֿ��ļ�
	FILE *ASCII;
	if ((ASCII = fopen("Asci0816.zf", "rb")) == NULL){
		printf("Can't open ascii.zf,Please check the path!");
		//getch();
		exit(0);
	}
	fseek(ASCII, offset, SEEK_SET);
	fread(buff, 16, 1, ASCII);
	int i, j;
	Point p1 = p;
	for (i = 0; i<16; i++)                  //ʮ����char
	{
		p.x = x_offset;
		for (j = 0; j < 8; j++)              //һ��char�˸�bit
		{
			p1 = p;
			if (buff[i] & (0x80 >> j))    /*���Ե�ǰλ�Ƿ�Ϊ1*/
			{
				/*
					����ԭ��ascii��Ĥ��8*16�ģ�������
					����ԭ����һ�����ص���4�����ص��滻��
					�滻�����16*32�����ص�
					ps���о�����д��������ˣ���Ŀǰ��ʱֻ�뵽�����ַ���
				*/
				circle(image, p1, 0, Scalar(0, 0, 255), -1);
				p1.x++;
				circle(image, p1, 0, Scalar(0, 0, 255), -1);
				p1.y++;
				circle(image, p1, 0, Scalar(0, 0, 255), -1);
				p1.x--;
				circle(image, p1, 0, Scalar(0, 0, 255), -1);
				
			}						
			p.x+=2;            //ԭ����һ�����ص��Ϊ�ĸ����ص㣬����x��y��Ӧ��+2
		}
		p.y+=2;
	}
	
}


int main(){	
	String image_path="D:\\png\\whitegirl.png";
    char* logo_path="logo.txt";
   putTextToImage(150,450,image_path,logo_path);
	return 0;		
}
