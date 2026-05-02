/*  位图 (Bitmap) 是一种基于位操作的数据结构，用于表示一组元素的集合信息。
它通常是一个仅包含0和1的数组，其中每个元素对应集合中的一个元素。
位图中的每个位（或者可以理解为数组的元素）代表一个元素是否存在于集合中。
当元素存在时，对应位的值为1；不存在时，对应位的值为0。

可能这么多听起来很复杂，其实总结下来就这个意思：
1.位图本质是个数组，用来存放0和1。
2.位图通过自身数组中的每个位来代表集合(我们要处理的数据)中的元素，每个位是0或1，代表元素的存在与否(0,不存在；1,存在)。*/
                   
#include <iostream>
#include <fstream>//文件读写
#include <cstring>
#include <cstdint>//uint32_t
using namespace std;

typedef struct {
    unsigned char* bitmap;
    int size;
} Bitmap;
// -------------------------
// 工具函数：操作位图中的 bit
// -------------------------
//uint32_t 是无符号的32位整数类型，表示非负整数。
void setBit(unsigned char* bitmap, uint32_t index) {
    //index >> 3: 将 index 右移 3 位，相当于 index / 8。
    // 因为每个 unsigned char 有 8 位，所以这个操作确定了 index 所在的字节位置。
    //index & 7: 计算 index 在其所在字节中的位位置。相当于index%8。
    // 7 的二进制是 00000111，所以 index & 7 会保留 index 的最后 3 位，表示位在字节中的偏移量。
    //是按位或赋值运算符。它的作用是将左边的操作数与右边的操作数进行按位或操作，然后将结果赋值给左边的操作数。
    bitmap[index >> 3] |= (1 << (index & 7));
}

bool testBit(const unsigned char* bitmap, uint32_t index) {
    //查找num是否存在
    if ((bitmap[index >> 3] & (1 << (index & 7))) != 0) return true;
    else return false;
}

//文件名，位图，文件开始位置，读取字节数
void processFileChunk(const char* fileName, unsigned char* bitmap, uint64_t start, uint64_t chunkSize) {
    // 以二进制形式读取文件
    ifstream fin(fileName, ios::in | ios::binary);
    if (!fin) {
        // 读取失败
        cerr << "文件无法打开: " << fileName << endl;
        return;
    }

    //将文件的读取指针移动到文件的指定位置，以便从该位置开始读取数据。
    // 定位到文件的起始位置
    fin.seekg(start, ios::beg);

    uint32_t value; // 用于存储从文件中读取的 4 个字节的数据
    uint64_t bytesRead = 0; // 用于记录已经读取的字节数

    // 循环读取数据，直到读取的字节数达到 chunkSize
    while (bytesRead < chunkSize) {
        value = 0; // 每次读取前清空 value

        // 逐字节读取 4 个字节
        for (size_t i = 0; i < sizeof(value); ++i) {
            char byte;
            if (fin.get(byte)) {
                // 将读取的字节存储到 value 的对应位置  static_cast<type>(value)将变量value强制转换成type类型
                //将static_cast<unsigned char>(byte)强制转换成 uint32_t然后再左移i*8位
                value |= static_cast<uint32_t>(static_cast<unsigned char>(byte)) << (i * 8);
            }
            else {
                // 如果读取失败，退出循环
                break;
            }
        }

        // 如果成功读取 4 个字节，调用 setBit 函数
        //文件处于正常状态可以继续读取
        if (fin.good()) {
            setBit(bitmap, value);
            bytesRead += sizeof(value);
        }
        else {
            // 如果读取失败，退出循环
            break;
        }
    }
    //关闭文件
    fin.close();
}

// -------------------------
// 主函数
// -------------------------
int main() {
    const char* inputFileName = "test.bin";

    // 打开文件获取大小
    //ios::ate 用于在打开文件时将文件指针定位到文件末尾。
    ifstream fin(inputFileName, ios::in | ios::binary | ios::ate);
    if (!fin) {
        cerr << "无法打开文件" << inputFileName << endl;
        return 1;
    }

    //获取当前文件的大小（以字节为单位）
    uint64_t fileSize = fin.tellg();
    //关闭文件
    fin.close();

    // 为位图分配内存，2^32/8（以字节为单位）
    const uint64_t BITMAP_SIZE = (1ULL << 32) / 8;
    unsigned char* bitmap = NULL;
    //分配失败时返回nullptr
    bitmap = new (nothrow) unsigned char[BITMAP_SIZE];
    if (bitmap == nullptr) {
        cerr << "位图内存分配失败！" << endl;
        return 1;
    }
    //初始化位图全部为0
    memset(bitmap, 0, BITMAP_SIZE);

    // 处理整个文件
    processFileChunk(inputFileName, bitmap, 0, fileSize);

    // 查找缺失的数
    cout << "缺失的数有：" << endl;

    //从0循环到2^32-1
    for (uint64_t i = 0; i < (1ULL << 32); i++) {
        if (!testBit(bitmap, static_cast<uint32_t>(i))) {
            //打印不存在的数
            cout << i << endl;
        }
    }

    // 释放资源
    delete[] bitmap;
    bitmap = nullptr;

    return 0;
}