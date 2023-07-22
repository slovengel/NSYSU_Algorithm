#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

class Point{
    public:
        Point(int, int, int);
        double operator-(Point&);
        int index;      //�����s��
        int X;          //����X�y��
        int Y;          //����Y�y��
};

Point::Point(int i, int x, int y):index(i), X(x), Y(y){};

double Point::operator-(Point& point){  //�^�Ǩ�ӫ����������Z��
    return sqrt(pow(X - point.X, 2) + pow(Y - point.Y, 2));
}

vector<Point> Points;   //Points:�x�s������T��vector
double **dist, **DP;    //dist:�x�s��ӫ����������Z��, DP:�x�sTSP(int, int)�����G

double TSP(int visiting, int status){   //visiting:�ثe�Ҧb����, status:�ثe�h�L���ǫ���(���p��0�y����)
    if(!status){    //����0�y�����H�~�A�ثe�S���h�L���󫰥�
        if(DP[visiting][status] == 0){  //�|�����
            DP[visiting][status] = dist[visiting][0];   //TSP(visiting, 0)=dist(�ثe�Ҧb����, ��0�y����)
        }
    }
    if(DP[visiting][status]==0){        //�|�����
        for(int i=1; i<Points.size(); i++){     //i:�̧ǱN�Ҧ������]���W�@�ӫ���
            if(status & (1<<(i-1))){            //�Y����i���ثe�h�L������
                if(DP[visiting][status] == 0 || DP[visiting][status] > TSP(i, status - (1<<(i-1))) + dist[i][visiting]){
                    DP[visiting][status] = TSP(i, status - (1<<(i-1))) + dist[i][visiting];
                }     //TSP(visiting, status)=min{TSP(�W�@�ӫ���, �ثe�h�L������-�W�@�ӫ���) + dist(�ثe�Ҧb����, �W�@�ӫ���)}
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

    int status = (1<<(Points.size()-1))-1, order[Points.size()];    //status:�h�L�Ҧ�����, order:�x�s�̨θѫ���ID����
    double bestDis= TSP(0, status); //�D�q��0�y�����X�o�A�g�L���������A�A�^���0�y�������̵u�Z��

    order[0] = 0;   //�q��0�y�����X�o(��0�y�����g�L�����ǬO0)
    for(int i=1; i<Points.size(); i++){     //i:�g�L�����ǬOi
        for(int j=1; j<Points.size(); j++){ //j:��j�y����
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
