#include "Employee.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>

int main(int argc, char* argv[]) {
	employee empl;
	std::string file_input_name = argv[1];
	std::string file_output_name = argv[2];
	int salary_for_hour = std::stoi(argv[3]);
	std::ifstream file_input;
	std::ofstream file_output;
	std::vector<employee> emps;
	file_input.open(file_input_name, std::ios::binary);
	if (file_input.is_open()) {
		while (!file_input.eof()) {
			file_input.read((char*)&empl, sizeof(employee));
			emps.push_back(empl);
		}
		emps.pop_back();
	}
	file_input.close();
	file_output.open(file_output_name);
	if (file_output.is_open()) {
		file_output << "Report to the file " << file_input_name << "\n";
		file_output << "NUMBER\tNAME\tHOURS\tSALARY\n";
		for (size_t i = 0; i < emps.size(); i++)
		{
			file_output << emps[i].num << "\t" << emps[i].name << "\t" << emps[i].hours
				<< "\t" << salary_for_hour * emps[i].hours << "\n";
		}
	}
	file_output.close();
	Sleep(100);
	return 0;
}