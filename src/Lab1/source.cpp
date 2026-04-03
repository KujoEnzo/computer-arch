#include <iostream>
#include <vector>
#include <numeric>
#include <iomanip>
#include <chrono>
using namespace std;
using namespace std::chrono;

//实验一
//平凡算法
void trivMat(const double* mat, const double* v, double* res, int N) {
    for (int j = 0; j < N; ++j) {
        res[j] = 0.0;
        for (int i = 0; i < N; ++i) {
            res[j] += mat[i * N + j] * v[i]; 
        }
    }
}

//Cache优化算法
void OpMat(const double* mat, const double* v, double* res, int N) {
    for (int j = 0; j < N; ++j) res[j] = 0.0;
    for (int i = 0; i < N; ++i) {
        double vec = v[i]; 
        for (int j = 0; j < N; ++j) {
            res[j] += mat[i * N + j] * vec; 
        }
    }
}
//实验二
//平凡算法
double TrivSum(const double* arr, int N) {
    double sum = 0.0;
    for (int i = 0; i < N; ++i) {
        sum += arr[i]; 
    }
    return sum;
}

//进阶优化
double OpSum(const double* arr, int N) {
    double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0, sum4 = 0.0;
    int i = 0;
    
    for (; i <= N - 4; i += 4) {
        sum1 += arr[i];
        sum2 += arr[i + 1];
        sum3 += arr[i + 2];
        sum4 += arr[i + 3];
    }
    
    double total_sum = sum1 + sum2 + sum3 + sum4;
    
    for (; i < N; ++i) {
        total_sum += arr[i];
    }
    return total_sum;
}

int main() {
    const int MATRIX_N = 8192; 
    const int ARRAY_N = 50000000; //在实验时N的取值不同，此处仅供参考
    const int ITERS = 5;
    
    cout << fixed << setprecision(4);
    cout << "START" << endl;

    vector<double> mat(MATRIX_N * MATRIX_N, 1.0f);
    vector<double> vec(MATRIX_N, 2.0f);
    vector<double> res_mat(MATRIX_N, 0.0);
    vector<double> arr(ARRAY_N, 1.5f);

    cout << "矩阵与向量内积" << endl;
        
    auto start = high_resolution_clock::now();
    for(int k = 0; k < ITERS; k++) {
        trivMat(mat.data(), vec.data(), res_mat.data(), MATRIX_N);
    }
    auto end = high_resolution_clock::now();
    double time_trivial_mat = duration_cast<milliseconds>(end - start).count() / (double)ITERS;
    cout << "平凡算法平均耗时: " << time_trivial_mat << " ms" << endl;

    start = high_resolution_clock::now();
    for(int k = 0; k < ITERS; k++) {
        OpMat(mat.data(), vec.data(), res_mat.data(), MATRIX_N);
    }
    end = high_resolution_clock::now();
    double time_opt_mat = duration_cast<milliseconds>(end - start).count() / (double)ITERS;
    cout << "优化算法平均耗时: " << time_opt_mat << " ms" << endl;
    cout << "-> Cache 优化加速比: " << time_trivial_mat / time_opt_mat << "x\n" << endl;

    cout << "数列求和测试" << endl;
    volatile double final_sum = 0;

    start = high_resolution_clock::now();
    for(int k = 0; k < ITERS * 10; k++) {
        final_sum = TrivSum(arr.data(), ARRAY_N);
    }
    end = high_resolution_clock::now();
    double time_trivial_sum = duration_cast<milliseconds>(end - start).count() / (double)(ITERS * 10);
    cout << "平凡算法平均耗时: " << time_trivial_sum << " ms (Sum: " << final_sum << ")" << endl;

    start = high_resolution_clock::now();
    for(int k = 0; k < ITERS * 10; k++) {
        final_sum = OpSum(arr.data(), ARRAY_N);
    }
    end = high_resolution_clock::now();
    double time_opt_sum = duration_cast<milliseconds>(end - start).count() / (double)(ITERS * 10);
    cout << "优化算法平均耗时: " << time_opt_sum << " ms (Sum: " << final_sum << ")" << endl;
    cout << "-> ILP及展开优化加速比: " << time_trivial_sum / time_opt_sum << "x\n" << endl;
    cout << "END" << endl;

    return 0;
}