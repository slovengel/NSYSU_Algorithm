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
        int index;      //�����s��
        int X;          //����X�y��
        int Y;          //����Y�y��
};

Point::Point(int i, int x, int y):index(i), X(x), Y(y){};

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

    int order[Points.size()], bestOrder[Points.size()], candidate;   //order:��eiteration�����s������, bestOrder:�̨θѫ����s������, candidate:�Ȧs�ثe�̨Ϊ��U�@�y����
    double dis, sumDis=0, bestDis=0;                //dis:�Ȧs���I�����Z��, sumDis:�Z���`�M, bestDis:�̨θѪ��Z��
    for(int k=0; k<Points.size(); k++, sumDis=0){   //�̧ǱN�C�@�y������@greedy���_�l�I(�Hk���Ĥ@�I��greedy)
        bool traveled[Points.size()]={};            //traveled:�O���C�y�����O�_���Q���L
        order[0]=k;                                 //���L�Ĥ@�y����
        traveled[k]=true;
        for(int i=0; i<Points.size()-1; i++){       //���L�ѤU������
            bool first=true;                        //first:�O�_�i�Jif�j��(dis�ݦ���l��)
            for(int j=0; j<Points.size(); j++){     //�٨S���L���������A�P�_�P���@�ӻP�{�b���쪺�����Z���̵u
                if(!traveled[j] && (first || Points[order[i]]-Points[j]<dis)){
                    dis=Points[order[i]]-Points[j];
                    candidate=j;
                    first=false;
                }
            }
            sumDis+=dis;                            //�N��eiteration�̨Ϊ��U�@�y�����Z���[�J�Z���`�M
            order[i+1]=candidate;                   //�N��eiteration�̨Ϊ��U�@�y�����[�J�̨θѫ����s������
            traveled[candidate]=true;
        }
        sumDis+=Points[order[Points.size()-1]]-Points[order[0]];    //�[�J�q�̫�@���I����Ĥ@���I���Z��
        if(sumDis<bestDis || !k){                   //�Y��eiteration���Z����ثe�̨θѪ��Z���u
            bestDis=sumDis;                         //�N�̨θѪ��Z���]����eiteration���Z��
            for(int i=0; i<Points.size(); i++){     //�̨θѫ����s�����ǳ]����eiteration������
                bestOrder[i]=order[i];
            }
        }
    }

    ofstream fout(string(argv[1])+"/ans_"+string(argv[1])+".txt");
    fout << "distance: " << bestDis << endl;    //�Ӵ��ո�Ƨ�쪺�̵u���׿�X���ɮײĤ@��
    for(int i=0; i<Points.size(); i++){         //����ӧ�쪺�̨θѨ̧ǿ�X�����s����X���ɮ�
        fout << Points[bestOrder[i]].index << endl;
    }
    fout.close();
    cout << "ans_" << string(argv[1]) << ".txt created" << endl;

    return 0;
}
