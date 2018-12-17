#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

#define T0 50000.0 // ��ʼ�¶�
#define T_end (1e-8) //
#define q 0.99 // �˻�ϵ��
#define L 10000 // ÿ���¶�ʱ�ĵ�������
#define N 131 // ��������

int city_list[N]; // ���һ����
double city_pos[N][2];
    

//int city_list[N];
//vector<vector<double>> city_pos;

// ����
double distance(double *, double *); // ������������֮��ľ���

double path_len(int *); // ����·������

void init();

void create_new(); // �����½�

double distance(double *city1, double *city2) {
  double x1 = *city1;
  double y1 = *(city1 + 1);
  double x2 = *(city2);
  double y2 = *(city2 + 1);

  double dis = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
  return dis;
}

double path_len(int *arr) {
  double path = 0;
  int index = *arr; // �������
  for (int i = 0; i < N - 1; i++) {
    int index1 = *(arr + i); // ���
    int index2 = *(arr + 1 + i); // �յ�
    double dis = distance(city_pos[index1 - 1], city_pos[index2 - 1]);
    path += dis;
  }
  // �����·
  int last_index = *(arr + N - 1); // ���һ���������
  int first_index = *arr; // ��һ���������
  double last_dis = distance(city_pos[last_index - 1], city_pos[first_index - 1]);
  path += last_dis;
  return path;
}

void init() {
  int k = 0;
  double index, num_x, num_y;
  ifstream infile;
  infile.open("tspdata.txt");

  if (!infile.is_open()) {
    printf("Open file failed!\n");
  }

  while (!infile.eof()) {
    infile >> index >> num_x >> num_y;
    city_pos[k][0] = num_x;
    city_pos[k][1] = num_y;
    k++;
  }

  for (int i = 0; i < N; i++) {
    city_list[i] = i+1; // ��ʼ��һ����
  }
}

// �����������λ�õķ�ʽ�����µĽ�
void create_new() {
  double p  = ((double)rand()) / (RAND_MAX + 1.0);

  if (p <= 0.25) {
    double r1 = ((double)rand()) / (RAND_MAX + 1.0);
    double r2 = ((double)rand()) / (RAND_MAX + 1.0);
    int pos1 = (int)(N * r1); // ��һ�������λ��
    int pos2 = (int)(N * r2); // �ڶ��������λ��
    while (pos1 == pos2) {
      r1 = ((double)rand()) / (RAND_MAX + 1.0);
      r2 = ((double)rand()) / (RAND_MAX + 1.0);
      pos1 = (int)(N * r1); // ��һ�������λ��
      pos2 = (int)(N * r2); // �ڶ��������λ��      
    }
    int temp = city_list[pos1];
    city_list[pos1] = city_list[pos2];
    city_list[pos2] = temp;
  }

  else if (p > 0.25 && p <= 0.50) {
    double r1 = ((double)rand()) / (RAND_MAX + 1.0);
    double r2 = ((double)rand()) / (RAND_MAX + 1.0);
    int pos1 = (int)(N * r1); // ��һ�������λ��
    int pos2 = (int)(N * r2); // �ڶ��������λ��
    while (pos1 == pos2) {
      r1 = ((double)rand()) / (RAND_MAX + 1.0);
      r2 = ((double)rand()) / (RAND_MAX + 1.0);
      pos1 = (int)(N * r1); // ��һ�������λ��
      pos2 = (int)(N * r2); // �ڶ��������λ��      
    }
    int p1 = min(pos1, pos2);
    int p2 = max(pos1, pos2);
    for (int i = p1,j = p2; i < j; i++,j--) {
      int temp = city_list[p1];
      city_list[p1] = city_list[p2];
      city_list[p2] = temp;
    }   
  }

  else if (p > 0.50 && p <= 0.75) {
    double r1 = ((double)rand()) / (RAND_MAX + 1.0);
    double r2 = ((double)rand()) / (RAND_MAX + 1.0);
    int pos1 = (int)(N * r1); // ��һ�������λ��
    int pos2 = (int)(N * r2); // �ڶ��������λ��
    while (pos1 == pos2) {
      r1 = ((double)rand()) / (RAND_MAX + 1.0);
      r2 = ((double)rand()) / (RAND_MAX + 1.0);
      pos1 = (int)(N * r1); // ��һ�������λ��
      pos2 = (int)(N * r2); // �ڶ��������λ��      
    }
    int* path_temp = new int[N];
    memcpy(path_temp, city_list, N * sizeof(int));
    city_list[0] = path_temp[pos1];
    city_list[1] = path_temp[pos2];
    int pos = 2;
    for (int i = 0; i < N; i++) {
      if (i == pos1 || i == pos2) continue;
      city_list[pos++] = path_temp[i];
    }
    free(path_temp);    
  }

  else {
    double r1 = ((double)rand()) / (RAND_MAX + 1.0);
    double r2 = ((double)rand()) / (RAND_MAX + 1.0);
    int pos1 = (int)(N * r1); // ��һ�������λ��
    int pos2 = (int)(N * r2); // �ڶ��������λ��
    while (pos1 == pos2) {
      r1 = ((double)rand()) / (RAND_MAX + 1.0);
      r2 = ((double)rand()) / (RAND_MAX + 1.0);
      pos1 = (int)(N * r1); // ��һ�������λ��
      pos2 = (int)(N * r2); // �ڶ��������λ��      
    }

    int* path_temp = new int[N];
    memcpy(path_temp, city_list, N * sizeof(int));    
    int p1 = min(pos1, pos2);
    int p2 = max(pos1, pos2);

    int pos = 0;
    for (int i = p2; i < N; i++) {
      city_list[pos++] = path_temp[i];
    }
    for (int i = p1+1; i < p2; i++) {
      city_list[pos++] = path_temp[i];
    }
    for (int i = 0; i <= p1; i++) {
      city_list[pos++] = path_temp[i];
    }

    free(path_temp);
  }
}

int main() {
  srand((unsigned)time(NULL)); // ��ʼ�����������
  time_t start, finish;
  start = clock(); // ��ʼ��ʱ
  double T;
  int count = 0; // ��¼���´���
  T = T0;
  init();
  int city_list_copy[N]; // ���ڱ���ԭʼ��
  double f1, f2, df; // f1Ϊ��ʼ��Ŀ�꺯��ֵ, f2Ϊ�½�Ŀ�꺯��ֵ,dfΪ���߲�ֵ
  double r; // 0-1֮������������������Ƿ�����½�

  ofstream outfile;
  outfile.open("result.txt");
  ofstream outfile2;
  outfile2.open("result2.txt");
  if (!outfile.is_open()) {
    printf("Open outfile failed!\n");
  }
  // ���¶ȵ��ڽ����¶�ʱ���˻����
  int count_output = 0;
  while (T > T_end) {
    for (int i = 0; i < L; i++) {
      memcpy(city_list_copy, city_list, N * sizeof(int)); // ��������
      create_new(); // �����½�
      f1 = path_len(city_list_copy);
      f2 = path_len(city_list);
      df = f2 - f1;

      // �˻�׼��
      if (df >= 0) {
        r = ((double)rand()) / (RAND_MAX);
        // ����ԭ���Ľ�
        if (exp(-df / T) <= r) {
          memcpy(city_list, city_list_copy, N * sizeof(int));
        }
      }
    }
    outfile2 << path_len(city_list) << endl;
    cout << "��ǰ�¶ȣ� " << T << "  " << "��ǰ���Ž�: " <<  path_len(city_list) << endl;
    count_output++;
    
    if(count%10 == 0){
	  for (int i = 0; i < N; i++) {
		outfile << (i+1) << "\t" << city_list[i] << endl; 
	  }
	}
    T *= q; // ����
    count++;
  }
  outfile2.close();
  outfile.close();
  
  finish = clock(); // �˻����
  double duration = ((double)(finish - start)) / CLOCKS_PER_SEC;
  cout << "Simulated Annealing: " << endl;
  cout << "Initial Temperature T0 = " << T0 << ", Cooling Coefficient q = " << q << ", Loops per Temperature: " << L << ", Total Cooling: " <<  count << "times" << endl;

  cout << "Best Path: " << endl;
  for (int i = 0; i < N - 1; i++) {
    cout << city_list[i] <<  "-->";
  }
  cout << city_list[N - 1];

  cout << "\nRelative Error: " << (path_len(city_list) - 564) / 564 << endl;

  cout << "\nBest Path Length: " << path_len(city_list) << endl;

  cout << "Time for Algorithm: " << duration << "sec" << endl;
  return 0;
}
