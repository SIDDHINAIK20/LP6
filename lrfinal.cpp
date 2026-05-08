#include<iostream>
#include<vector>
#include<omp.h>
#include<chrono>
#include<iomanip>

using namespace std;
using namespace chrono;



// =====================================================
//                 SEQUENTIAL LINEAR REGRESSION
// =====================================================

void sequential_linear_regression(vector<double> x,
                                  vector<double> y,
                                  int n)
{
    double sum_x = 0;
    double sum_y = 0;
    double sum_xy = 0;
    double sum_x2 = 0;



    for(int i = 0; i < n; i++)
    {
        sum_x += x[i];

        sum_y += y[i];

        sum_xy += x[i] * y[i];

        sum_x2 += x[i] * x[i];
    }



    double slope =
        (n * sum_xy - sum_x * sum_y) /
        (n * sum_x2 - sum_x * sum_x);



    double intercept =
        (sum_y - slope * sum_x) / n;



    cout << "\nSequential Linear Regression\n";

    cout << "Slope (m)     : " << slope << endl;

    cout << "Intercept (b) : " << intercept << endl;
}



// =====================================================
//                 PARALLEL LINEAR REGRESSION
// =====================================================

void parallel_linear_regression(vector<double> x,
                                vector<double> y,
                                int n)
{
    double sum_x = 0;
    double sum_y = 0;
    double sum_xy = 0;
    double sum_x2 = 0;



    #pragma omp parallel for reduction(+:sum_x,sum_y,sum_xy,sum_x2)
    for(int i = 0; i < n; i++)
    {
        sum_x += x[i];

        sum_y += y[i];

        sum_xy += x[i] * y[i];

        sum_x2 += x[i] * x[i];
    }



    double slope =
        (n * sum_xy - sum_x * sum_y) /
        (n * sum_x2 - sum_x * sum_x);



    double intercept =
        (sum_y - slope * sum_x) / n;



    cout << "\nParallel Linear Regression\n";

    cout << "Slope (m)     : " << slope << endl;

    cout << "Intercept (b) : " << intercept << endl;
}



// =====================================================
//                        MAIN
// =====================================================

int main()
{
    int n;

    cout << "Enter number of data points: ";
    cin >> n;



    vector<double> x(n);
    vector<double> y(n);



    cout << "\nEnter x values:\n";

    for(int i = 0; i < n; i++)
    {
        cin >> x[i];
    }



    cout << "\nEnter y values:\n";

    for(int i = 0; i < n; i++)
    {
        cin >> y[i];
    }



    // =====================================================
    //                  SEQUENTIAL
    // =====================================================

    auto seq_start = high_resolution_clock::now();

    sequential_linear_regression(x, y, n);

    auto seq_end = high_resolution_clock::now();



    // =====================================================
    //                  PARALLEL
    // =====================================================

    auto par_start = high_resolution_clock::now();

    parallel_linear_regression(x, y, n);

    auto par_end = high_resolution_clock::now();



    // =====================================================
    //                    TIME
    // =====================================================

    duration<double> seq_time =
        seq_end - seq_start;

    duration<double> par_time =
        par_end - par_start;



    // =====================================================
    //                    OUTPUT
    // =====================================================

    cout << "\nSequential Time : "
         << fixed << setprecision(10)
         << seq_time.count() << " seconds";



    cout << "\nParallel Time   : "
         << par_time.count() << " seconds";



    cout << "\nSpeedup         : "
         << seq_time.count() / par_time.count();



    return 0;
}
