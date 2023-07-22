#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

class Point{
    public:
        Point(int, int, int);
        double operator-(Point&);
        int index;      //城市編號
        int X;          //城市X座標
        int Y;          //城市Y座標
};

Point::Point(int i, int x, int y):index(i), X(x), Y(y){};

double Point::operator-(Point& point){  //回傳兩個城市之間的距離
    return sqrt(pow(X - point.X, 2) + pow(Y - point.Y, 2));
}

vector<Point> Points;   //Points:儲存城市資訊的vector
double **dist, **DP;    //dist:儲存兩個城市之間的距離, DP:儲存TSP(int, int)的結果

double TSP(int visiting, int status){   //visiting:目前所在城市, status:目前去過哪些城市(不計第0座城市)
    if(!status){    //除第0座城市以外，目前沒有去過任何城市
        if(DP[visiting][status] == 0){  //尚未填表
            DP[visiting][status] = dist[visiting][0];   //TSP(visiting, 0)=dist(目前所在城市, 第0座城市)
        }
    }
    if(DP[visiting][status]==0){        //尚未填表
        for(int i=1; i<Points.size(); i++){     //i:依序將所有城市設為上一個城市
            if(status & (1<<(i-1))){            //若城市i為目前去過的城市
                if(DP[visiting][status] == 0 || DP[visiting][status] > TSP(i, status - (1<<(i-1))) + dist[i][visiting]){
                    DP[visiting][status] = TSP(i, status - (1<<(i-1))) + dist[i][visiting];
                }     //TSP(visiting, status)=min{TSP(上一個城市, 目前去過的城市-上一個城市) + dist(目前所在城市, 上一個城市)}
            }
        }
    }
    return DP[visiting][status];
}

int main(int argc, char **argv){
    ifstream fin(string(argv[1])+"/point.txt");

    int index, x, y;
    if(fin){
        while(fin >> index >> x >> y){
            Points.push_back(Point(index, x, y));
        }
    }
    fin.close();

    DP = new double* [Points.size()];
    dist = new double* [Points.size()];
    for(int i=0; i<Points.size(); i++){
        DP[i] = new double [1<<(Points.size()-1)];
        dist[i] = new double[Points.size()];
        for(int j=0; j<1<<(Points.size()-1); j++){
            DP[i][j] = 0;
        }
        for(int j = 0; j < Points.size(); j++){
        	dist[i][j] = (Points[i] - Points[j]);
        }
    }

    int status = (1<<(Points.size()-1))-1, order[Points.size()];    //status:去過所有城市, order:儲存最佳解城市ID順序
    double bestDis= TSP(0, status); //求從第0座城市出發，經過全部城市，再回到第0座城市的最短距離

    order[0] = 0;   //從第0座城市出發(第0座城市經過的順序是0)
    for(int i=1; i<Points.size(); i++){     //i:經過的順序是i
        for(int j=1; j<Points.size(); j++){ //j:第j座城市
            if(DP[j][status - (1<<(j-1))] + dist[j][order[i-1]] == DP[order[i-1]][status]){ //back trace
                order[i]=j;
                status-=(1<<(j-1));
                break;
            }
        }
    }

    ofstream fout(string(argv[1])+"/ans_"+string(argv[1])+".txt");
    fout << "distance: " << bestDis << endl;
    for(int i=0; i<Points.size(); i++){
        fout << Points[order[i]].index << endl;
    }
    fout.close();
    cout << "ans_" << string(argv[1]) << ".txt created" << endl;

	for(int i = 0; i < Points.size(); i++){
        delete DP[i];
        delete dist[i];
	}
	delete DP;
	delete dist;

    return 0;
}
