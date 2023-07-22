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
        int index;      //�����s��
        int X;          //����X�y��
        int Y;          //����Y�y��
};

Point::Point(int i, int x, int y):index(i), X(x), Y(y){};

bool Point::operator<(Point& point){    //�����ӫ������s���j�p
    return index < point.index;
}

double Point::operator-(Point& point){  //�^�Ǩ�ӫ����������Z��
    return sqrt(pow(X - point.X, 2) + pow(Y - point.Y, 2));
}

int main(int argc, char **argv){
    ifstream fin(string(argv[1])+"/point.txt");
    vector<Point> Points;   //Points:�x�s������T��vector
    int index, x, y;
    if(fin){
        while(fin >> index >> x >> y){  //�q�ɮ�Ū�J�C�y�������s���BX�y�ЩMY�y��
            Points.push_back(Point(index, x, y));
        }
    }
    fin.close();

    int bestOrder[Points.size()];   //bestOrder:�x�s�̨θѫ����s������
    double dis, bestDis=0;          //dis:��eiteration���Z��, bestDis:�̨θѪ��Z��
    do{
        dis=Points[Points.size()-1]-Points[0];  //�[�J�q�̫�@���I����Ĥ@���I���Z��
        for(int i=1; i<Points.size(); i++){     //�̧ǥ[�J�q��n���I�����n+1���I���Z��
            dis+=Points[i]-Points[i-1];
        }
        if(dis<bestDis || !bestDis){            //�Y��eiteration���Z����ثe�̨θѪ��Z���u
            bestDis=dis;                        //�N�̨θѪ��Z���]����eiteration���Z��
            for(int i=0; i<Points.size(); i++){ //�̨θѫ����s�����ǳ]����eiteration������
                bestOrder[i]=Points[i].index;
            }
        }
    }while(next_permutation(Points.begin()+1, Points.end()));   //���o�U�@��iteration

    ofstream fout(string(argv[1])+"/ans_"+string(argv[1])+".txt");
    fout << "distance: " << bestDis << endl;    //�Ӵ��ո�Ƨ�쪺�̵u���׿�X���ɮײĤ@��
    for(int i=0; i<Points.size(); i++){         //����ӧ�쪺�̨θѨ̧ǿ�X�����s����X���ɮ�
        fout << bestOrder[i] << endl;
    }
    fout.close();
    cout << "ans_" << string(argv[1]) << ".txt created" << endl;

    return 0;
}
