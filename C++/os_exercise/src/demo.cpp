#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <string>

// 跨平台宏定义
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

// 定义GiB单位（编译期计算）
constexpr size_t GiB = 1024ULL * 1024 * 1024;

// 跨平台内存映射类
class MemoryMapper {
private:
    volatile uint8_t* mapped_ptr = nullptr;
    size_t map_size = 0;
#ifdef _WIN32
    HANDLE hFile = INVALID_HANDLE_VALUE;
    HANDLE hMap = nullptr;
#endif

public:
    // 映射指定大小的内存
    bool map(size_t size) {
        map_size = size;
#ifdef _WIN32
        // Windows实现：使用匿名文件映射
        hFile = CreateFileA("\\\\.\\PHYSICALDRIVE0", 0, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
        if (hFile == INVALID_HANDLE_VALUE) {
            // 匿名映射失败时 fallback 到页面文件
            hFile = CreateFileMappingA(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 
                                      static_cast<DWORD>(size >> 32), static_cast<DWORD>(size), nullptr);
        }
        
        if (hFile == INVALID_HANDLE_VALUE) {
            return false;
        }

        hMap = MapViewOfFile(hFile, FILE_MAP_ALL_ACCESS, 0, 0, size);
        if (hMap == nullptr) {
            CloseHandle(hFile);
            hFile = INVALID_HANDLE_VALUE;
            return false;
        }

        mapped_ptr = reinterpret_cast<volatile uint8_t*>(hMap);
        return true;
#else
        // Linux实现：使用mmap
        mapped_ptr = reinterpret_cast<volatile uint8_t*>(
            mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)
        );
        return mapped_ptr != reinterpret_cast<volatile uint8_t*>(-1);
#endif
    }

    // 获取映射的内存指针
    volatile uint8_t* get_ptr() const {
        return mapped_ptr;
    }

    // 释放映射的内存
    void unmap() {
        if (mapped_ptr == nullptr) return;

#ifdef _WIN32
        UnmapViewOfFile(hMap);
        CloseHandle(hFile);
        hMap = nullptr;
        hFile = INVALID_HANDLE_VALUE;
#else
        munmap(const_cast<uint8_t*>(mapped_ptr), map_size);
#endif
        mapped_ptr = nullptr;
        map_size = 0;
    }

    // 获取错误信息
    std::string get_last_error() const {
#ifdef _WIN32
        DWORD err = GetLastError();
        char* msg_buf = nullptr;
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                      nullptr, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                      reinterpret_cast<char*>(&msg_buf), 0, nullptr);
        std::string msg(msg_buf ? msg_buf : "Unknown error");
        LocalFree(msg_buf);
        return msg;
#else
        return std::string(strerror(errno));
#endif
    }

    ~MemoryMapper() {
        unmap(); // 析构时自动释放资源
    }
};

int main(int argc, char* argv[]) {
    constexpr size_t map_size = 8 * GiB;
    MemoryMapper mapper;

    // 执行内存映射
    if (!mapper.map(map_size)) {
        std::cerr << "内存映射失败: " << mapper.get_last_error() << std::endl;
        return EXIT_FAILURE;
    }

    // 输出映射地址（十六进制）
    auto ptr = mapper.get_ptr();
    std::cout << "映射地址: 0x" << std::hex << reinterpret_cast<uintptr_t>(ptr) << std::dec << std::endl;

    // 写入数据
    *(ptr + 2 * GiB) = 1;
    *(ptr + 4 * GiB) = 2;
    *(ptr + 7 * GiB) = 3;

    // 读取数据并输出
    std::cout << "读取 2GiB 位置: " << static_cast<int>(*(ptr + 2 * GiB)) << std::endl;
    std::cout << "读取 4GiB 位置: " << static_cast<int>(*(ptr + 4 * GiB)) << std::endl;
    std::cout << "读取 6GiB 位置: " << static_cast<int>(*(ptr + 6 * GiB)) << std::endl; // 未初始化，值不确定
    std::cout << "读取 7GiB 位置: " << static_cast<int>(*(ptr + 7 * GiB)) << std::endl;

    // 自动释放资源（析构函数调用unmap）
    return 0;
}
