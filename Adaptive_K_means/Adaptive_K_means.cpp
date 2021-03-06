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
int dimension = -1;
void set_dimension(int d){
	dimension = d;
}

class POINT {
public:
	friend POINT log(const POINT&b);
	friend double get_distance(const POINT&, const POINT&);
	POINT(int _num = 2):num(_num) {
		if (_num < 0) {
			cerr << "the dimension from input is negative!!!" << std::endl;
			exit(1);
		}
		v = new double[num];
		memset(v, 0, sizeof(double)*num);
	}
	POINT(double *_v, int _num) :num(_num) {
		v = new double[num];
		memcpy(v, _v, sizeof(double)*num);
	}
	POINT(const POINT&b):num(b.num) {
		v = new double[num];
		memcpy(v, b.v, sizeof(double)*num);
	}
	POINT(POINT&&b):num(b.num) {
		v = b.v;

		b.num = 0;
		b.v = nullptr;
	}
	POINT& operator=(const POINT&b) {
		if (this == &b) {
			return *this;
		}
		delete[]v;
		num = b.num;
		v = new double[num];
		memcpy(v, b.v, sizeof(double)*num);
		return *this;
	}
	POINT& operator=(POINT &&b) {
		if (this == &b)
			return *this;
		num = b.num;
		delete[]v;
		v = b.v;

		b.v = nullptr;
		b.num = 0;
		return *this;
	}

	POINT& operator /=(double b) {
		for (int i = 0; i < num; ++i) {
			v[i] /= b;
		}
		return *this;
	}
	POINT& operator +=(double b) {
		for (int i = 0; i < num; ++i) {
			v[i] += b;
		}
		return *this;
	}
	POINT& operator -=(double b) {
		for (int i = 0; i < num; ++i) {
			v[i] -= b;
		}
		return *this;
	}
	POINT& operator *=(double b) {
		for (int i = 0; i < num; ++i) {
			v[i] *= b;
		}
		return *this;
	}
	POINT& operator /=(const POINT& b) {
		if (b.num != num) {
			std::cerr << "dimension error!!!" << std::endl;
			exit(1);
		}
		for (int i = 0; i < num; ++i) {
			v[i] /= b[i];
		}
		return *this;
	}
	POINT& operator *=(const POINT& b) {
		if (b.num != num) {
			std::cerr << "dimension error!!!" << std::endl;
			exit(1);
		}
		for (int i = 0; i < num; ++i) {
			v[i] *= b[i];
		}
		return *this;
	}
	POINT& operator +=(const POINT& b) {
		if (b.num != num) {
			std::cerr << "dimension error!!!" << std::endl;
			exit(1);
		}
		for (int i = 0; i < num; ++i) {
			v[i] += b[i];
		}
		return *this;
	}
	POINT& operator -=(const POINT& b) {
		if (b.num != num) {
			std::cerr << "dimension error!!!" << std::endl;
			exit(1);
		}
		for (int i = 0; i < num; ++i) {
			v[i] -= b[i];
		}
		return *this;
	}

	POINT operator /(double b) {
		POINT temp(*this);
		for (int i = 0; i < num; ++i) {
			temp.v[i] /= b;
		}
		return temp;
	}
	POINT operator +(double b) {
		POINT temp(*this);
		for (int i = 0; i < num; ++i) {
			temp.v[i] += b;
		}
		return temp;
	}
	POINT operator -(double b) {
		POINT temp(*this);
		for (int i = 0; i < num; ++i) {
			temp.v[i] -= b;
		}
		return temp;
	}
	POINT operator *(double b) {
		POINT temp(*this);
		for (int i = 0; i < num; ++i) {
			temp.v[i] *= b;
		}
		return temp;
	}
	POINT operator /(const POINT& b) {
		if (b.num != num) {
			std::cerr << "dimension error!!!" << std::endl;
			exit(1);
		}
		POINT temp(*this);
		for (int i = 0; i < num; ++i) {
			temp.v[i] /= b[i];
		}
		return temp;
	}
	POINT operator *(const POINT& b) {
		if (b.num != num) {
			std::cerr << "dimension error!!!" << std::endl;
			exit(1);
		}
		POINT temp(*this);
		for (int i = 0; i < num; ++i) {
			temp.v[i] *= b[i];
		}
		return temp;
	}
	POINT operator +(const POINT& b) {
		if (b.num != num) {
			std::cerr << "dimension error!!!" << std::endl;
			exit(1);
		}
		POINT temp(*this);
		for (int i = 0; i < num; ++i) {
			temp.v[i] += b[i];
		}
		return temp;
	}
	POINT operator -(const POINT& b) {
		if (b.num != num) {
			std::cerr << "dimension error!!!" << std::endl;
			exit(1);
		}
		POINT temp(*this);
		for (int i = 0; i < num; ++i) {
			temp.v[i] -= b[i];
		}
		return temp;
	}


	bool operator==(const POINT&b)const {
		if (num != b.num) {
			std::cerr << "dimemsion error!!!"<<std::endl;
		}
		for (int i = 0; i < num; ++i) {
			if (v[i] != b[i]) {
				return false;
			}
		}
		return true;
	}
	bool operator!=(const POINT&b)const {
		if (num != b.num) {
			std::cerr << "dimemsion error!!!" << std::endl;
		}
		for (int i = 0; i < num; ++i) {
			if (v[i] == b[i]) {
				return false;
			}
		}
		return true;
	}
	const double& operator[](int i) const {
		return v[i];
	}
	double&	operator[](int i) {
		return v[i];
	}

	~POINT() {
		delete[]v;
	}
	size_t size() const{ return num;}
private:
	int num;
	double *v;
};

POINT log(const POINT&b) {
	POINT temp(b);
	for (int i = 0; i < temp.num; ++i) {
		temp.v[i] = log(temp.v[i]);
	}
	return temp;
}
double get_distance(const POINT&a, const POINT&b) {
	double *dv=new double[a.size()];
	int num = a.size();
	for (int i = 0; i < num; ++i) {
		dv[i] = a[i] - b[i];
	}
	double dis = 0;
	for (int i = 0; i < num; ++i) {
		dis += dv[i] * dv[i];
	}
	delete[]dv;
	return dis;
}
class Cluster {
public:
	Cluster(int _num=dimension):num(0),all_point(_num){}
	Cluster(double *_v, int _num):num(1),all_point(_v,_num) {}
	Cluster(const POINT& p) :num(1),all_point(p){}
	Cluster(POINT&& p) :num(1),all_point(p){}
	void add(const POINT& p) {
		all_point += p;
		++num;
	}
	POINT get_center() {
		return all_point/num;
	}
	int num;
private:
	POINT all_point;
};

class P_C {
public:
	P_C() :id(-1), dis(0), label(-1) {}
	P_C(const P_C&b) :id(b.id), p(b.p), dis(b.dis), label(b.label) {}
	P_C(int _id, const POINT &_p, int _label) :id(_id), p(_p), dis(0), label(_label) {}
	P_C(int _id, const POINT &_p, int _dis, int _label) :id(_id), p(_p), dis(_dis), label(_label) {}
	P_C& operator=(const P_C&b) {
		if (this == &b) {
			return *this;
		}
		p = b.p;
		dis = b.dis;
		label = b.label;
		id = b.id;
		return *this;
	}
	POINT p;
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



vector<int> get_seed(int, int);


bool check(const POINT& a,const POINT& b,const std::vector<double>& dspan,int multiple=1)
{
	if (a.size() != b.size() || a.size() != dspan.size()) {
		cerr << "dimension error in check!!!" << endl;
		exit(1);
	}
	for (int i = 0; i < a.size(); ++i) {
		if (a[i]<b[i] - dspan[i]*multiple || a[i]>b[i] + dspan[i]*multiple) {
			return false;
		}
	}
	return true;
}



void Adaptive_K_means(vector<int>&label, const vector<POINT>&p, int k, int scope,const int dimension=2) {
	const static Scalar Color[] = { CV_RGB(0,0,0),CV_RGB(0,0,130) ,CV_RGB(0,130,0) ,CV_RGB(130,0,0) ,CV_RGB(150,150,0)
	,CV_RGB(0,150,150) ,CV_RGB(150,0,150) ,CV_RGB(120,130,150) ,CV_RGB(130,120,150) ,CV_RGB(170,130,130),
	CV_RGB(200,30,40), CV_RGB(40,30,200), CV_RGB(70,230,0), CV_RGB(0,70,230), CV_RGB(250,30,250), };
	std::vector<double> dmax(dimension),dmin(dimension);
	for (int i = 0; i < dimension; ++i) {
		dmax[i] = -INF;
		dmin[i] = INF;
	}
	for (int i = 0; i < p.size(); ++i) {
		for (int j = 0; j < dimension; ++j) {
			dmax[j] = max(dmax[j], p[i][j]);
			dmin[j] = min(dmin[j], p[i][j]);
		}
	}

	std::vector<double> DSPAN(dimension);
	for (int i = 0; i < dimension; ++i) {
		DSPAN[i] = max(5.0, (double)(dmax[i] - dmin[i]) / k / 5);
	}

	//sort(p.begin(), p.end(), comp);
	label.resize(p.size());
	for (int i = 0; i < label.size(); ++i)
		label[i] = -1;
	std::vector<int> seed = get_seed(k, p.size());
	priority_queue<P_C, vector<P_C>, Compare> q;
	std::vector<Cluster> cluster;
	vector<Cluster> rcen;
	vector<vector<double>> dspan;
	rcen.resize(seed.size());
	for (int i = 0; i < seed.size(); ++i) {
		POINT temp = p[seed[i]];
		temp= log(temp)*scope;
		cluster.push_back(temp);
		q.push(P_C(seed[i], p[seed[i]], i));
		dspan.push_back(DSPAN);
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
			POINT center = cluster[k].get_center();
			POINT rcenter = rcen[k].get_center();
			int number = 0;
			do {
				number = 0;
				for (int i = 0; i < p.size(); ++i) {
					if (-1 == label[i] && check(p[i], rcenter,dspan[k])) {
						POINT b = p[i];
						double dis = get_distance(center,b);
						q.push(P_C(i, b, dis, k));
						++number;
					}
				}
				for (int i = 0; i < dimension; ++i) {
					dspan[k][i] += DSPAN[i];
				}
			} while (0 == number && num < p.size());
		}
	}

	for (int i = 0; i < p.size(); ++i) {
		if (10 > cluster[label[i]].num) {
			for (int j = 0; j < p.size(); ++j) {
				if (label[i] != label[j] && 10 <= cluster[label[j]].num&&check(p[j], p[i], DSPAN, 10)) {
					--cluster[label[i]].num;
					++cluster[label[j]].num;
					label[i] = label[j];
					break;
				}
			}
		}
	}
	if (2 == dimension) {
		Mat image(1000, 1000, CV_8UC3, CV_RGB(255, 255, 255));
		namedWindow("SHOW");
		imshow("SHOW", image);

		int u = 0;
		map<int, int> ltoi;
		for (int j = 0; j < p.size(); ++j) {
			//cout << label[j] << " ";
			if (ltoi.end() == ltoi.find(label[j]))
				ltoi[label[j]] = u++;
			Point t(p[j][0], p[j][1]);
			circle(image, t, 4, Color[ltoi[label[j]]], -1);
			imshow("SHOW", image);
			waitKey(10);
		}
		waitKey(0);
		destroyWindow("SHOW");
	}
}

int get_accuracy(int k,const vector<int> origin, vector<int> label) {
	int s = 0;
	int e = 1;
	int num = 0;
	std::map<int, bool> sign;
	struct LABEL{
		int label;
		int num;
		LABEL():label(-1),num(0) {}
		bool operator<(const LABEL&L)const { return num > L.num; }
		bool operator==(const LABEL&L) const{ return num == L.num; }
		int operator++() { return ++num; }
		void set_label(int _label) { label = _label; }
	};
	std::vector<LABEL> label_num(k+1);
	while (e <= origin.size()) {
		if (e==origin.size()||origin[e] != origin[e-1]) {
			label_num.clear();
			label_num.resize(k + 1);
			for (int i = 0; i <= k; ++i)
				label_num[i].label = i;
			for (int i = s; i < e; ++i) {
				++label_num[label[i]];
			}
			sort(label_num.begin(), label_num.end());
			std::vector<LABEL>::iterator it = label_num.begin();
			do {
				if (sign.find(it->label) == sign.end()) {
					if(it==label_num.begin())
						sign[it->label] = true;
					break;
				}
				++it;
			} while (it!=label_num.end());
			if (it == label_num.end()) {
				num += e - s;
				s = e;
				++e;
				continue;
			}
			int l = it->label;
			for (int i = s; i < e; ++i) {
				if (label[i] != l) {
					++num;
				}
			}
			s = e;
		}
		++e;
	}
	
	return num;
}

void transform(std::vector<POINT>& p, int range = 500) {
	std::vector<double> dmin(dimension), dmax(dimension);
	for (int i = 0; i < dimension; ++i) {
		dmin[i] = INF;
		dmax[i] = -INF;
	}
	for (int i = 0; i < p.size(); ++i) {
		for (int j = 0; j < dimension; ++j) {
			dmin[j] = min(dmin[j], p[i][j]);
			dmax[j] = max(dmax[j], p[i][j]);
		}
	}
	for (int i = 0; i < p.size(); ++i) {
		for (int j = 0; j < dimension; ++j) {
			p[i][j] = (p[i][j] - dmin[j]) / (dmax[j] - dmin[j])*range + 100;
		}
	}
}

int main()
{
	ifstream in("./data06.txt");
	vector<POINT> p;
	vector<int>label;
	vector<int> origin;
	vector<int> origin_center;
	int l = 0;
	int tl;
	double x;
	double y;
	int n = 0;
	int dimension;
	in >> dimension;
	set_dimension(dimension);
	double *v = new double[dimension];

	while (in>>v[0]) {
		for (int i = 1; i < dimension; ++i) {
			in >> v[i];
		}
		in >> tl;
		POINT t(v,dimension);
		p.push_back(t);
		origin.push_back(tl);
		++n;
	}
	transform(p);
	/*Mat image(1000, 1000, CV_8UC3, CV_RGB(255, 255, 255));
	namedWindow("SHOW");
	imshow("SHOW", image);
	const static Scalar Color[] = { CV_RGB(0,0,0),CV_RGB(0,0,130) ,CV_RGB(0,130,0) ,CV_RGB(130,0,0) ,CV_RGB(150,150,0)
	,CV_RGB(0,150,150) ,CV_RGB(150,0,150) ,CV_RGB(120,130,150) ,CV_RGB(130,120,150) ,CV_RGB(170,130,130),
	CV_RGB(200,30,40), CV_RGB(40,30,200), CV_RGB(70,230,0), CV_RGB(0,70,230), CV_RGB(250,30,250), };
	for (int j = 0; j < p.size(); ++j) {
		Point t(p[j][0], p[j][1]);
		circle(image, t, 4, Color[origin[j]], -1);
		imshow("SHOW", image);
		waitKey(10);
	}
	waitKey(0);
	destroyWindow("SHOW");*/

	in.close();
	delete[]v;

	transform(p);
	int k = p.size()/20;
	k = 0 == k ? 5 : k;
	Adaptive_K_means(label, p, k,9,dimension);
	std::cout << (n-get_accuracy(k,origin, label))/(double)n << endl;
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