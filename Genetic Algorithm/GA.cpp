#include <iostream>  
#include <string.h>  
#include <fstream>  
#include <iterator>  
#include <algorithm>  
#include <limits.h>  
#include <math.h>  
#include <stdlib.h>  
#include <ctime>
#include <vector>
#include <map>

using namespace std;

const int nCities = 131; 					// ��������  
const int BEST = 564;						// ���Ž�

double length_table[nCities][nCities];		// �洢��������
	
const int group_size = 1000;					// ��Ⱥ��С
const int time_to_breed = 8000;				// ��ֳ����
const double p_cross = 0.8;					// �������
double p_variation = 0.5;					// �������

// һ������
class Path 
{
public:
	// ����·���ĳ���
	void Calculate_length() {
		length = 0;
		//����path
		for (int i = 1; i < nCities; i++) {
			length += length_table[path[i - 1] - 1][path[i] - 1];
		}
		length += length_table[path[nCities - 1] - 1][path[0] - 1];
	}

	// �������һ����
	Path() {
		length = 0;
		generate_random();
		Calculate_length();
	}

	void generate_random(){
		for (int i = 0; i < nCities; ++i)
		{
			path[i] = i+1;
		}
		srand(time(0));
		/*for (int i = 0; i < nCities; ++i)
		{
			int city1 = rand() % nCities;
			int city2 = rand() % nCities;
			int temp = path[city2];
			path[city2] = path[city1];
			path[city1] = temp;
		}*/
	}

	void getNewSolution_cross(Path &t) {
		// ���㽻��
		int mark = rand() % (nCities - 2) + 1;//1 to nCities - 2
		for (int i = mark; i < nCities; i++) {
			int temp = path[i];
			path[i] = t.path[i];
			t.path[i] = temp;
		}
		// �жϽ�ĺϷ���
		int i = 0; int j = 0;
		bool count_dup_1[nCities + 1] = { false };
		bool count_dup_2[nCities + 1] = { false };
		while (i < nCities && j < nCities) {
			if (count_dup_1[path[i]] && count_dup_2[t.path[j]]) {
				int temp = path[i];
				path[i] = t.path[j];
				t.path[j] = temp;
				i++;
				j++;
			}
			if (i >= nCities || j >= nCities)
				break;
			if (!count_dup_1[path[i]]) {
				count_dup_1[path[i]] = true;
				i++;
			}
			if (!count_dup_2[t.path[j]]) {
				count_dup_2[t.path[j]] = true;
				j++;
			}
		}
		Calculate_length();
		t.Calculate_length();
	}

	// ���ֱ������,�����ڱ仯��������ַǷ���
	void getNewSolution_variation() {
		int i = rand() % nCities;
		int j = rand() % nCities;
		if (i > j) {
			swap(i, j);
		}
		else if (i == j)return;
		//���ȡ·����������в���

		int choose = rand() % 3;
		switch (choose) {
		case 0:
			swap(path[i], path[j]); break;
		case 1:
			reverse(path + i, path + j); break;
		default:
			if (j == (nCities - 1)) return;
			rotate(path + i, path + j, path + j + 1);
		}
		Calculate_length();
	}

	double getLength(){
		return length;
	}

	int* getPath(){
		return path;
	}

private:
	double length;//���ۣ��ܳ���
	int path[nCities];//·��
};

class GA{
public:
	struct node{
		int num;
		double x;
		double y;
	}nodes[nCities];

	//��Ⱥ����СΪgroup_size
	vector<Path> group;

	void init_dis();
	
	GA();
	
	~GA();

	Path getPath();

private:
	void choose(vector<Path>& group);
	
	void cross(vector<Path>& group);

	void variation(vector<Path>& group);

	void judge(vector<Path> & old_group, vector<Path> & group);

	void init();
};

GA::GA(){
}

GA::~GA(){
	group.clear();
}

void GA::init(){
	// ��ʼ����Ⱥ
	group.resize(group_size,Path());	
	// ����ȺһЩ �ֲ��������������Ž�
	Path new_path;
	Path copy = new_path;
	int j = 0;
	while(j < 50000){
		new_path.getNewSolution_variation();
		if(copy.getLength() < new_path.getLength()){
			copy = new_path;
		}
		j++;
	}
	for(int i = 0; i < 100; i++){
		int num = rand()%group_size;
		group[num] = copy;
	}
	 
}

// ��ȡtxt�ļ�����ȡ����
void GA::init_dis() 
{
	int i, j;
	ifstream in("tspdata.txt");
	for (i = 0; i < nCities; i++)
	{
		in >> nodes[i].num >> nodes[i].x >> nodes[i].y;
	}

	for (i = 0; i < nCities; i++)
	{
		length_table[i][i] = (double)INT_MAX;
		for (j = i + 1; j < nCities; j++)
		{
			length_table[i][j] = length_table[j][i] = sqrt(
				(nodes[i].x - nodes[j].x) * (nodes[i].x - nodes[j].x) +
				(nodes[i].y - nodes[j].y) * (nodes[i].y - nodes[j].y));
		}
	}
}

// ѡ���������Ⱥ
void GA::choose(vector<Path> & group){
	double sum_fitness = 0;
	double fitness[group_size];//��Ӧ�����飬����Ӧ����������
	double chance[group_size];//��������
	double pick;//�������̶ĵ������
	vector<Path> next;

	for (int i = 0; i < group_size; i++) {
		fitness[i] = 1 / group[i].getLength();
		sum_fitness += fitness[i];
	}
	for (int i = 0; i < group_size; i++) {
		chance[i] = fitness[i] / sum_fitness;
	}
	//���̶Ĳ���
	for (int i = 0; i < group_size; i++) {
		pick = ((double)rand()) / RAND_MAX;//0��1�������
		for (int j = 0; j < group_size; j++) {
			pick -= chance[j];
			// ��������ѡ����pickС��0��ѡ����Ⱥ��chanceԽ��Խ�л���
			if (pick <= 0) {
				next.push_back(group[j]);
				break;
			}
			//��δѡ�У������Ѿ������һ����
			if (j == group_size - 1) {
				next.push_back(group[j]);
			}
		}
	}
	group = next;
}

// ����
void GA::cross(vector<Path> & group) {
	int point = 0;
	int choice1, choice2;
	while (point < group_size) {
		double pick = ((double)rand()) / RAND_MAX;//0��1�������
		if (pick > p_cross)
			continue;//�ж��Ƿ񽻲�
		else {
			// ѡ���ٽ������������н���
			// ���ԸĽ�Ϊ���ѡ��
			choice1 = point;
			choice2 = point + 1;
			group[choice1].getNewSolution_cross(group[choice2]);//����
		}
		point += 2;
	}
}

// ����
void GA::variation(vector<Path> & group) {
	int point = 0;
	while (point < group_size) {
		double pick = ((double)rand()) / RAND_MAX;//0��1�������
		// ���ʱ���
		// ���ԸĽ�Ϊ���ѡ��
		if (pick < p_variation) {
			group[point].getNewSolution_variation();
		}
		point++;
	}
}

// �����Ӵ��Ƿ���ȡ���ױ�����ȡ��������Ⱥ
void GA::judge(vector<Path> & old_group, vector<Path> & group) {
	int point = 0;
	while (point < group_size) {
		if (old_group[point].getLength() < group[point].getLength())
			group[point] = old_group[point];
		point++;
	}
}

Path GA::getPath() {
	// ��ʼ�����������
	srand((unsigned)time(NULL));
	// ��ʼ����Ⱥ
	init(); 

	ofstream outfile, outfile2;
	outfile2.open("result2.txt");
	outfile.open("result.txt");
	if (!outfile.is_open()) {
	  	printf("Open outfile failed!\n");
	}
	
	Path best;
	for (int i = 0; i < time_to_breed; i++) {
		vector<Path> old_group = group;
		// ѡ��
		choose(group);
		// ����
		cross(group);
		// �������Ϊ5�������µĸ���
		for(int j = 0; j < 5; j++){
			variation(group);
			judge(old_group,group);
		}
		// �ҳ���Ⱥ�е����Ž�
		for (int j = 0; j < group_size; j++) {
			group[j].Calculate_length();
			if (group[j].getLength() < best.getLength()){
				best = group[j];
			}
		}
		cout << "��ǰ���Ž�Ϊ��" << best.getLength() << "  ��ǰ��������Ϊ��" << i << endl;
		
		if(i % 100 == 0){	
			for (int i = 0; i < nCities; i++) {
				outfile << (i+1) << "\t" << best.getPath()[i] << endl; 
			}
		}
		outfile2 << best.getLength() << endl;
	}
	outfile2.close();
	outfile.close();
	return best;
}

int main() {
	time_t start, finish;
    start = clock(); // ��ʼ��ʱ
	GA process;
	process.init_dis();
	Path ans = process.getPath();
	bool count[nCities] = {false};
	int count2 = 0;
	for (int j = 0; j < nCities; ++j)
	{
		for (int i = 0; i < nCities; ++i)
		{
			if (ans.getPath()[i] == j+1)
			{
				count2++;
				break;
			}
		}
	}
	finish = clock(); //��ʱ���� 
	if (count2 == nCities)
	{
		cout << endl << "Legal Solution!\n";
	}
	else{
		cout << endl << "Illegal Solution!\n";
		return 0;
	}
	double duration = ((double)(finish - start)) / CLOCKS_PER_SEC;
    cout << "Genetic Algorithm: " << endl;
    cout << "Best Path: " << endl;
  
	for (int i = 0; i < nCities-1; i++) {
		cout << ans.getPath()[i] << " -> ";
	}
	cout << ans.getPath()[nCities - 1];
	
	cout << "\nRelative Error: " << (ans.getLength() - BEST) / BEST << endl;

	cout << "\nBest Path Length: " << ans.getLength() << endl;

	cout << "Time for Algorithm: " << duration << "sec" << endl;
	cout << endl;
}



