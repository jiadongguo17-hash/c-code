#include <iostream>
#include <iomanip>
#include <fstream> // 用于文件交互
#include <cstdlib>  // 用于清屏
#include <limits>   // 添加此行
#include<string> 
using namespace std; 
// 客房信息结构体
struct Room 
{
    int roomNumber;                // 客房号
    string roomType;          // 客房类型
    double roomPrice;              // 客房价格
    double discount;               // 折扣情况
    string guestName;         // 入住客人姓名
    char gender;                   // 入住客人性别
    string idNumber;          // 入住客人身份证号
    string checkInDate;       // 入住时间
    string expectedCheckOutDate;  // 预计入住期
    string roomManager;       // 客房负责人
    bool occupied;                 // 客房入住状态
};

const int MAX_ROOMS = 30;; // 最大客房数量

// 酒店管理系统基类
class HotelManagementSystemBase 
{
protected:
    Room rooms[MAX_ROOMS]; // 客房信息数组
    int numRooms;          // 当前客房数量

public:
    HotelManagementSystemBase() : numRooms(0) {}

    virtual ~HotelManagementSystemBase() {}

    virtual void displayMenu() = 0;//菜单界面 

    virtual void addGuestInfo() = 0;//添加客人信息 

    virtual void displayGuestInfo() = 0;//展示客人信息 

    virtual void deleteGuestInfo() = 0;//删除客人信息 

    virtual void modifyGuestInfo() = 0;//修改客人信息 

    virtual void searchGuestInfo() = 0;//查找客人信息 

    virtual void sortRoomsByNumber() = 0;//按编号排序 

    virtual void clearAllInfo() = 0;//清除所有信息 
    virtual void generateMonthlyReport() = 0;//月报表 
    virtual void clearScreen() const = 0;//清屏操作 


// 在 HotelManagementSystemBase 类中添加以下虚拟函数
virtual void displayAllRooms()=0;

    virtual void saveToFile(const string& filename) {
        ofstream outFile("filename",ios::out);
        if (outFile.is_open()) {
            for (int i = 0; i < numRooms; ++i) {
                outFile << rooms[i].roomNumber << " "
                        << rooms[i].roomType << " "
                        << rooms[i].roomPrice << " "
                        << rooms[i].discount << " "
                        << rooms[i].guestName << " "
                        << rooms[i].gender << " "
                        << rooms[i].idNumber << " "
                        << rooms[i].checkInDate << " "
                        << rooms[i].expectedCheckOutDate << " "
                        << rooms[i].roomManager << " "
                        << rooms[i].occupied << "\n";
            }
            outFile.close();
            cout << "数据成功保存到文件 " << filename << " 中。\n";
        } else 
		{
            std::cerr << "无法打开文件 " << filename << " 进行写入。\n";//cerr表示输出错误 
        }
    }

    virtual void loadFromFile(const std::string& filename) {
        ifstream inFile("filename",ios::in);
        if (inFile.is_open()) {
            numRooms = 0;
            while (inFile >> rooms[numRooms].roomNumber
                           >> rooms[numRooms].roomType
                           >> rooms[numRooms].roomPrice
                           >> rooms[numRooms].discount
                           >> rooms[numRooms].guestName
                           >> rooms[numRooms].gender
                           >> rooms[numRooms].idNumber
                           >> rooms[numRooms].checkInDate
                           >> rooms[numRooms].expectedCheckOutDate
                           >> rooms[numRooms].roomManager
                           >> rooms[numRooms].occupied) 
						   {
                ++numRooms;
                if (numRooms >= MAX_ROOMS) {
                    std::cerr << "文件中的客房信息数量超过最大限制，部分数据未加载。\n";
                    break;
                }
            }
            inFile.close();
            cout << "数据成功从文件 " << filename << " 中加载。\n";
        } else {
            cerr << "无法打开文件 " << filename << " 进行读取。\n";//cerr表示输出错误 
        }
    }
};

// 酒店管理系统类，继承基类
class HotelManagementSystem : public HotelManagementSystemBase {
public:
    void displayMenu()  {
        cout << "******欢迎使用酒店管理系统!*****\n"
             << "********0.退出管理程序**********\n"
             << "********1.增加客人信息**********\n"
             << "********2.显示客人信息**********\n"
             << "********3.删除离开客人**********\n"
             << "********4.修改客人信息**********\n"
             << "********5.查找客人信息**********\n"
             << "********6.按照编号排序**********\n"
             << "********7.清空所有信息**********\n"
             << "********8.保存数据到文件********\n"
             << "********9.从文件加载数据********\n"
             << "********10.客房入住月报表*******\n"
             << "********11.显示所有客房*********\n";  

    }
//override
int findRoomIndex(int roomNumber) {
    for (int i = 0; i < numRooms; ++i) {
        if (rooms[i].roomNumber == roomNumber) {
            return i;
        }
    }
    return -1;
}

void displayRoomInfo(const Room& room) {
    std::cout << "客房号: " << room.roomNumber << "\n"
              << "客房类型: " << room.roomType << "\n"
              << "客房价格: " << room.roomPrice << "\n"
              << "折扣情况: " << room.discount << "\n"
              << "客房状态: " << (room.occupied ? "已入住" : "未入住") << "\n";

    if (room.occupied) {
        std::cout << "入住客人信息:\n"
                  << "姓名: " << room.guestName << "\n"
                  << "性别: " << room.gender << "\n"
                  << "身份证号: " << room.idNumber << "\n"
                  << "入住时间: " << room.checkInDate << "\n"
                  << "预计入住期: " << room.expectedCheckOutDate << "\n"
                  << "客房负责人: " << room.roomManager << "\n";
    }

    std::cout << "------------------------\n";
}

// 实现添加客人信息的逻辑
void addGuestInfo(){
    if (numRooms < MAX_ROOMS) {
        Room newRoom;
        // 从标准输入获取客房信息
        std::cout << "请输入客房号: ";
        std::cin >> newRoom.roomNumber;

        // 省略其他信息的输入，根据需要添加
        std::cout << "请输入客房类型: ";
        std::cin >> newRoom.roomType;
        std::cout << "请输入客房价格: ";
        std::cin >> newRoom.roomPrice;
        std::cout << "请输入折扣情况: ";
        std::cin >> newRoom.discount;
        std::cout << "请输入客房状态 (1: 已入住, 0: 未入住): ";
        std::cin >> newRoom.occupied;

        if (newRoom.occupied) {
            std::cout << "请输入入住客人信息:\n";
            std::cout << "姓名: ";
            std::cin >> newRoom.guestName;
            std::cout << "性别 (M/F): ";
            std::cin >> newRoom.gender;
            std::cout << "身份证号: ";
            std::cin >> newRoom.idNumber;
            std::cout << "入住时间: ";
            std::cin >> newRoom.checkInDate;
            std::cout << "预计入住期: ";
            std::cin >> newRoom.expectedCheckOutDate;
            std::cout << "客房负责人: ";
            std::cin >> newRoom.roomManager;
        }

        rooms[numRooms++] = newRoom;
        std::cout << "客人信息添加成功！\n";
    } else {
        std::cout << "客房已满，无法添加新客人信息。\n";
    }
}

// 实现显示客人信息的逻辑
void displayGuestInfo() {
    if (numRooms == 0) {
        std::cout << "当前没有客人信息。\n";
    } else {
        std::cout << "客房信息如下:\n";
        for (int i = 0; i < numRooms; ++i) {
            displayRoomInfo(rooms[i]);
        }
    }
}

// 实现删除客人信息的逻辑
void deleteGuestInfo() {
    int roomNumberToDelete;
    std::cout << "请输入要删除的客房号: ";
    std::cin >> roomNumberToDelete;

    int index = findRoomIndex(roomNumberToDelete);
    if (index != -1) {
        // 将该客房后面的客房信息向前移动
        for (int i = index; i < numRooms - 1; ++i) {
            rooms[i] = rooms[i + 1];
        }
        --numRooms;
        std::cout << "客人信息删除成功！\n";
    } else {
        std::cout << "未找到对应客房号的客人信息。\n";
    }
}

// 实现修改客人信息的逻辑
void modifyGuestInfo(){
    int roomNumberToModify;
    std::cout << "请输入要修改的客房号: ";
    std::cin >> roomNumberToModify;

    int index = findRoomIndex(roomNumberToModify);
    if (index != -1) {
        // 根据需要修改客房信息
        std::cout << "请输入修改后的客人姓名: ";
        std::cin >> rooms[index].guestName;
        std::cout << "客人信息修改成功！\n";
    } else {
        std::cout << "未找到对应客房号的客人信息。\n";
    }
}

// 实现查找客人信息的逻辑
void searchGuestInfo() {
    int roomNumberToSearch;
    std::cout << "请输入要查找的客房号: ";
    std::cin >> roomNumberToSearch;

    int index = findRoomIndex(roomNumberToSearch);
    if (index != -1) {
        std::cout << "找到客人信息:\n";
        displayRoomInfo(rooms[index]);
    } else {
        std::cout << "未找到对应客房号的客人信息。\n";
    }
}

// 实现按照编号排序的逻辑
void sortRoomsByNumber() {
    // 使用简单的冒泡排序按照客房号排序
    for (int i = 0; i < numRooms - 1; ++i) {
        for (int j = 0; j < numRooms - i - 1; ++j) {
            if (rooms[j].roomNumber > rooms[j + 1].roomNumber) {
                std::swap(rooms[j], rooms[j + 1]);
            }
        }
    }
    std::cout << "客房按照编号排序成功！\n";
}

// 实现清空所有信息的逻辑
void clearAllInfo() {
    numRooms = 0;
    std::cout << "所有信息已清空！\n";
}

    // 实现客房入住月报表的逻辑
    void generateMonthlyReport() {
        std::cout << "客房入住月报表:\n";

        for (int i = 0; i < numRooms; ++i) {
            if (rooms[i].occupied) {
                std::cout << "客房号: " << rooms[i].roomNumber << "\n"
                          << "客人姓名: " << rooms[i].guestName << "\n"
                          << "入住时间: " << rooms[i].checkInDate << "\n"
                          << "预计入住期: " << rooms[i].expectedCheckOutDate << "\n"
                          << "------------------------\n";
            }
        }
    }

// 实现显示所有客房的逻辑
void displayAllRooms()  {
    std::cout << "所有客房信息如下:\n";
    for (int i = 101; i <= 130; ++i) {
        int index = findRoomIndex(i);
        if (index != -1) {
            std::cout << "客房号: " << rooms[index].roomNumber
                      << ", 客房状态: " << (rooms[index].occupied ? "已入住" : "未入住") << "\n";
        } else {
            std::cout << "客房号: " << i << ", 客房状态: 未入住\n";
        }
    }
}

    // 清屏函数
 /*   void clearScreen() {
        system("cls");

        //system("clear");

    }*/
    void clearScreen() const  {
        cout << "按下任意键以继续...";
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        cin.get();
        system("cls");
    }

};

int main() {
    HotelManagementSystem hotelSystem;

    int choice;
    do {
    	

        hotelSystem.displayMenu();
        cout << "请输入您的选择: ";
        cin >> choice;

        switch (choice) {
            case 0:
                std::cout << "感谢使用酒店管理系统，再见！\n";
                break;
            case 1:
                hotelSystem.addGuestInfo();
                break;
            case 2:
                hotelSystem.displayGuestInfo();
                break;
            case 3:
                hotelSystem.deleteGuestInfo();
                break;
            case 4:
                hotelSystem.modifyGuestInfo();
                break;
            case 5:
                hotelSystem.searchGuestInfo();
                break;
            case 6:
                hotelSystem.sortRoomsByNumber();
                break;
            case 7:
                hotelSystem.clearAllInfo();
                break;
            case 8: {
                string filename;
                cout << "请输入要保存数据的文件名: ";
                cin >> filename;
                hotelSystem.saveToFile(filename);
                break;
            }
            case 9: {
                string filename;
                cout << "请输入要加载数据的文件名: ";
                cin >> filename;
                hotelSystem.loadFromFile(filename);
                break;
            }
            case 10:
			    hotelSystem.generateMonthlyReport(); 
			    break;
			case 11:
			    hotelSystem.displayAllRooms();
			    break;
            default:
                cout << "无效的选择，请重新输入。\n";
        }
                hotelSystem.clearScreen();  // 在每次循环开始前清屏

    } while (choice != 0);

    return 0;
}
/* 
// 实现添加客人信息的逻辑
void HotelManagementSystem::addGuestInfo() {

}

// 实现显示客人信息的逻辑
void HotelManagementSystem::displayGuestInfo() {

}

// 实现删除客人信息的逻辑
void HotelManagementSystem::deleteGuestInfo() {

}

// 实现修改客人信息的逻辑
void HotelManagementSystem::modifyGuestInfo() {

}

// 实现查找客人信息的逻辑
void HotelManagementSystem::searchGuestInfo() {

}

// 实现按照编号排序的逻辑
void HotelManagementSystem::sortRoomsByNumber() {

}

// 实现清空所有信息的逻辑
void HotelManagementSystem::clearAllInfo() {

}*/


