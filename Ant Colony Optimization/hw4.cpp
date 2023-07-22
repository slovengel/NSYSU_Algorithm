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
        int index;      //�����s��
        int X;          //����X�y��
        int Y;          //����Y�y��
};

Point::Point(int i, int x, int y):index(i), X(x), Y(y){};

double Point::operator-(Point& point){  //�^�Ǩ�ӫ����������Z��
    return sqrt(pow(X - point.X, 2) + pow(Y - point.Y, 2));
}

vector<Point> Points;   //Points:�x�s������T��vector
double **dist, **ph;    //dist:�x�s��ӫ����������Z��, ph:�x�s��ӫ����������O���X

class Ant{
    public:
        Ant();
        void walkCycle();
        int currPoint;              //�ثe�Ҧb����
        int visitedOrder[MAXCITY];  //���X����, E.g. visitedOrder[3]: �s��4�ӫ��X��������ID
        vector<int> unvisited;      //�����X�������M��
        double distSum;             //�樫�Z��
};

Ant::Ant(){
    for(int i = 0; i < Points.size(); i++){     //�Ҧ������������X
        unvisited.push_back(i);
    }
    uniform_int_distribution<int> dir(0, Points.size()-1);
    currPoint = unvisited[dir(gen)];    //�H����ܤ@�y�����@���X�o�I
    for(int i = 0; i < unvisited.size(); i++){
        if(unvisited[i] == currPoint){
            unvisited.erase(unvisited.begin() + i); //�N�X�o�I�����q�����X�������M�椤����
            break;
        }
    }
    visitedOrder[0] = currPoint;    //�N�X�o�I�����[�J���X����
};

void Ant::walkCycle(){
    for(int i = 1; i < Points.size(); i++){     //�����ƨ��L�Ҧ�����
        double totalPh = 0;
        for(int j = 0; j < unvisited.size(); j++){      //totalPh:���v�`�M = sum(�ثe�����쥼���X�������O���X/�ثe�����쥼���X�������Z��)
            totalPh += ph[currPoint][unvisited[j]]/dist[currPoint][unvisited[j]];
        }
        uniform_real_distribution<double> dir(0.0, totalPh);
        double randVal = dir(gen), tempSum=ph[currPoint][unvisited[0]]/dist[currPoint][unvisited[0]];   //randVal:0~totalPh�������H����
        for(int j = 1; j < unvisited.size()+1; j++){                                                    //tempSum:��0�y���j-1�y�����X���������v�[�`
            if(randVal < tempSum){      //�H���ȿ���j�y�����X�������@���U�@�y����
                currPoint = unvisited[j-1];     //����U�@�y����
                for(int k = 0; k < unvisited.size(); k++){
                    if(unvisited[k] == currPoint){
                        unvisited.erase(unvisited.begin() + k);     //�N��j�y�����X�������q�����X�������M�椤����
                        break;
                    }
                }
                visitedOrder[i] = currPoint;    //�N�U�@�y�����[�J���X����
                break;
            }else{
                tempSum += ph[currPoint][unvisited[j]]/dist[currPoint][unvisited[j]];
            }
        }
    }
    distSum = dist[visitedOrder[Points.size()-1]][visitedOrder[0]];     //�p��樫�Z��
    for(int i = 0; i < Points.size()-1; i++){
        distSum += dist[visitedOrder[i]][visitedOrder[i+1]];
    }
}

Ant findBestAnt(Ant ant[], int numofAnt){
    int bestAnt = 0;
    for(int i = 1; i < numofAnt; i++){
        if(ant[i].distSum < ant[bestAnt].distSum){  //���樫�Z���̵u������
            bestAnt=i;
        }
    }
    return ant[bestAnt];
}

void phUpdate(Ant ant){
    for(int i = 0; i < Points.size(); i++){
        for(int j = 0; j < Points.size(); j++){
            ph[i][j] *= 0.9;        //�N����y�����������O���X���H0.9
        }
    }
    ph[ant.visitedOrder[Points.size()-1]][ant.visitedOrder[0]] += 1/ant.distSum;    //�N���ƩҸg�L���|���O���X�[�W���Ʀ樫�Z�����˼�
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
                ph[i][j] = (i == j? 0:1/dist[i][j]);    //��l�ƶO���X���Z���˼�
            }
        }
        for(int i = 0; i < iter; i++){
            Ant ant[numofAnt];
            for(int a = 0; a < numofAnt; a++){
                ant[a].walkCycle();                     //���C�@�����ƨ��@��(�g�L�Ҧ��I�^����I)
            }
            if(i<iter-1){
                phUpdate(findBestAnt(ant, numofAnt));   //�H�o��iteration���̨ο��ƪ����|��s�O���X
            }else{
                bestAnt=findBestAnt(ant, numofAnt);     //�H��30��iteration���̨ο��ƪ��樫�Z���@���o�@run�����G
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
