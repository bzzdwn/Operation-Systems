//#include <fstream>
//#include <windows.h>
//#include <conio.h>
//#include <iostream>
//
//int main(int argc, char* argv[]) {
//    std::string file_name = argv[1];
//    std::fstream file;
//    HANDLE hStartEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, L"Process Started");
//    if (hStartEvent == NULL)
//    {
//        std::cout << "Open event failed. \nInput any char to exit.\n";
//        std::cin.get();
//        return GetLastError();
//    }
//    HANDLE hInputReadySemaphore = OpenSemaphore(SEMAPHORE_MODIFY_STATE, FALSE, L"Input Semaphore started");
//    if (hInputReadySemaphore == NULL)
//        return GetLastError();
//    /*HANDLE hOutputReadySemaphore = OpenSemaphore(EVENT_ALL_ACCESS, FALSE, L"Output Semaphore started");
//    if (hOutputReadySemaphore == NULL)
//        return GetLastError();*/
//    HANDLE hOutput = OpenEvent(EVENT_ALL_ACCESS, 0,
//        L"read");
//    HANDLE hMutex = OpenMutex(SYNCHRONIZE, FALSE, L"mut ex");
//    SetEvent(hStartEvent);
//    std::cout << "Event was started\n";
//    std::cout << "Input 1 to write message;\nInput 0 to exit process\n";
//    int key;
//    std::cin >> key;
//    file.open(file_name, std::ios::binary | std::ios::out | std::ios::app);
//    while (true) {
//        if (key == 1) {
//            WaitForSingleObject(hMutex, INFINITE);
//
//                std::cout << "Input message to add\n";
//
//                std::string msg;
//                std::cin >> msg;
//                char message[20];
//                for (int i = 0; i < msg.length(); i++)
//                {
//                    message[i] = msg[i];
//                }
//                for (int i = msg.length(); i < 20; i++)
//                {
//                    message[i] = '\0';
//                }
//                message[19] = '\n';
//                file.write((char*)&message, sizeof(message));
//            ReleaseMutex(hMutex);
//            if (ReleaseSemaphore(hInputReadySemaphore, 1, NULL) != 1) {
//                std::cout << "file is full";
//                //ReleaseSemaphore(hOutputReadySemaphore, 1, NULL);
//                ResetEvent(hOutput);
//                WaitForSingleObject(hOutput, INFINITE);
//                ReleaseSemaphore(hInputReadySemaphore, 1, NULL);
//            }
//            std::cout << "\nInput 1 to write message;\nInput 0 to exit process\n";
//            std::cin >> key;
//        }
//        if (key != 0 && key != 1) {
//            std::cout << "\nIncorrect value!\nInput 1 to write message;\nInput 0 to exit process\n";
//            std::cin >> key;
//        }
//        if (key == 0) {
//            std::cout << "Process ended.";
//            break;
//        }
//    }
//    file.close();
//    return 0;
//}

#include <fstream>
#include <windows.h>
#include <conio.h>
#include <iostream>

int main(int argc, char* argv[]) {
    std::string file_name = argv[1];
    std::fstream file;
    HANDLE hStartEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, L"Process Started");
    if (hStartEvent == NULL)
    {
        std::cout << "Open event failed. \nInput any char to exit.\n";
        std::cin.get();
        return GetLastError();
    }
    HANDLE hInputReadySemaphore = OpenSemaphore(EVENT_ALL_ACCESS, FALSE, L"Input Semaphore started");
    if (hInputReadySemaphore == NULL)
        return GetLastError();
    HANDLE hOutputReadySemaphore = OpenSemaphore(EVENT_ALL_ACCESS, FALSE, L"Output Semaphore started");
    if (hOutputReadySemaphore == NULL)
        return GetLastError();
    HANDLE hMutex = OpenMutex(SYNCHRONIZE, FALSE, L"mut ex");
    SetEvent(hStartEvent);
    std::cout << "Event was started\n";
    std::cout << "Input 1 to write message;\nInput 0 to exit process\n";
    int key;
    std::cin >> key;
    while (true) {
        if (key == 1) {
            //WaitForSingleObject(hInputReadySemaphore, INFINITE);
            WaitForSingleObject(hMutex, INFINITE);
            file.open(file_name, std::ios::out | std::ios::app);
            std::string msg;
            std::cout << "Input message to add\n";
            std::cin >> msg;
            char message[20];
            for (int i = 0; i < msg.length(); i++)
            {
                message[i] = msg[i];
            }
            for (int i = msg.length(); i < 20; i++)
            {
                message[i] = '\0';
            }
            //ReleaseSemaphore(hInputReadySemaphore, 1, NULL);
            message[19] = '\n';
            ReleaseMutex(hMutex);
            ReleaseSemaphore(hOutputReadySemaphore, 1, NULL);
            if (ReleaseSemaphore(hInputReadySemaphore, 1, NULL) != 1) {
                std::cout << "file is full";
                ReleaseSemaphore(hOutputReadySemaphore, 1, NULL);
                WaitForSingleObject(hOutputReadySemaphore, INFINITE);
                ReleaseSemaphore(hOutputReadySemaphore, 1, NULL);
                ReleaseSemaphore(hInputReadySemaphore, 1, NULL);
                for (int i = 0; i < 20; i++)
                {
                    file << message[i];
                }
                
            }
            else {
                for (int i = 0; i < 20; i++)
                {
                    file << message[i];
                }
            }
            file.close();
            std::cout << "\nInput 1 to write message;\nInput 0 to exit process\n";
            std::cin >> key;
        }
        if (key != 0 && key != 1) {
            std::cout << "\nIncorrect value!\nInput 1 to write message;\nInput 0 to exit process\n";
            std::cin >> key;
        }
        if (key == 0) {
            std::cout << "Process ended.";
            break;
        }
    }

    return 0;
}