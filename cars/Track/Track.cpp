#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include "TrackGen.h"

using namespace std;

vector<int> test(0);

class track
{
public:
	track (char *filename);
	double w, h, dt, dvx, dy, r;
	int n;
	vector<double> x;
	vector<double> y;
};
track::track (char *filename = "parametres.txt")
{	
	ifstream F (filename);
	F >> track::w >> track::h >> track::dt >> track::dvx >> track::dy >> track::r >> track::n;
	track::x.resize(n);
	track::y.resize(n);
	for (int i = 0; i < n; i++)
		F >> track::x[i] >> track::y[i];
}

class route
{
public:
	route(vector<int> moveX, vector<int> moveY);
	vector<pair<int, int>> dxy;
	
};
route::route(vector<int> moveX = test, vector<int> moveY = test)
{
	route::dxy.resize(moveX.size());
	for (int i = 0; i < moveX.size(); i++)
	{
		route::dxy[i].first = moveX[i];
		route::dxy[i].second = moveY[i];
	}
}

class model
{
public:
	model (char *, track, route);
	double t;
	track NowTrack;
	route Route;
	int hCar, wCar;
	double xCar, yCar, vCar;
	bool live;
	bool CheckMines (int num);
	bool CheckDieOrNot();
	void ApplySingleMove (int a);
};
model::model (char *filename, track StartPos, route Rroute){
	ifstream F (filename);
	F >> model::hCar >> model::wCar;
	model::xCar = model::wCar / 2;
	model::yCar = StartPos.h / 2;
	model::vCar = 0;
	model::t = 0;
	model::NowTrack = StartPos;
	model::Route = Rroute;
	model::live = true;
}
	bool model::CheckMines (int num){
		bool flag = true;
		double a, b, c;
		if ((xCar-wCar/2 <= NowTrack.x[num]) && ( NowTrack.x[num] <= xCar+wCar/2) && (yCar-hCar/2 <= NowTrack.y[num]) && (yCar+hCar/2 >= NowTrack.y[num])) 
			flag = false;
		else {
			if (NowTrack.y[num] > yCar+hCar/2) {
				if (NowTrack.x[num] < xCar-wCar/2) {
					a = NowTrack.y[num]-yCar-hCar/2;
					b = xCar-wCar/2-NowTrack.x[num];
					c = sqrt(a*a+b*b);
					if (c <= r) flag = false;
				}
				else if  ((NowTrack.x[num] <= xCar+wCar/2) && (NowTrack.y[num]-yCar-hCar/2 <= r)) flag = false;
				else {
					a = NowTrack.y[num]-hCar/2-yCar;
					b = NowTrack.x[num]-wCar/2-xCar;
					c = sqrt(a*a+b*b);
					if (c <= r) flag = false;
				}
			}
			if ((NowTrack.y[num] >= yCar-hCar/2) && (NowTrack.y[num] <= yCar+hCar/2)) {
				if ((NowTrack.x[num] < xCar-wCar/2) && (xCar-wCar/2-NowTrack.x[num] <= r)) flag = false;
				if ((NowTrack.x[num] > xCar+wCar/2) && (NowTrack.x[num]-xCar-wCar/2 <= r)) flag = false;
			} 
			if (NowTrack.y[num] < yCar-hCar/2) {
				if (NowTrack.x[num] < xCar-wCar/2) {
					a = yCar-hCar/2-NowTrack.y[num];
					b = xCar-wCar/2-NowTrack.x[num];
					c = sqrt(a*a+b*b);
					if (c <= r) flag = false;
				}
				else if  ((NowTrack.x[num] <= xCar+wCar/2) && (yCar-hCar/2-NowTrack.y[num] <= r)) flag = false;
				else {
					a = yCar-hCar/2-NowTrack.y[num];
					b = NowTrack.x[num]-wCar/2-xCar;
					c = sqrt(a*a+b*b);
					if (c <= r) flag = false;
				} 
			}
		}
		return (flag);
	}
	bool model::CheckDieOrNot(){
		int i = 0;
		while ((live == true) && (i < NowTrack.n)){
			if (CheckMines(i) == false) live = false;
			i++;
		}
		if (live == true){
			if (yCar + hCar/2 >= h) live = false;
			else if (yCar - hCar/2 <= 0) live = false;
		}
		if (xCar >= NowTrack.w-wCar/2) live = false;
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!if (yCar
		return (live);
	}
	void model::ApplySingleMove (int a){
		yCar += (NowTrack.dy * Route.dxy[a].second);
		xCar += (vCar * NowTrack.dt);
		vCar += (NowTrack.dvx * NowTrack.dt * Route.dxy[a].first);
		t += NowTrack.dt;
		CheckDieOrNot();
		return;
	}

int main()
{
	cout << "Create a new track?" << endl;
	int ans;
	cin >> ans;
	if (ans == 1)
		coordinates();

	vector<int> SimpleRouteX(30);
	for (int i = 0; i < 30; i++) SimpleRouteX[i] = 1;
	vector<int> SimpleRouteY(30, 0);

	route SimpleRoute(SimpleRouteX, SimpleRouteY);
	track Track("parametres.txt");
	model Model("car.txt", Track, SimpleRoute);

	int countSteps = 0;
	while (Model.live)
	{
		Model.ApplySingleMove(countSteps);
		countSteps++;
	}
	cout << countSteps << endl;
	return 0;
}