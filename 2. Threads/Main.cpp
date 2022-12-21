#include <iostream>
#include <Windows.h>

int		array_size = 0;
int*	arr = nullptr;
int		sum = 0;
int		min_element = 0;
int		max_element = 0;
int		min_index = 0;
int		max_index = 0;

DWORD WINAPI min_max(LPVOID) {
	min_element = arr[0];
	max_element = arr[0];
	for (size_t i = 1; i < array_size; i++)
	{
		if (arr[i] > max_element) {
			max_element = arr[i];
			max_index = i;
		}
		Sleep(7);
		if (arr[i] < min_element) {
			min_element = arr[i];
			min_index = i;
		}
		Sleep(7);
	}
	std::cout << "Minimal element:\t" << min_element << "\n";
	std::cout << "Maximal element:\t" << max_element << "\n";
	return 0;
}

DWORD WINAPI average(LPVOID) {
	for (size_t i = 0; i < array_size; i++)
	{
		sum += arr[i];
		Sleep(12);
	}
	std::cout << "Average:\t" << sum / array_size << "\n";
	return 0;
}

int main() {
	HANDLE	hMinMax;
	DWORD	IDMinMax;
	HANDLE  hAverage;
	DWORD	IDAverage;

	std::cout << "Input array size:\n";
	std::cin >> array_size;
	arr = new int[array_size];

	std::cout << "Input array elements:\n";
	for (size_t i = 0; i < array_size; i++)
	{
		std::cin >> arr[i];
	}

	hMinMax = CreateThread(NULL, 0, min_max, NULL, 0, &IDMinMax);
	hAverage= CreateThread(NULL, 0, average, NULL, 0, &IDAverage);
	WaitForSingleObject(hMinMax, INFINITE);
	WaitForSingleObject(hAverage, INFINITE);

	arr[min_index] = arr[max_index] = sum / array_size;
	std::cout << "Massive with changed minimal & maximal elements:\n";
	for (size_t i = 0; i < array_size; i++)
	{
		std::cout << arr[i] << " ";
	}

	CloseHandle(hMinMax);
	CloseHandle(hAverage);
	system("pause");
	return 0;
}