#include <windows.h>
int main() {
    HDC hdc = GetDC(NULL);
    while (true) {
        BitBlt(hdc, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), hdc, 10, 10, NOTSRCCOPY);
        Sleep(100);
    }
    ReleaseDC(NULL, hdc);
    return 0;
}
