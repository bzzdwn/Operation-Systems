#include <iostream>
#include<Windows.h>
#include <algorithm>
#include <thread>
#include <condition_variable>
#include <mutex>
using namespace std;

int countofX = 0;
std::mutex Mutex;
std::condition_variable_any ContinueEvent, OutEvent;
std::mutex cv_ce, cv_oe;

struct arrayInfo
{
    int size;
    int* array;

};

void Work(arrayInfo* per)
{
    Mutex.lock();
    int* arr = per->array;
    int size = per->size;

    cout << "Work is started." << endl;

    int k = 0;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (arr[i] == arr[j])
                k++;
        }
        if (k > 1)
            cout << arr[i] << " ";
        k = 0;
    }


    cout << endl;

    cout << "Work is finished." << endl;

    Mutex.unlock();
}
int n;
void MultElement(arrayInfo* per)
{
    Mutex.lock();
    int* arr = per->array;
    int size = per->size;

    cout << "MultElement is started." << endl;
    int com = 1;

    for (int i = 0; i < size; i++)
    {
        com *= arr[i];
    }
    n = com;



    Mutex.unlock();

    OutEvent.notify_all();
    std::unique_lock<std::mutex> ce(cv_ce);
    ContinueEvent.wait(ce);
    cout << "MultElement is finished." << endl;
}

int main()
{
    int size;
    cout << "Enter size of array: " << endl;
    cin >> size;

    int* arr = new int[size];
    cout << "Enter " << size << " elements" << endl;
    for (int i = 0; i < size; i++)
    {
        cin >> arr[i];
    }

    cout << "Arr: " << endl;
    for (int i = 0; i < size; i++)
    {
        cout << arr[i] << " ";
    }
    arrayInfo* per = new arrayInfo();
    per->size = size;
    per->array = arr;
    cout << endl;

    std::thread ThreadWork(Work, per);
    
    std::thread ThreadMultElement(MultElement, per);

    std::unique_lock<std::mutex> oe(cv_oe);
    OutEvent.wait(oe);

    cout << "Composition of element: " << n << endl;

    ContinueEvent.notify_all();

    ThreadWork.join();
    ThreadMultElement.join();

    delete[] arr;
    return 0;
}