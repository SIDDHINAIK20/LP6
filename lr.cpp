#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace chrono;

void linearRegression(vector<double> &x, vector<double> &y,
                      bool parallel)
{

    int n = x.size();

    double sx = 0, sy = 0, sxy = 0, sx2 = 0;

    auto start = high_resolution_clock::now();

    if (parallel)
    {

#pragma omp parallel for reduction(+ : sx, sy, sxy, sx2)
        for (int i = 0; i < n; i++)
        {
            sx += x[i];
            sy += y[i];
            sxy += x[i] * y[i];
            sx2 += x[i] * x[i];
        }
    }
    else
    {

        for (int i = 0; i < n; i++)
        {
            sx += x[i];
            sy += y[i];
            sxy += x[i] * y[i];
            sx2 += x[i] * x[i];
        }
    }

    double b1 =
        (n * sxy - sx * sy) /
        (n * sx2 - sx * sx);

    double b0 =
        (sy - b1 * sx) / n;

    auto end = high_resolution_clock::now();

    double time =
        duration<double>(end - start).count();

    cout << (parallel ? "\nParallel\n" : "\nSequential\n");

    cout << "Equation: y = "
         << b1 << "x + "
         << b0 << endl;

    cout << "Time: "
         << time << " sec\n";

    static double seq;

    if (!parallel)
        seq = time;
    else
        cout << "Speedup: "
             << seq / time << endl;
}

int main()
{
    int n = 1000000;

    vector<double> x(n), y(n);

    for (int i = 0; i < n; i++)
    {
        x[i] = rand() % 1000;
        y[i] = rand() % 1000;
    }

    linearRegression(x, y, false);

    linearRegression(x, y, true);

    return 0;
}
