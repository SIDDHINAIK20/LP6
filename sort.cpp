#include<iostream>
#include<omp.h>
#include<chrono>
#include<iomanip>

using namespace std;
using namespace chrono;



// ---------------- PRINT ARRAY ----------------

void print_array(int arr[], int size)
{
    for(int i = 0; i < size; i++)
    {
        cout << arr[i] << " ";
    }

    cout << endl;
}



// =====================================================
//                BUBBLE SORT
// =====================================================



// ---------------- SEQUENTIAL BUBBLE SORT ----------------

void sequential_bubble_sort(int arr[], int size)
{
    for(int i = 0; i < size - 1; i++)
    {
        for(int j = 0; j < size - i - 1; j++)
        {
            if(arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}



// ---------------- PARALLEL BUBBLE SORT ----------------

void parallel_bubble_sort(int arr[], int size)
{
    for(int i = 0; i < size - 1; i++)
    {
        #pragma omp parallel for
        for(int j = 0; j < size - i - 1; j++)
        {
            if(arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}



// =====================================================
//                MERGE SORT
// =====================================================



// ---------------- MERGE FUNCTION ----------------

void merge(int arr[], int left, int mid, int right)
{
    int i = left;
    int j = mid + 1;
    int k = 0;

    int temp[100000];

    while(i <= mid && j <= right)
    {
        if(arr[i] < arr[j])
        {
            temp[k++] = arr[i++];
        }
        else
        {
            temp[k++] = arr[j++];
        }
    }

    while(i <= mid)
    {
        temp[k++] = arr[i++];
    }

    while(j <= right)
    {
        temp[k++] = arr[j++];
    }

    for(i = left, k = 0; i <= right; i++, k++)
    {
        arr[i] = temp[k];
    }
}



// ---------------- SEQUENTIAL MERGE SORT ----------------

void sequential_merge_sort(int arr[], int left, int right)
{
    if(left < right)
    {
        int mid = (left + right) / 2;

        sequential_merge_sort(arr, left, mid);

        sequential_merge_sort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}



// ---------------- PARALLEL MERGE SORT ----------------

void parallel_merge_sort(int arr[], int left, int right)
{
    if(left < right)
    {
        int mid = (left + right) / 2;

        #pragma omp parallel sections
        {

            #pragma omp section
            parallel_merge_sort(arr, left, mid);

            #pragma omp section
            parallel_merge_sort(arr, mid + 1, right);
        }

        merge(arr, left, mid, right);
    }
}



// =====================================================
//                     MAIN
// =====================================================

int main()
{
    int size;

    cout << "Enter array size: ";
    cin >> size;



    int bubble_seq[size];
    int bubble_par[size];

    int merge_seq[size];
    int merge_par[size];



    // Random values
    for(int i = 0; i < size; i++)
    {
        int value = rand() % 1000;

        bubble_seq[i] = value;
        bubble_par[i] = value;

        merge_seq[i] = value;
        merge_par[i] = value;
    }



    // =====================================================
    //            BUBBLE SORT
    // =====================================================

    cout << "\n=========== BUBBLE SORT ===========\n";



    auto bs_start = high_resolution_clock::now();

    sequential_bubble_sort(bubble_seq, size);

    auto bs_end = high_resolution_clock::now();



    auto bp_start = high_resolution_clock::now();

    parallel_bubble_sort(bubble_par, size);

    auto bp_end = high_resolution_clock::now();



    duration<double> bubble_seq_time = bs_end - bs_start;

    duration<double> bubble_par_time = bp_end - bp_start;



    cout << "\nSequential Bubble Sort:\n";
    print_array(bubble_seq, size);



    cout << "\nParallel Bubble Sort:\n";
    print_array(bubble_par, size);



    cout << "\nSequential Time : "
         << fixed << setprecision(10)
         << bubble_seq_time.count() << " seconds";



    cout << "\nParallel Time   : "
         << bubble_par_time.count() << " seconds";



    cout << "\nSpeedup         : "
         << bubble_seq_time.count() / bubble_par_time.count();



    // =====================================================
    //            MERGE SORT
    // =====================================================

    cout << "\n\n=========== MERGE SORT ===========\n";



    auto ms_start = high_resolution_clock::now();

    sequential_merge_sort(merge_seq, 0, size - 1);

    auto ms_end = high_resolution_clock::now();



    auto mp_start = high_resolution_clock::now();

    parallel_merge_sort(merge_par, 0, size - 1);

    auto mp_end = high_resolution_clock::now();



    duration<double> merge_seq_time = ms_end - ms_start;

    duration<double> merge_par_time = mp_end - mp_start;



    cout << "\nSequential Merge Sort:\n";
    print_array(merge_seq, size);



    cout << "\nParallel Merge Sort:\n";
    print_array(merge_par, size);



    cout << "\nSequential Time : "
         << merge_seq_time.count() << " seconds";



    cout << "\nParallel Time   : "
         << merge_par_time.count() << " seconds";



    cout << "\nSpeedup         : "
         << merge_seq_time.count() / merge_par_time.count();



    return 0;
}
