//
//  LeakDetector.hpp
//
//
#ifndef __LEAK_DETECTOR__
#define __LEAK_DETECTOR__

void* operator new(size_t size, char *fileName, unsigned int lineNum);
void* operator new[](size_t size, char *fileName, unsigned int lineNum);

#ifndef __NEW_OVERLOAD_IMPLEMENTATION__
#define new	new(__FILE__, __LINE__)
#endif

class LeakDetector
{
public:
    static unsigned int callCount;
    LeakDetector() noexcept {
        ++callCount;
    }
    ~LeakDetector() noexcept {
        if (--callCount == 0)
            Detect();
    }
private:
    static unsigned int Detect() noexcept;
};

#endif
