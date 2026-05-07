#include<iostream>
#include<omp.h>
#include<iomanip>
#include<limits>
#include<time.h>
#include<chrono>

using namespace std;
using namespace chrono;



// ---------------- SUM SERIAL ----------------

int sum_serial(int arr[], int size)
{
    int sum = 0;

    for(int i = 0; i < size; i++)
    {
        sum += arr[i];
    }

    return sum;
}



// ---------------- SUM PARALLEL ----------------

int sum_parallel(int arr[], int size)
{
    int sum = 0;

    #pragma omp parallel for reduction(+:sum)
    for(int i = 0; i < size; i++)
    {
        sum += arr[i];
    }

    return sum;
}



// ---------------- AVERAGE SERIAL ----------------

int average_serial(int arr[], int size)
{
    int sum = 0;

    for(int i = 0; i < size; i++)
    {
        sum += arr[i];
    }

    return sum / size;
}



// ---------------- AVERAGE PARALLEL ----------------

int average_parallel(int arr[], int size)
{
    int sum = 0;

    #pragma omp parallel for reduction(+:sum)
    for(int i = 0; i < size; i++)
    {
        sum += arr[i];
    }

    return sum / size;
}



// ---------------- MIN SERIAL ----------------

int min_serial(int arr[], int size)
{
    int min_val = arr[0];

    for(int i = 1; i < size; i++)
    {
        if(arr[i] < min_val)
        {
            min_val = arr[i];
        }
    }

    return min_val;
}



// ---------------- MIN PARALLEL ----------------

int min_parallel(int arr[], int size)
{
    int min_val = arr[0];

    #pragma omp parallel for reduction(min:min_val)
    for(int i = 1; i < size; i++)
    {
        if(arr[i] < min_val)
        {
            min_val = arr[i];
        }
    }

    return min_val;
}



// ---------------- MAX SERIAL ----------------

int max_serial(int arr[], int size)
{
    int max_val = arr[0];

    for(int i = 1; i < size; i++)
    {
        if(arr[i] > max_val)
        {
            max_val = arr[i];
        }
    }

    return max_val;
}



// ---------------- MAX PARALLEL ----------------

int max_parallel(int arr[], int size)
{
    int max_val = arr[0];

    #pragma omp parallel for reduction(max:max_val)
    for(int i = 1; i < size; i++)
    {
        if(arr[i] > max_val)
        {
            max_val = arr[i];
        }
    }

    return max_val;
}



// ---------------- MAIN FUNCTION ----------------

int main()
{
    int size;

    cout << "Enter array size: ";
    cin >> size;

    int arr[size];

    srand(time(0));



    // Random array values
    for(int i = 0; i < size; i++)
    {
        arr[i] = rand() % 1000;
    }



    // ---------------- SUM ----------------

    auto ss_start = high_resolution_clock::now();

    int sum_s = sum_serial(arr, size);

    auto ss_end = high_resolution_clock::now();



    auto sp_start = high_resolution_clock::now();

    int sum_p = sum_parallel(arr, size);

    auto sp_end = high_resolution_clock::now();



    duration<double> d_ss = ss_end - ss_start;
    duration<double> d_sp = sp_end - sp_start;



    cout << "\n1. SUM\n\n";

    cout << "Serial Result   : " << sum_s << endl;
    cout << "Parallel Result : " << sum_p << endl;

    cout << "Serial Time     : "
         << fixed << setprecision(10)
         << d_ss.count() << " seconds\n";

    cout << "Parallel Time   : "
         << fixed << setprecision(10)
         << d_sp.count() << " seconds\n";

    cout << "Speedup         : "
         << d_ss.count() / d_sp.count() << "\n\n";



    // ---------------- AVERAGE ----------------

    auto as_start = high_resolution_clock::now();

    int avg_s = average_serial(arr, size);

    auto as_end = high_resolution_clock::now();



    auto ap_start = high_resolution_clock::now();

    int avg_p = average_parallel(arr, size);

    auto ap_end = high_resolution_clock::now();



    duration<double> d_as = as_end - as_start;
    duration<double> d_ap = ap_end - ap_start;



    cout << "2. AVERAGE\n\n";

    cout << "Serial Result   : " << avg_s << endl;
    cout << "Parallel Result : " << avg_p << endl;

    cout << "Serial Time     : "
         << d_as.count() << " seconds\n";

    cout << "Parallel Time   : "
         << d_ap.count() << " seconds\n";

    cout << "Speedup         : "
         << d_as.count() / d_ap.count() << "\n\n";



    // ---------------- MIN ----------------

    auto mins_start = high_resolution_clock::now();

    int min_s = min_serial(arr, size);

    auto mins_end = high_resolution_clock::now();



    auto minp_start = high_resolution_clock::now();

    int min_p = min_parallel(arr, size);

    auto minp_end = high_resolution_clock::now();



    duration<double> d_mins = mins_end - mins_start;
    duration<double> d_minp = minp_end - minp_start;



    cout << "3. MINIMUM\n\n";

    cout << "Serial Result   : " << min_s << endl;
    cout << "Parallel Result : " << min_p << endl;

    cout << "Serial Time     : "
         << d_mins.count() << " seconds\n";

    cout << "Parallel Time   : "
         << d_minp.count() << " seconds\n";

    cout << "Speedup         : "
         << d_mins.count() / d_minp.count() << "\n\n";



    // ---------------- MAX ----------------

    auto maxs_start = high_resolution_clock::now();

    int max_s = max_serial(arr, size);

    auto maxs_end = high_resolution_clock::now();



    auto maxp_start = high_resolution_clock::now();

    int max_p = max_parallel(arr, size);

    auto maxp_end = high_resolution_clock::now();



    duration<double> d_maxs = maxs_end - maxs_start;
    duration<double> d_maxp = maxp_end - maxp_start;



    cout << "4. MAXIMUM\n\n";

    cout << "Serial Result   : " << max_s << endl;
    cout << "Parallel Result : " << max_p << endl;

    cout << "Serial Time     : "
         << d_maxs.count() << " seconds\n";

    cout << "Parallel Time   : "
         << d_maxp.count() << " seconds\n";

    cout << "Speedup         : "
         << d_maxs.count() / d_maxp.count() << "\n\n";



    return 0;
}
