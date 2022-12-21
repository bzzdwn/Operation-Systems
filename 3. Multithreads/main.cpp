#include <Windows.h>
#include <iostream>

int array_size = 0;
int* arr = nullptr;
CRITICAL_SECTION cs;
HANDLE* hThreads;
HANDLE* hThreadsStartEvents;
HANDLE* hThreadWorkStopped;
HANDLE* hThreadsExit;
HANDLE hMutex;

DWORD WINAPI marker(LPVOID thread_index) {
	WaitForSingleObject(hThreadsStartEvents[(int)thread_index], INFINITE);
	int marked_numbers_counter = 0;
	srand((int)thread_index);
	while (true) {
		int random_number = rand() % array_size;
		if (arr[random_number] == 0) {
			EnterCriticalSection(&cs);
			Sleep(5);
			arr[random_number] = (int)thread_index + 1;
			marked_numbers_counter++;
			Sleep(5);
			LeaveCriticalSection(&cs);
		}
		else {
			EnterCriticalSection(&cs);
			std::cout << "\nNumber of thread = " << (int)thread_index <<
				"\nNumber of marked elements = " << marked_numbers_counter <<
				"\nIndex of not implemented element = " << random_number << "\n";
			LeaveCriticalSection(&cs);
			SetEvent(hThreadWorkStopped[(int)thread_index]);
			ResetEvent(hThreadsStartEvents[(int)thread_index]);
			HANDLE hThreadsStartExit[]{hThreadsStartEvents[(int)thread_index], hThreadsExit[(int)thread_index]};
			if (WaitForMultipleObjects(2, hThreadsStartExit, FALSE, INFINITE) == WAIT_OBJECT_0 + 1) {
				EnterCriticalSection(&cs);
				for (size_t i = 0; i < array_size; i++)
				{
					if (arr[i] == (int)thread_index + 1)
						arr[i] = 0;
				}
				LeaveCriticalSection(&cs);
				ExitThread(NULL);
			}
			else {
				ResetEvent(hThreadWorkStopped[(int)thread_index]);
				continue;
			}
		}
	}



}

int main() {
	int amount_of_threads = 0;
	int amount_of_terminated_threads = 0;

	std::cout << "Input array size:\n";
	std::cin >> array_size;
	arr = new int[array_size] {};
	std::cout << "Input amount of threads:\n";
	std::cin >> amount_of_threads;

	InitializeCriticalSection(&cs);

	hThreads = new HANDLE [amount_of_threads];
	hThreadsStartEvents = new HANDLE[amount_of_threads];
	hThreadWorkStopped = new HANDLE[amount_of_threads];
	hThreadsExit = new HANDLE[amount_of_threads];
	hMutex = CreateMutex(NULL, FALSE, NULL);

	bool* hThreadsExited = new bool[amount_of_threads] {};
	DWORD* dwThreads = new DWORD[amount_of_threads];
	
	for (int i = 0; i < amount_of_threads; i++)
	{
		hThreads[i] = CreateThread(NULL, 1, marker, (LPVOID)i, NULL, &dwThreads[i]);
		hThreadsStartEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		hThreadWorkStopped[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		hThreadsExit[i] = CreateEvent(NULL, TRUE, FALSE, NULL);

	}

	for (size_t i = 0; i < amount_of_threads; i++)
	{
			SetEvent(hThreadsStartEvents[i]);
	}

	while (amount_of_terminated_threads < amount_of_threads) {
		if (WaitForMultipleObjects(amount_of_threads, hThreadWorkStopped, TRUE, INFINITE) == WAIT_FAILED) {
			std::cout << "Wait for multiple objects failed.\n";
			std::cout << "Input any char to exit.\n";
		}
		WaitForSingleObject(hMutex, INFINITE);
		for (size_t i = 0; i < array_size; i++)
		{
			std::cout << arr[i] << "\t";
		}
		std::cout << "\n";
		ReleaseMutex(hMutex);
		
		std::cout << "Input marker thread number to stop:\n";
		int stop_marker_id;
		std::cin >> stop_marker_id;
		if (!hThreadsExited[stop_marker_id]) {
			amount_of_terminated_threads++;
			hThreadsExited[stop_marker_id] = true;
			SetEvent(hThreadsExit[stop_marker_id]);
			WaitForSingleObject(hThreads[stop_marker_id], INFINITE);
			CloseHandle(hThreads[stop_marker_id]);
			CloseHandle(hThreadsExit[stop_marker_id]);
			CloseHandle(hThreadsStartEvents[stop_marker_id]);
		}
		hMutex = OpenMutex(NULL, FALSE, NULL);
		WaitForSingleObject(hMutex, INFINITE);
		for (size_t i = 0; i < array_size; i++)
		{
			std::cout << arr[i] << "\t";
		}
		std::cout << "\n";
		ReleaseMutex(hMutex);


		for (size_t i = 0; i < amount_of_threads; i++)
		{
			if (!hThreadsExited[i]) {
				ResetEvent(hThreadWorkStopped[i]);
				SetEvent(hThreadsStartEvents[i]);
			}
		}
	}
	for (size_t i = 0; i < amount_of_threads; i++)
	{
		CloseHandle(hThreadWorkStopped[i]);
	}
	DeleteCriticalSection(&cs);
	return 0;
}