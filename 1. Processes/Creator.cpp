#include "Employee.h"
#include <string>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
	employee empl;
	std::string file_name = argv[1];
	int number_of_notes = std::stoi(argv[2]);
	std::ofstream file;
	file.open(file_name, std::ios::binary);
	if (file.is_open()) {
		for (size_t i = 0; i < number_of_notes; i++)
		{
			std::cout << "Enter employee " << i + 1 << ":\n";
			std::cin >> empl.num >> empl.name >> empl.hours;
			file.write((char*)&empl, sizeof(employee));
		}
	}
	file.close();
	return 0;
}