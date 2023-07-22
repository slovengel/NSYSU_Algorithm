#include <iostream>
#include <algorithm>
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

    int order[Points.size()], bestOrder[Points.size()], candidate;   //order:當前iteration城市編號順序, bestOrder:最佳解城市編號順序, candidate:暫存目前最佳的下一座城市
    double dis, sumDis=0, bestDis=0;                //dis:暫存兩點之間距離, sumDis:距離總和, bestDis:最佳解的距離
    for(int k=0; k<Points.size(); k++, sumDis=0){   //依序將每一座城市當作greedy的起始點(以k為第一點做greedy)
        bool traveled[Points.size()]={};            //traveled:記錄每座城市是否有被走過
        order[0]=k;                                 //走過第一座城市
        traveled[k]=true;
        for(int i=0; i<Points.size()-1; i++){       //走過剩下的城市
            bool first=true;                        //first:是否進入if迴圈(dis需有初始值)
            for(int j=0; j<Points.size(); j++){     //還沒走過的城市中，判斷與哪一個與現在走到的城市距離最短
                if(!traveled[j] && (first || Points[order[i]]-Points[j]<dis)){
                    dis=Points[order[i]]-Points[j];
                    candidate=j;
                    first=false;
                }
            }
            sumDis+=dis;                            //將當前iteration最佳的下一座城市距離加入距離總和
            order[i+1]=candidate;                   //將當前iteration最佳的下一座城市加入最佳解城市編號順序
            traveled[candidate]=true;
        }
        sumDis+=Points[order[Points.size()-1]]-Points[order[0]];    //加入從最後一個點走到第一個點的距離
        if(sumDis<bestDis || !k){                   //若當前iteration的距離比目前最佳解的距離短
            bestDis=sumDis;                         //將最佳解的距離設為當前iteration的距離
            for(int i=0; i<Points.size(); i++){     //最佳解城市編號順序設為當前iteration的順序
                bestOrder[i]=order[i];
            }
        }
    }

    ofstream fout(string(argv[1])+"/ans_"+string(argv[1])+".txt");
    fout << "distance: " << bestDis << endl;    //該測試資料找到的最短長度輸出到檔案第一行
    for(int i=0; i<Points.size(); i++){         //後續照找到的最佳解依序輸出城市編號輸出到檔案
        fout << Points[bestOrder[i]].index << endl;
    }
    fout.close();
    cout << "ans_" << string(argv[1]) << ".txt created" << endl;

    return 0;
}
