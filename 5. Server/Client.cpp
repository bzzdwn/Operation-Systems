#include <conio.h>
#include <windows.h>
#include <iostream>

struct employee
{
    int num; // идентификационный номер сотрудника
    char name[10]; // имя сотрудника
    double hours; // количество отработанных часов
};

int main(int argc, char* argv) {
	HANDLE hStartEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, L"Process Started");
    if (hStartEvent == NULL)
    {
        std::cout << "Open event failed. \nInput any char to exit.\n";
        std::cin.get();
        return GetLastError();
    }
    SetEvent(hStartEvent);
    HANDLE hPipe = CreateFile(L"\\\\.\\pipe\\pipe_name", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hPipe == INVALID_HANDLE_VALUE)
    {
        std::cout << "Creation of the named pipe failed.\n The last error code: " << GetLastError() << "\n";
        std::cout << "Press any char to finish server: ";
        _getch();
        return (0);
    }
    while (true) {
        int chosen_option = 0;
        std::cout << "Choose option:\n 1. Modify data;\n 2. Read data;\n 3. Exit.\n";
        std::cin >> chosen_option;
        if (chosen_option == 3) {
            break;
        }
        else if (chosen_option == 1) {
            DWORD dwBytesWritten;
            DWORD dwBytesReaden;
            int ID;
            std::cout << "Input an ID of employee:\n";
            std::cin >> ID;
            int message_to_send = ID * 10 + chosen_option;
            bool checker = WriteFile(
                hPipe, // дескриптор канала
                &message_to_send, // данные
                sizeof(message_to_send), // размер данных
                &dwBytesWritten, // количество записанных байтов
                NULL);
            if (checker) {
                std::cout << "Message was sent.\n";
            }
            else {
                std::cout << "Message wasn't sent.\n";
            }
            employee* emp = new employee();
            ReadFile(
                hPipe, // дескриптор канала
                emp, // адрес буфера для ввода данных
                sizeof(employee), // количество читаемых байтов
                &dwBytesReaden, // количество прочитанных байтов
                NULL);

            std::cout << "ID of employee: " << emp->num << ".\nName of employee: " << emp->name << ".\nHours of employee: " << emp->hours << ".\n";
            std::cout << "Input new Name:\n";
            std::cin >> emp->name;
            std::cout << "Input new Hours:\n";
            std::cin >> emp->hours;
            checker = WriteFile(
                hPipe, // дескриптор канала
                emp, // данные
                sizeof(employee), // размер данных
                &dwBytesWritten, // количество записанных байтов
                NULL);
            if (checker) {
                std::cout << "Message was sent.\n";
            }
            else {
                std::cout << "Message wasn't sent.\n";
            }
            std::cout << "Press any key to confirm the option...\n";
            _getch();
            message_to_send = 1;
            WriteFile(hPipe, &message_to_send, sizeof(message_to_send), &dwBytesWritten, NULL);
        }
        else if (chosen_option == 2) {
            DWORD dwBytesWritten;
            DWORD dwBytesReaden;
            int ID;
            std::cout << "Input an ID of employee:\n";
            std::cin >> ID;
            int message_to_send = ID * 10 + chosen_option;
            bool checker = WriteFile(
                hPipe, // дескриптор канала
                &message_to_send, // данные
                sizeof(message_to_send), // размер данных
                &dwBytesWritten, // количество записанных байтов
                NULL);
            if (checker) {
                std::cout << "Message was sent.\n";
            }
            else {
                std::cout << "Message wasn't sent.\n";
            }
            employee* emp = new employee();
            ReadFile(
                hPipe, // дескриптор канала
                emp, // адрес буфера для ввода данных
                sizeof(employee), // количество читаемых байтов
                &dwBytesReaden, // количество прочитанных байтов
                NULL);
            
                std::cout << "ID of employee: " << emp->num << ".\nName of employee: " << emp->name << ".\nHours of employee: " << emp->hours <<".\n";
                std::cout << "Press any key to confirm the option...\n";
                _getch();
                message_to_send = 1;
                WriteFile(hPipe, &message_to_send,sizeof(message_to_send), &dwBytesWritten, NULL);
        }

    }
   
    

	return 0;
}