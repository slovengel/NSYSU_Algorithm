#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <random>
using namespace std;

#define MAXCITY 51

random_device rd;
mt19937 gen(rd());

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
double **dist, **ph;    //dist:儲存兩個城市之間的距離, ph:儲存兩個城市之間的費洛蒙

class Ant{
    public:
        Ant();
        void walkCycle();
        int currPoint;              //目前所在城市
        int visitedOrder[MAXCITY];  //走訪順序, E.g. visitedOrder[3]: 存第4個拜訪的城市的ID
        vector<int> unvisited;      //未拜訪的城市清單
        double distSum;             //行走距離
};

Ant::Ant(){
    for(int i = 0; i < Points.size(); i++){     //所有城市都未拜訪
        unvisited.push_back(i);
    }
    uniform_int_distribution<int> dir(0, Points.size()-1);
    currPoint = unvisited[dir(gen)];    //隨機選擇一座城市作為出發點
    for(int i = 0; i < unvisited.size(); i++){
        if(unvisited[i] == currPoint){
            unvisited.erase(unvisited.begin() + i); //將出發點城市從未拜訪的城市清單中移除
            break;
        }
    }
    visitedOrder[0] = currPoint;    //將出發點城市加入走訪順序
};

void Ant::walkCycle(){
    for(int i = 1; i < Points.size(); i++){     //讓螞蟻走過所有城市
        double totalPh = 0;
        for(int j = 0; j < unvisited.size(); j++){      //totalPh:機率總和 = sum(目前城市到未拜訪城市的費洛蒙/目前城市到未拜訪城市的距離)
            totalPh += ph[currPoint][unvisited[j]]/dist[currPoint][unvisited[j]];
        }
        uniform_real_distribution<double> dir(0.0, totalPh);
        double randVal = dir(gen), tempSum=ph[currPoint][unvisited[0]]/dist[currPoint][unvisited[0]];   //randVal:0~totalPh之間的隨機值
        for(int j = 1; j < unvisited.size()+1; j++){                                                    //tempSum:第0座到第j-1座未拜訪城市的機率加總
            if(randVal < tempSum){      //隨機值選到第j座未拜訪的城市作為下一座城市
                currPoint = unvisited[j-1];     //走到下一座城市
                for(int k = 0; k < unvisited.size(); k++){
                    if(unvisited[k] == currPoint){
                        unvisited.erase(unvisited.begin() + k);     //將第j座未拜訪的城市從未拜訪的城市清單中移除
                        break;
                    }
                }
                visitedOrder[i] = currPoint;    //將下一座城市加入走訪順序
                break;
            }else{
                tempSum += ph[currPoint][unvisited[j]]/dist[currPoint][unvisited[j]];
            }
        }
    }
    distSum = dist[visitedOrder[Points.size()-1]][visitedOrder[0]];     //計算行走距離
    for(int i = 0; i < Points.size()-1; i++){
        distSum += dist[visitedOrder[i]][visitedOrder[i+1]];
    }
}

Ant findBestAnt(Ant ant[], int numofAnt){
    int bestAnt = 0;
    for(int i = 1; i < numofAnt; i++){
        if(ant[i].distSum < ant[bestAnt].distSum){  //找到行走距離最短的螞蟻
            bestAnt=i;
        }
    }
    return ant[bestAnt];
}

void phUpdate(Ant ant){
    for(int i = 0; i < Points.size(); i++){
        for(int j = 0; j < Points.size(); j++){
            ph[i][j] *= 0.9;        //將任兩座城市之間的費洛蒙乘以0.9
        }
    }
    ph[ant.visitedOrder[Points.size()-1]][ant.visitedOrder[0]] += 1/ant.distSum;    //將螞蟻所經過路徑的費洛蒙加上螞蟻行走距離的倒數
    ph[ant.visitedOrder[ant.visitedOrder[0]]][Points.size()-1] += 1/ant.distSum;
    for(int i = 0; i < Points.size()-1; i++){
        ph[ant.visitedOrder[i]][ant.visitedOrder[i+1]] += 1/ant.distSum;
        ph[ant.visitedOrder[i+1]][ant.visitedOrder[i]] += 1/ant.distSum;
    }
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

    dist = new double* [Points.size()];
    for(int i=0; i<Points.size(); i++){
        dist[i] = new double[Points.size()];
        for(int j = 0; j < Points.size(); j++){
        	dist[i][j] = (Points[i] - Points[j]);
        }
    }

    int run=stoi(argv[2]), iter=stoi(argv[3]), numofAnt=stoi(argv[4]);
    Ant bestAnt;
    double runDistSum=0;
    for(int r = 0; r < run; r++){
        ph = new double* [Points.size()];
        for(int i = 0; i < Points.size(); i++){
            ph[i] = new double[Points.size()];
            for(int j = 0; j < Points.size(); j++){
                ph[i][j] = (i == j? 0:1/dist[i][j]);    //初始化費洛蒙為距離倒數
            }
        }
        for(int i = 0; i < iter; i++){
            Ant ant[numofAnt];
            for(int a = 0; a < numofAnt; a++){
                ant[a].walkCycle();                     //讓每一隻螞蟻走一圈(經過所有點回到原點)
            }
            if(i<iter-1){
                phUpdate(findBestAnt(ant, numofAnt));   //以這次iteration的最佳螞蟻的路徑更新費洛蒙
            }else{
                bestAnt=findBestAnt(ant, numofAnt);     //以第30次iteration的最佳螞蟻的行走距離作為這一run的結果
                runDistSum += bestAnt.distSum;
            }
        }
        for(int i = 0; i < Points.size(); i++){
            delete ph[i];
        }
        delete ph;
    }

    ofstream fout(string(argv[1])+"/ans_"+string(argv[1])+".txt");
    fout << "distance: " << runDistSum/run << endl;
    for(int i = 0; i < Points.size(); i++){
        fout << Points[bestAnt.visitedOrder[i]].index << endl;
    }
    fout.close();
    cout << "ans_" << string(argv[1]) << ".txt created" << endl;

	for(int i = 0; i < Points.size(); i++){
		delete dist[i];
	}
	delete dist;

    return 0;
}
