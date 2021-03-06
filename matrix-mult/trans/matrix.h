#include <iostream>
#include <vector>
#include "../threads/matrix_generator.h"
#include "csv_generator.h"
#include <chrono>
#include <unistd.h>
using namespace std;

class MatrixOperation
{
private:
  vector<vector<int>> m1;
  vector<vector<int>> m2;
  vector<vector<int>> r;

public:
  MatrixOperation(int n)
  {
    this->m1.resize(n, vector<int>(n));
    this->m2.resize(n, vector<int>(n));
    this->r.resize(n, vector<int>(n));
    this->m1 = matrix_generator(n);
    this->m2 = matrix_generator(n);
  }
  ~MatrixOperation()
  {
    this->m1.clear();
    this->m2.clear();
    this->r.clear();
    // cout << "destructor success";
  }

  void add_elemets(vector<vector<int>> &m1, vector<vector<int>> &m2, vector<vector<int>> &r, long i)
  {
    long mi = long(i);
    //cout<<endl<<"thread number "<< mi <<endl;
    for (long im1 = 0; im1 < m1.size(); im1++)
    {
      int sum = 0;
      for (long pos = 0; pos < m1[im1].size(); pos++)
        sum = sum + m1[im1][pos] * m2[mi][pos];
      r[im1][mi] = sum;
    }
  }
  void matrixprint(vector<vector<int>> &v)
  {
    for (int i = 0; i < v.size(); i++)
    {
      for (int j = 0; j < v.size(); j++)
      {
        cout << "\t" << v[i][j] << "\t";
      }
      cout << endl;
    }
  }

  vector<vector<int>> transp_matrix(vector<vector<int>> &v){
    vector<vector<int>> matrix;
    matrix.resize(v.size(),vector<int>(v.size()));

    for (int i=0;i < matrix.size();i ++){
        for (int j=0; j< matrix[i].size();j++)
        matrix[i][j] = v[j][i];
    }
    return matrix;
}

  double add()
  {

    // cpu time
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    this->m2 = transp_matrix(m2);
    for (long i = 0; i < this->m2.size(); i++)
      this->add_elemets(this->m1, this->m2, this->r, i);
    end = std::chrono::system_clock::now();
    double time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    //cout <<endl<<"time here : " <<time<<endl;
    csv_generator(this->r);
    csv_time(time, this->m2.size());
    return time;
  }
};