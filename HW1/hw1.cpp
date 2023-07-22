#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

class Point{
    public:
        Point(int, int, int);
        bool operator<(Point&);
        double operator-(Point&);
        int index;      //城市編號
        int X;          //城市X座標
        int Y;          //城市Y座標
};

Point::Point(int i, int x, int y):index(i), X(x), Y(y){};

bool Point::operator<(Point& point){    //比較兩個城市的編號大小
    return index < point.index;
}

double Point::operator-(Point& point){  //回傳兩個城市之間的距離
    return sqrt(pow(X - point.X, 2) + pow(Y - point.Y, 2));
}

int main(int argc, char **argv){
    ifstream fin(string(argv[1])+"/point.txt");
    vector<Point> Points;   //Points:儲存城市資訊的vector
    int index, x, y;
    if(fin){
        while(fin >> index >> x >> y){  //從檔案讀入每座城市的編號、X座標和Y座標
            Points.push_back(Point(index, x, y));
        }
    }
    fin.close();

    int bestOrder[Points.size()];   //bestOrder:儲存最佳解城市編號順序
    double dis, bestDis=0;          //dis:當前iteration的距離, bestDis:最佳解的距離
    do{
        dis=Points[Points.size()-1]-Points[0];  //加入從最後一個點走到第一個點的距離
        for(int i=1; i<Points.size(); i++){     //依序加入從第n個點走到第n+1個點的距離
            dis+=Points[i]-Points[i-1];
        }
        if(dis<bestDis || !bestDis){            //若當前iteration的距離比目前最佳解的距離短
            bestDis=dis;                        //將最佳解的距離設為當前iteration的距離
            for(int i=0; i<Points.size(); i++){ //最佳解城市編號順序設為當前iteration的順序
                bestOrder[i]=Points[i].index;
            }
        }
    }while(next_permutation(Points.begin()+1, Points.end()));   //取得下一個iteration

    ofstream fout(string(argv[1])+"/ans_"+string(argv[1])+".txt");
    fout << "distance: " << bestDis << endl;    //該測試資料找到的最短長度輸出到檔案第一行
    for(int i=0; i<Points.size(); i++){         //後續照找到的最佳解依序輸出城市編號輸出到檔案
        fout << bestOrder[i] << endl;
    }
    fout.close();
    cout << "ans_" << string(argv[1]) << ".txt created" << endl;

    return 0;
}
