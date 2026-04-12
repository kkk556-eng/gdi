
#define _WIN32_WINNT 0x0501 // Windows XP
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <mmsystem.h>

// ---------- BYTEBEAT ----------
DWORD WINAPI soundThread(LPVOID) {
    HWAVEOUT hWave;
    WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0};

    waveOutOpen(&hWave, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    const int size = 8000 * 20;
    static unsigned char buffer[size];

    int mode = rand() % 4;

    for (int t = 0; t < size; t++) {
        switch (mode) {
            case 0: buffer[t] = t * (t >> 5 | t >> 8); break;
            case 1: buffer[t] = t * ((t >> 11) & (t >> 8)); break;
            case 2: buffer[t] = (t * 5 & t >> 7) | (t * 3 & t >> 10); break;
            case 3: buffer[t] = (t >> 4) | (t * (t >> 5)); break;
        }
    }

    WAVEHDR hdr = { (LPSTR)buffer, size, 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWave, &hdr, sizeof(hdr));
    waveOutWrite(hWave, &hdr, sizeof(hdr));

    Sleep(20000);

    waveOutUnprepareHeader(hWave, &hdr, sizeof(hdr));
    waveOutClose(hWave);
    return 0;
}

// ---------- ЭФФЕКТЫ ----------
void effectText(HDC hdc, int w, int h) {
    SetTextColor(hdc, RGB(rand()%255, rand()%255, rand()%255));
    SetBkMode(hdc, TRANSPARENT);
    TextOutA(hdc, rand()%w, rand()%h, "GDI", 3);
}

void effectTunnel(HDC hdc, int w, int h) {
    StretchBlt(hdc, 10, 10, w-20, h-20, hdc, 0, 0, w, h, SRCCOPY);
}

void effectSmelt(HDC hdc, int w, int h) {
    int x = rand()%w;
    BitBlt(hdc, x, rand()%h, 120, 10, hdc, x, rand()%h - 10, SRCCOPY);
}

void effectPoly(HDC hdc, int w, int h) {
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(0,255,0));
    HBRUSH brush = CreateSolidBrush(RGB(0,255,0));

    HPEN oldPen = (HPEN)SelectObject(hdc, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

    POINT v[] = {
        {rand()%w, rand()%h},
        {rand()%w, rand()%h},
        {rand()%w, rand()%h}
    };

    Polygon(hdc, v, 3);

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);

    DeleteObject(pen);
    DeleteObject(brush);
}

// ---------- MAIN ----------
int main() {
    srand(time(0));

    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);

    HDC hdc = GetDC(0);

    int mode = 0;

    while (true) {
        DWORD start = GetTickCount();

        // 🔊 звук под каждый эффект
        CreateThread(0, 0, soundThread, 0, 0, 0);

        while (GetTickCount() - start < 20000) {
            if (GetAsyncKeyState(VK_ESCAPE)) {
                ReleaseDC(0, hdc);
                return 0;
            }

            switch (mode) {
                case 0: effectText(hdc, w, h); break;
                case 1: effectTunnel(hdc, w, h); break;
                case 2: effectSmelt(hdc, w, h); break;
                case 3: effectPoly(hdc, w, h); break;
            }

            Sleep(30);
        }

        mode = (mode + 1) % 4;
    }
}