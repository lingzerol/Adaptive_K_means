// Adaptive_K_means.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp> 
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <queue>
#define INF 0x3f3f3f3f
using namespace std;
using namespace cv;

const double scope = 4;

class D_Point {
public:
	D_Point() : x(0), y(0) {}
	D_Point(double _x, double _y) :x(_x), y(_y) {}
	D_Point(const Point& p) :x(p.x), y(p.y) {}
	double x, y;
};
class Cluster {
public:
	Cluster() :x(0), y(0), num(0) {}
	Cluster(double _x, double _y) :x(_x), y(_y),num(1) {}
	Cluster(const Point& p) :x(p.x), y(p.y),num(1) {}
	Cluster(const D_Point& p) :x(p.x), y(p.y), num(1) {}
	void add(D_Point p) {
		x += p.x;
		y += p.y;
		++num;
	}
	D_Point get_center() {
		return D_Point(x / num, y / num);
	}
	int num;
private:
	double x, y;

};

class P_C {
public:
	P_C():id(0),dis(0),label(-1){}
	P_C(int _id,const D_Point &_p,int _label) :id(_id),p(_p),dis(0),label(_label) {}
	P_C(int _id, const Point &_p,int _label) :id(_id), p(_p),dis(0), label(_label) {}
	P_C(int _id, const D_Point &_p,int _dis, int _label) :id(_id), p(_p), dis(_dis), label(_label) {}
	P_C(int _id, const Point &_p, int _dis,int _label) :id(_id), p(_p), dis(_dis), label(_label) {}
	D_Point p;
	double dis;
	int label;
	int id;
};

class Compare {
public:
	bool operator()(const P_C&a, const P_C&b) {
		return a.dis > b.dis;
	}
};

bool comp(const Point& a, const Point& b) {
	if (a.x != b.x) {
		return a.x < b.x;
	}
	else return a.y < b.y;
}

template<typename T, typename U>
bool COMPARE(const T& a, const U& b) {
	if (a.x != b.x) {
		return a.x < b.x;
	}
	else return a.y < b.y;
}
vector<int> get_seed(int,int);
void get_neighbour(const double, const double, D_Point,vector<int>&, const vector<Point>&);

template<typename T>
bool check(T p, double xmin,double xmax,double ymin,double ymax)
{
	if (p.x >= xmin && p.x <= xmax&&p.y<=ymax&&p.y>=ymin) {
		return true;
	}
	return false;
}

int main()
{
	const Scalar Color[10] = {CV_RGB(0,0,0),CV_RGB(0,0,130) ,CV_RGB(0,130,0) ,CV_RGB(130,0,0) ,CV_RGB(150,150,0) 
	,CV_RGB(0,150,150) ,CV_RGB(150,0,150) ,CV_RGB(120,130,150) ,CV_RGB(130,120,150) ,CV_RGB(170,130,130) };
	ifstream in("./data00.txt");
	vector<Point> p;
	int x;
	int y;
	int xmax, xmin, ymax, ymin;
	xmax = ymax = -INF;
	xmin = ymin = INF;
	while (in >> x >> y) {
		xmax = max(x, xmax);
		xmin = min(x, xmin);
		ymax = max(y, ymax);
		ymin = min(y, ymin);
		p.push_back(Point(x, y));
	}
	int k=10;
	/*do{
		cout << "input the number of center: ";
		cin >> k;
	}while (k >= p.size());*/
	const double XSPAN = max(5.0,(double)(xmax - xmin)/k/5);
	const double YSPAN = max(5.0,(double)(ymax - ymin)/k/5);
	in.close();


	sort(p.begin(), p.end(), comp);

	int* label = new int[p.size()];
	memset(label, -1, sizeof(int)*p.size());

	std::vector<int> seed = get_seed(k,p.size());
	priority_queue<P_C, vector<P_C>, Compare> q;
	std::vector<Cluster> cluster;
	vector<Cluster> rcen;
	vector<double> xspan;
	vector<double> yspan;
	rcen.resize(seed.size());
	for (int i = 0; i < seed.size(); ++i) {
		D_Point temp = p[seed[i]];
		temp.x /= scope;
		temp.y /= scope;
		cluster.push_back(temp);
		q.push(P_C(seed[i],p[seed[i]],i));
		xspan.push_back(XSPAN);
		yspan.push_back(YSPAN);
	}
	int num = 0;
	while (!q.empty()) {
		P_C temp = q.top();
		q.pop();
		int k = temp.label;
		int id = temp.id;
		if (-1==label[id]) {
			++num;
			cluster[k].add(temp.p);
			if (seed[k]==id) {
				--cluster[k].num;
			}
			rcen[k].add(temp.p);
			label[id] = k;
			D_Point center = cluster[k].get_center();
			D_Point rcenter = rcen[k].get_center();
			int number = 0;
			do {
				number = 0;
				for (int i = 0; i < p.size(); ++i) {
					if (-1 == label[i] && check(p[i], rcenter.x - xspan[k], rcenter.x + xspan[k], rcenter.y - yspan[k], rcenter.y + yspan[k])) {
						D_Point b = p[i];
						double dx, dy;
						dx = center.x - b.x;
						dy = center.y - b.y;
						double dis = dx * dx + dy * dy;
						q.push(P_C(i, b, dis, k));
						++number;
					}
				}
				xspan[k] += XSPAN;
				yspan[k] += YSPAN;
			} while (0==number&&num<p.size());
		}
	}
	
	for (int i = 0; i < p.size(); ++i) {
		if (20 > cluster[label[i]].num) {
			for (int j = 0; j < p.size(); ++j) {
				if (label[i] != label[j] &&check(p[j], p[i].x - XSPAN*2, p[i].x + XSPAN*2, p[i].y - YSPAN*2, p[i].y + YSPAN*2)) {
					--cluster[label[i]].num;
					label[i] = label[j];
					break;
				}
			}
		}
	}
	Mat image(1000, 1000,CV_8UC3,CV_RGB(255, 255, 255));
	namedWindow("SHOW");
	imshow("SHOW", image);
	/*for (int i = 0; i < 10; ++i) {
		Mat image(1000, 1000, CV_8UC3,Color[i]);
		namedWindow("s");
		imshow("s", image);
		waitKey(0);
		destroyWindow("s");
	}*/
	for (int j = 0; j < p.size(); ++j) {
		cout << label[j] << " ";
		circle(image, p[j], 4,Color[label[j]],-1);
		imshow("SHOW", image);
		waitKey(10);
	}
	waitKey(0);
	destroyWindow("SHOW");
	cout << endl;
	delete label;
	return 0;
}


vector<int> get_seed(int k, int num) {
	srand((unsigned)time(NULL));

	int* label = new int[num];
	memset(label, -1, sizeof(int)*num);
	vector<int> seed;
	for (int i = 0; i < k; ++i) {
		int t = rand() % num;
		if (label[t] != -1) {
			--i;
		}
		else { 
			label[t] = i; 
			seed.push_back(t);
		}
	}
	delete label;
	return seed;
}
