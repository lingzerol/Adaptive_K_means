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
#include <map>
#define INF 0x3f3f3f3f
using namespace std;
using namespace cv;

const double scope = 11.5;

class D_Point {
public:
	D_Point() : x(0), y(0) {}
	D_Point(double _x, double _y) :x(_x), y(_y) {}
	D_Point(const Point& p) :x(p.x), y(p.y) {}
	operator Point() const {
		Point t;
		t.x = x;
		t.y = y;
		return t;
	}
	double x, y;
};
class Cluster {
public:
	Cluster() :x(0), y(0), num(0) {}
	Cluster(double _x, double _y) :x(_x), y(_y), num(1) {}
	Cluster(const Point& p) :x(p.x), y(p.y), num(1) {}
	Cluster(const D_Point& p) :x(p.x), y(p.y), num(1) {}
	void add(const D_Point& p) {
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
	P_C() :id(0), dis(0), label(-1) {}
	P_C(int _id, const D_Point &_p, int _label) :id(_id), p(_p), dis(0), label(_label) {}
	P_C(int _id, const Point &_p, int _label) :id(_id), p(_p), dis(0), label(_label) {}
	P_C(int _id, const D_Point &_p, int _dis, int _label) :id(_id), p(_p), dis(_dis), label(_label) {}
	P_C(int _id, const Point &_p, int _dis, int _label) :id(_id), p(_p), dis(_dis), label(_label) {}
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
vector<int> get_seed(int, int);
void get_neighbour(const double, const double, D_Point, vector<int>&, const vector<Point>&);

template<typename T>
bool check(const T& p, double xmin, double xmax, double ymin, double ymax)
{
	if (p.x >= xmin && p.x <= xmax && p.y <= ymax && p.y >= ymin) {
		return true;
	}
	return false;
}

void Adaptive_K_means(vector<int>&label, const vector<D_Point>&p, int k, int scope) {
	const static Scalar Color[] = { CV_RGB(0,0,0),CV_RGB(0,0,130) ,CV_RGB(0,130,0) ,CV_RGB(130,0,0) ,CV_RGB(150,150,0)
	,CV_RGB(0,150,150) ,CV_RGB(150,0,150) ,CV_RGB(120,130,150) ,CV_RGB(130,120,150) ,CV_RGB(170,130,130),
	CV_RGB(200,30,40), CV_RGB(40,30,200), CV_RGB(70,230,0), CV_RGB(0,70,230), CV_RGB(250,30,250), };
	double xmax, xmin, ymax, ymin;
	xmax = ymax = -INF;
	xmin = ymin = INF;
	for (int i = 0; i < p.size(); ++i) {
		xmax = max(p[i].x, xmax);
		xmin = min(p[i].x, xmin);
		ymax = max(p[i].y, ymax);
		ymin = min(p[i].y, ymin);
	}
	const double XSPAN = max(5.0, (double)(xmax - xmin) / k / 5);
	const double YSPAN = max(5.0, (double)(ymax - ymin) / k / 5);



	//sort(p.begin(), p.end(), comp);
	label.resize(p.size());
	for (int i = 0; i < label.size(); ++i)
		label[i] = -1;
	std::vector<int> seed = get_seed(k, p.size());
	priority_queue<P_C, vector<P_C>, Compare> q;
	std::vector<Cluster> cluster;
	vector<Cluster> rcen;
	vector<double> xspan;
	vector<double> yspan;
	rcen.resize(seed.size());
	for (int i = 0; i < seed.size(); ++i) {
		D_Point temp = p[seed[i]];
		temp.x = log(temp.x)*scope;
		temp.y = log(temp.y)*scope;
		cluster.push_back(temp);
		q.push(P_C(seed[i], p[seed[i]], i));
		xspan.push_back(XSPAN);
		yspan.push_back(YSPAN);
	}
	int num = 0;
	while (!q.empty()) {
		P_C temp = q.top();
		q.pop();
		int k = temp.label;
		int id = temp.id;
		if (-1 == label[id]) {
			++num;
			cluster[k].add(temp.p);
			if (seed[k] == id) {
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
			} while (0 == number && num < p.size());
		}
	}

	for (int i = 0; i < p.size(); ++i) {
		if (10 > cluster[label[i]].num) {
			for (int j = 0; j < p.size(); ++j) {
				if (label[i] != label[j] && 10 <= cluster[label[j]].num&&check(p[j], p[i].x - XSPAN * 10, p[i].x + XSPAN * 10, p[i].y - YSPAN * 10, p[i].y + YSPAN * 10)) {
					--cluster[label[i]].num;
					++cluster[label[j]].num;
					label[i] = label[j];
					break;
				}
			}
		}
	}
	Mat image(1000, 1000, CV_8UC3, CV_RGB(255, 255, 255));
	namedWindow("SHOW");
	imshow("SHOW", image);

	int u = 0;
	map<int, int> ltoi;
	for (int j = 0; j < p.size(); ++j) {
		//cout << label[j] << " ";
		if (ltoi.end() == ltoi.find(label[j]))
			ltoi[label[j]] = u++;

		circle(image, p[j], 4, Color[ltoi[label[j]]], -1);
		imshow("SHOW", image);
		waitKey(10);
	}
	waitKey(0);
	destroyWindow("SHOW");
	cout << endl;
}

int get_accuracy(const vector<int> origin, const vector<int> origrin_center, vector<int> label) {
	vector<int> otr;
	int num = 0;
	for (int i = 0; i < origrin_center.size(); ++i) {
		int l1, l2;
		l1 = origin[origrin_center[i]];
		l2 = label[origrin_center[i]];
		for (int j = 0; j < label.size(); ++j) {
			if (origin[j] == l1 && label[j] != l2) {
				++num;
			}
		}
	}
	return num;
}

int main()
{
	ifstream in("./data06.txt");
	vector<D_Point> p;
	vector<int>label;
	vector<int> origin;
	vector<int> origin_center;
	int l = 0;
	int tl;
	double x;
	double y;
	int n = 0;
	while (in >> x >> y>>tl) {
		p.push_back(D_Point(x, y));
		origin.push_back(tl);
		if (l != tl) {
			++l;
			origin_center.push_back(n - 1);
		}
		
		++n;
	}
	origin_center.push_back(n - 1);
	in.close();
	Adaptive_K_means(label, p, 40, 10);
	cout << get_accuracy(origin, origin_center, label) << endl;
	return 0;
}


vector<int> get_seed(int k, int num) {
	srand((unsigned)time(NULL));
	int* label = new int[num];
	memset(label, -1, sizeof(int)*num);
	vector<int> seed;
	for (int i = 0; i < k; ++i) {
		int j = rand() % num;
		if (j < 0)
			j = 0;
		if (label[j] != -1) {
			--i;
		}
		else {
			label[j] = i;
			seed.push_back(j);
		}
	}
	delete label;
	return seed;
}