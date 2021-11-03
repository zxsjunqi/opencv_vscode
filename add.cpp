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
	//通过图片路径获取图片
	Mat img = imread(imagePath);
	unsigned char qh, wh;
	unsigned long offset;	
	//用于存放从记事本读取的十六进制		
	unsigned char hexcode[30];          
	//打开包含名字的txt文件，转为十六进制，存入hexcode中
	FILE* filetxt;
	if ((filetxt = fopen(txtPath, "rb")) == NULL){
		printf("Can't open txtfile,Please check the path!");
		//getch();
		exit(0);
	}
	fseek(filetxt, 0, SEEK_SET);			 /*将文件指针移动到偏移量的位置*/
	fread(hexcode, 30, 1, filetxt);
	int x = x_offset, y = y_offset;//x,y:在图片上绘文字的起始的坐标
	for (int m = 0; m < 30; )
		if (hexcode[m] == 0x23)                /* 当读取到‘#’号时结束 */
			break;
		//判断高阶两个十六进制数，大于或等于b0（汉字第一个是b0a1）的都应该从汉字字库中找
		else if (hexcode[m] > 0xaf)
		{
			qh = hexcode[m] - 0xaf;						/*计算区码*/
			wh = hexcode[m+1] - 0xa0;						/*计算位码*/
			offset = (94 * (qh - 1) + (wh - 1)) * 72L;/*计算该汉字在字库中偏移量*/
			PaintSChinese(img, x , y , offset);
			m = m + 2;                               //一个汉字占两个char，所以加2                             						
			x += 24;      //一个汉字在图片上占24个像素点，所以水平坐标每次+24
		}
		else
		{
			wh = hexcode[m];
			offset = wh * 16L;   
			cout<<offset<<endl;                //计算其他英文字符的偏移量
			PaintSAscii(img, x, y, offset);
			m++;                                //一个char
			//原本应该8*16，但是和24*24比小了点，所以改为16*32，原本的一个像素点现在用四个像素点绘画
			x += 16;
		}
		cv::imshow("image", img);
    cv::waitKey();				
}
void PaintSChinese(Mat& image, int x_offset, int y_offset, unsigned long offset){
	//实际在图片上绘制的像素点坐标
	Point p;
	p.x = x_offset;
	p.y = y_offset;
	//打开hzk24汉字库文件
	FILE *HZK;
	char buff[72];            //存放汉字字膜
	if ((HZK = fopen("HZKf2424.hz", "rb")) == NULL){
		printf("Can't open hzk24.hz,Please check the path!");
		//getch();
		exit(0);
	}
	fseek(HZK, offset, SEEK_SET);			 /*将文件指针移动到偏移量的位置*/
	fread(buff, 72, 1, HZK);				 /*从偏移量的位置读取72个字节，每个汉字占72个字节*/
	bool mat[24][24];                        /*定义一个新的矩阵存放转置后的文字字膜 */
	int i, j, k;
	//转置汉字字膜矩阵，因为汉字字膜存储的是装置后的数据（反的）
	for (i = 0; i<24; i++)                 /*24x24点阵汉字，一共有24行*/
	{
		for (j = 0; j<3; j++)                /*横向有3个字节，循环判断每个字节的*/
			for (k = 0; k<8; k++)              /*每个字节有8位，循环判断每位是否为1*/
				if (buff[i * 3 + j] & (0x80 >> k))    /*测试当前位是否为1*/
				{
					mat[j * 8 + k][i] = true;          /*为1的存入新的字膜中*/
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
				circle(image, p, 1, Scalar(255, 0, 0), -1);		  //写(替换)像素点
			p.x++;                                                //右移一个像素点
		}
		p.y++;                                                    //下移一个像素点
	}

}
void PaintSAscii(Mat& image, int x_offset, int y_offset, unsigned long offset){
	//绘制的起点坐标
	Point p;
	p.x = x_offset;
	p.y = y_offset;
	 //存放ascii字膜
	char buff[16];           
	//打开ascii字库文件
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
	for (i = 0; i<16; i++)                  //十六个char
	{
		p.x = x_offset;
		for (j = 0; j < 8; j++)              //一个char八个bit
		{
			p1 = p;
			if (buff[i] & (0x80 >> j))    /*测试当前位是否为1*/
			{
				/*
					由于原本ascii字膜是8*16的，不够大，
					所以原本的一个像素点用4个像素点替换，
					替换后就有16*32个像素点
					ps：感觉这样写代码多余了，但目前暂时只想到了这种方法
				*/
				circle(image, p1, 0, Scalar(0, 0, 255), -1);
				p1.x++;
				circle(image, p1, 0, Scalar(0, 0, 255), -1);
				p1.y++;
				circle(image, p1, 0, Scalar(0, 0, 255), -1);
				p1.x--;
				circle(image, p1, 0, Scalar(0, 0, 255), -1);
				
			}						
			p.x+=2;            //原来的一个像素点变为四个像素点，所以x和y都应该+2
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
