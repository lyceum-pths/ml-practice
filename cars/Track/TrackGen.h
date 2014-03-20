#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

const double w = 300, h = 50, dt = 0.5, dvx = 5, dy = 5, r = 5; 

vector<pair<double, double>> coordinates()
{
	srand(time(NULL));

	int	n = rand() % int(w*h/(8*r*r));

	vector<pair<double, double>> xy (0);
	pair<double, double> tempxy;
	double l;
	bool fl, lfl;

	for (int i = 0; i < n; i++){
		fl = false;
		while (fl == false){
			lfl = true;
			tempxy.first = (double)(rand())/RAND_MAX*(w - 2*r) + r;
			tempxy.second = (double)(rand())/RAND_MAX*(h - 2*r) + r;
			for (int j = 0; j < xy.size(); j++){
				l = sqrt((xy[j].first - tempxy.first)*(xy[j].first - tempxy.first) + (xy[j].second - tempxy.second)*(xy[j].second - tempxy.second));
				if (l < 2*r){
					lfl = false;
					break;
				}
			}
			if (lfl == true){
				xy.push_back (tempxy);
				fl = true;
			}
		}
	}

	ofstream f ("parametres.txt");
	f << w << endl << h << endl << dt << endl << dvx << endl << dy << endl << r << endl << n << endl;
	for (int i = 0; i < n; i++)
		f << xy[i].first << " " << xy[i].second << endl;
	f.close();

	return (xy);
}