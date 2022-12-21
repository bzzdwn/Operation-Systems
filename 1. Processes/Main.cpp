#include <iostream>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include "Employee.h"
#include <vector>
#include <string>
int main() {
	std::string binary_file_name;
	std::string txt_file_name;
	std::string number_of_notes;
	std::string pay_for_an_hour;
	std::ifstream file_input;
	std::ifstream file_output;
	std::vector<employee> emps;
	employee empl;

	std::cout << "The main process has been started...\n";
	std::cout << "Input binary file name:\n";
	std::cin >> binary_file_name;
	std::cout << "Input number of notes:\n";
	std::cin >> number_of_notes;

	std::string creator_cmd = "Creator.exe " + binary_file_name + " " + number_of_notes;
	std::wstring converting_creator_to_lpwstr = std::wstring(creator_cmd.begin(), creator_cmd.end());
	LPWSTR lpszCreatorProcessCommandLine = &converting_creator_to_lpwstr[0];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	if (!CreateProcess(NULL, lpszCreatorProcessCommandLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		std::cout << "The Creator Process is not started.\n";
		return GetLastError();
	}
	WaitForSingleObject(pi.hProcess, INFINITE);

	file_input.open(binary_file_name, std::ios::binary);
	if (file_input.is_open()) {
		while (!file_input.eof()) {
			file_input.read((char*)&empl, sizeof(employee));
			emps.push_back(empl);
		}
		emps.pop_back();
	}
	std::cout << "BINARY FILE INFO:\nNUMBER\tNAME\tHOURS\n";
	for (size_t i = 0; i < emps.size(); i++)
	{
		std::cout << emps[i].num << "\t" << emps[i].name << "\t" << emps[i].hours << "\n";
	}

	std::cout << "Input txt file name:\n";
	std::cin >> txt_file_name;
	std::cout << "Input pay for an hour:\n";
	std::cin >> pay_for_an_hour;

	std::string reporter_cmd = "Reporter.exe " + binary_file_name + " " + txt_file_name + " " + pay_for_an_hour;
	std::wstring converting_reporter_to_lpwstr = std::wstring(reporter_cmd.begin(), reporter_cmd.end());
	LPWSTR lpszReporterProcessCommandLine = &converting_reporter_to_lpwstr[0];

	if (!CreateProcess(NULL, lpszReporterProcessCommandLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		std::cout << "The Reporter Process is not started.\n";
		return GetLastError();
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	std::cout << "Reporter Process has been terminated.\n";
	file_output.open(txt_file_name);
	if (file_output.is_open()) {
		while (!file_output.eof()) {
			std::string temporary;
			getline(file_output, temporary);
			std::cout << temporary << "\n";
		}
	}
	file_output.close();
	_getch();
	TerminateProcess(pi.hProcess, 1);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	std::cout << "Main Process has been terminated.\n";
	_getch();
	return 0;
}