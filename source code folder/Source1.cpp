#include<iostream>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
void change(Mat src, Mat dst, Point a, Point b, Point c, Point d, Point a1, Point b1, Point c1, Point d1)
{
	Mat x(8, 1, CV_32FC1, Scalar(0));
	x.ptr<float>(0)[0] = a1.x;
	x.ptr<float>(1)[0] = a1.y;
	x.ptr<float>(2)[0] = b1.x;
	x.ptr<float>(3)[0] = b1.y;
	x.ptr<float>(4)[0] = c1.x;
	x.ptr<float>(5)[0] = c1.y;
	x.ptr<float>(6)[0] = d1.x;
	x.ptr<float>(7)[0] = d1.y;	
	//initial matrix
	Mat xx(8, 8, CV_32FC1, Scalar(0));
	//**************************************************
	xx.ptr<float>(0)[0] = (float)a.x;  xx.ptr<float>(0)[1] = (float)a.y;  xx.ptr<float>(0)[2] = 1;  xx.ptr<float>(0)[6] = -(float)a.x*a1.x;  xx.ptr<float>(0)[7] = -(float)a.y*a1.x;
	xx.ptr<float>(1)[3] = (float)a.x;  xx.ptr<float>(1)[4] = (float)a.y;  xx.ptr<float>(1)[5] = 1;  xx.ptr<float>(1)[6] = -(float)a.x*a1.y;  xx.ptr<float>(1)[7] = -(float)a.y*a1.y;
	xx.ptr<float>(2)[0] = (float)b.x;  xx.ptr<float>(2)[1] = (float)b.y;  xx.ptr<float>(2)[2] = 1;  xx.ptr<float>(2)[6] = -(float)b.x*b1.x;  xx.ptr<float>(2)[7] = -(float)b.y*b1.x;
	xx.ptr<float>(3)[3] = (float)b.x;  xx.ptr<float>(3)[4] = (float)b.y;  xx.ptr<float>(3)[5] = 1;  xx.ptr<float>(3)[6] = -(float)b.x*b1.y;  xx.ptr<float>(3)[7] = -(float)b.y*b1.y;
	xx.ptr<float>(4)[0] = (float)c.x;  xx.ptr<float>(4)[1] = (float)c.y;  xx.ptr<float>(4)[2] = 1;  xx.ptr<float>(4)[6] = -(float)c.x*c1.x;  xx.ptr<float>(4)[7] = -(float)c.y*c1.x;
	xx.ptr<float>(5)[3] = (float)c.x;  xx.ptr<float>(5)[4] = (float)c.y;  xx.ptr<float>(5)[5] = 1;  xx.ptr<float>(5)[6] = -(float)c.x*c1.y;  xx.ptr<float>(5)[7] = -(float)c.y*c1.y;
	xx.ptr<float>(6)[0] = (float)d.x;  xx.ptr<float>(6)[1] = (float)d.y;  xx.ptr<float>(6)[2] = 1;  xx.ptr<float>(6)[6] = -(float)d.x*d1.x;  xx.ptr<float>(6)[7] = -(float)d.y*d1.x;
	xx.ptr<float>(7)[3] = (float)d.x;  xx.ptr<float>(7)[4] = (float)d.y;  xx.ptr<float>(7)[5] = 1;  xx.ptr<float>(7)[6] = -(float)d.x*d1.y;  xx.ptr<float>(7)[7] = -(float)d.y*d1.y;
	//**************************************************
	Mat h = xx.inv()*x;
	//**************************************************
	Mat nexth(3, 3, CV_32FC1, Scalar(0));
	int count = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (count <= 7)
			{
				nexth.ptr<float>(i)[j] = h.ptr<float>(count)[0];
			}
			else nexth.ptr<float>(i)[j] = 1;
			count++;
		}
	}
	////***************************************************
	for (int i = 0; i < dst.rows; i++)
	{
		for (int j = 0; j < dst.cols; j++)
		{
			Mat tmp(3, 1, CV_32FC1, Scalar(1));
			tmp.ptr<float>(0)[0] = j;
			tmp.ptr<float>(1)[0] = i;
			Mat result(3, 1, CV_32FC1, Scalar(1));
			result = nexth.inv()*tmp;
			result.ptr<float>(0)[0] /= result.ptr<float>(2)[0];
			result.ptr<float>(1)[0] /= result.ptr<float>(2)[0];
			result.ptr<float>(2)[0] = 1;
			int tx = result.ptr<float>(0)[0];
			int ty = result.ptr<float>(1)[0];
			if (ty>0 && tx>0  && ty < src.rows &&  tx < src.cols)
			{
				dst.ptr<uchar>(i, j)[0] = src.ptr<uchar>(ty, tx)[0];
				dst.ptr<uchar>(i, j)[1] = src.ptr<uchar>(ty, tx)[1];
				dst.ptr<uchar>(i, j)[2] = src.ptr<uchar>(ty, tx)[2];
			}
		}
	}
}


int main()
{
	Mat src = imread("1.jpg", CV_LOAD_IMAGE_ANYCOLOR);
	Mat dst(src.rows, src.cols, CV_8UC3, Scalar(0));
	Point a(432,204); Point a1(501,313);
	Point b(973,636); Point b1(1008,798);
	Point c(1520,664); Point c1(1485,863);
	Point d(2027,349); Point d1(1883,601);
	change(src,dst,a,b,c,d,a1,b1,c1,d1);//convert to 2.jpg
	//*********************************
	//¶K¸É
	Mat src1 = imread("2.jpg", CV_LOAD_IMAGE_ANYCOLOR);
	Mat src2 = imread("2zz.jpg", CV_LOAD_IMAGE_ANYCOLOR);
	for (int i = 0; i < src1.rows; i++)
	{
		for (int j = 0; j <src1.cols; j++)
		{
			if (src1.ptr<uchar>(i, j)[0] > 240 && src1.ptr<uchar>(i, j)[1] > 240 && src1.ptr<uchar>(i, j)[2] > 240)
			{
				src2.ptr<uchar>(i, j)[0] = dst.ptr<uchar>(i, j)[0];
				src2.ptr<uchar>(i, j)[1] = dst.ptr<uchar>(i, j)[1];
				src2.ptr<uchar>(i, j)[2] = dst.ptr<uchar>(i, j)[2];
			}

		}
	}
	//*************************************¬M®g¦^600*800
	int w = 800; int h = 600;
	Mat dst1(h, w, CV_8UC3, Scalar(0));
	Point a2(478, 306); Point a12(0, 0);
	Point b2(1883, 603); Point b12(w, 0);
	Point c2(455, 1901); Point c12(0, h);
	Point d2(1909,1744); Point d12(w, h);
	change(src2, dst1, a2, b2, c2, d2, a12, b12, c12, d12);
	//**********************************
	imwrite("result.jpg",dst1);
 	system("pause");
}