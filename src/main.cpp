#include <windows.h>
#include <commctrl.h>
#include <shlobj.h>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>
#include <iomanip>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shell32.lib")

// 윈도우 컨트롤 ID
#define ID_BUTTON_BACKUP 1001
#define ID_BUTTON_SELECT_SOURCE 1002
#define ID_BUTTON_SELECT_DEST 1003
#define ID_EDIT_SOURCE 1004
#define ID_EDIT_DEST 1005
#define ID_STATIC_STATUS 1006
#define ID_PROGRESS 1007

// 전역 변수
HWND g_hWnd;
HWND g_hEditSource;
HWND g_hEditDest;
HWND g_hStatus;
HWND g_hProgress;
HWND g_hButtonBackup;

std::wstring g_sourcePath = L"C:\\hanmi";
std::wstring g_destPath = L"C:\\hanmi_backup";

// 함수 선언
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void UpdateStatus(const std::wstring& status);
bool CreateDirectoryRecursive(const std::wstring& path);
bool CopyDirectoryRecursive(const std::wstring& source, const std::wstring& dest, int& fileCount);
void PerformBackup();
std::wstring GetCurrentTimestamp();
std::wstring BrowseForFolder(HWND hwnd, const std::wstring& title);

// 현재 타임스탬프를 문자열로 반환
std::wstring GetCurrentTimestamp() {
    time_t now = time(nullptr);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);

    std::wostringstream oss;
    oss << std::setfill(L'0')
        << std::setw(4) << (timeinfo.tm_year + 1900)
        << std::setw(2) << (timeinfo.tm_mon + 1)
        << std::setw(2) << timeinfo.tm_mday
        << L"_"
        << std::setw(2) << timeinfo.tm_hour
        << std::setw(2) << timeinfo.tm_min
        << std::setw(2) << timeinfo.tm_sec;

    return oss.str();
}

// 상태 업데이트
void UpdateStatus(const std::wstring& status) {
    SetWindowTextW(g_hStatus, status.c_str());
}

// 디렉토리 재귀적으로 생성
bool CreateDirectoryRecursive(const std::wstring& path) {
    if (PathFileExistsW(path.c_str())) {
        return true;
    }

    size_t pos = path.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        std::wstring parent = path.substr(0, pos);
        if (!CreateDirectoryRecursive(parent)) {
            return false;
        }
    }

    return CreateDirectoryW(path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
}

// 디렉토리 재귀적으로 복사
bool CopyDirectoryRecursive(const std::wstring& source, const std::wstring& dest, int& fileCount) {
    // 대상 디렉토리 생성
    if (!CreateDirectoryRecursive(dest)) {
        return false;
    }

    std::wstring searchPath = source + L"\\*";
    WIN32_FIND_DATAW findData;
    HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        return false;
    }

    do {
        std::wstring fileName = findData.cFileName;

        // . 과 .. 건너뛰기
        if (fileName == L"." || fileName == L"..") {
            continue;
        }

        std::wstring sourcePath = source + L"\\" + fileName;
        std::wstring destPath = dest + L"\\" + fileName;

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // 디렉토리인 경우 재귀 호출
            if (!CopyDirectoryRecursive(sourcePath, destPath, fileCount)) {
                FindClose(hFind);
                return false;
            }
        } else {
            // 파일인 경우 복사
            if (CopyFileW(sourcePath.c_str(), destPath.c_str(), FALSE)) {
                fileCount++;

                // 상태 업데이트
                std::wostringstream oss;
                oss << L"복사 중... (" << fileCount << L"개 파일 완료)";
                UpdateStatus(oss.str());

                // 메시지 처리 (UI 응답성 유지)
                MSG msg;
                while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            } else {
                std::wostringstream oss;
                oss << L"파일 복사 실패: " << fileName;
                UpdateStatus(oss.str());
            }
        }
    } while (FindNextFileW(hFind, &findData));

    FindClose(hFind);
    return true;
}

// 폴더 선택 대화상자
std::wstring BrowseForFolder(HWND hwnd, const std::wstring& title) {
    BROWSEINFOW bi = { 0 };
    bi.hwndOwner = hwnd;
    bi.lpszTitle = title.c_str();
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

    LPITEMIDLIST pidl = SHBrowseForFolderW(&bi);
    if (pidl != nullptr) {
        wchar_t path[MAX_PATH];
        if (SHGetPathFromIDListW(pidl, path)) {
            CoTaskMemFree(pidl);
            return std::wstring(path);
        }
        CoTaskMemFree(pidl);
    }

    return L"";
}

// 백업 수행
void PerformBackup() {
    // 소스 경로 확인
    if (!PathFileExistsW(g_sourcePath.c_str())) {
        MessageBoxW(g_hWnd, L"소스 폴더가 존재하지 않습니다!", L"오류", MB_OK | MB_ICONERROR);
        return;
    }

    // 백업 버튼 비활성화
    EnableWindow(g_hButtonBackup, FALSE);

    // 타임스탬프가 포함된 백업 폴더 생성
    std::wstring timestamp = GetCurrentTimestamp();
    std::wstring backupPath = g_destPath + L"\\" + timestamp;

    UpdateStatus(L"백업 시작 중...");

    // 백업 디렉토리 생성
    if (!CreateDirectoryRecursive(backupPath)) {
        MessageBoxW(g_hWnd, L"백업 폴더를 생성할 수 없습니다!", L"오류", MB_OK | MB_ICONERROR);
        EnableWindow(g_hButtonBackup, TRUE);
        return;
    }

    // 백업 수행
    int fileCount = 0;
    bool success = CopyDirectoryRecursive(g_sourcePath, backupPath, fileCount);

    if (success) {
        std::wostringstream oss;
        oss << L"백업 완료! " << fileCount << L"개 파일이 복사되었습니다.\n백업 위치: " << backupPath;
        MessageBoxW(g_hWnd, oss.str().c_str(), L"성공", MB_OK | MB_ICONINFORMATION);
        UpdateStatus(L"백업 완료");
    } else {
        MessageBoxW(g_hWnd, L"백업 중 오류가 발생했습니다!", L"오류", MB_OK | MB_ICONERROR);
        UpdateStatus(L"백업 실패");
    }

    // 백업 버튼 다시 활성화
    EnableWindow(g_hButtonBackup, TRUE);
}

// 윈도우 프로시저
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            // 폰트 생성
            HFONT hFont = CreateFontW(
                18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"맑은 고딕"
            );

            // 소스 폴더 레이블
            HWND hLabelSource = CreateWindowW(
                L"STATIC", L"백업할 폴더:",
                WS_CHILD | WS_VISIBLE | SS_LEFT,
                20, 20, 120, 25,
                hwnd, NULL, NULL, NULL
            );
            SendMessage(hLabelSource, WM_SETFONT, (WPARAM)hFont, TRUE);

            // 소스 폴더 경로 입력
            g_hEditSource = CreateWindowW(
                L"EDIT", g_sourcePath.c_str(),
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
                20, 50, 400, 30,
                hwnd, (HMENU)ID_EDIT_SOURCE, NULL, NULL
            );
            SendMessage(g_hEditSource, WM_SETFONT, (WPARAM)hFont, TRUE);

            // 소스 폴더 선택 버튼
            HWND hBtnSelectSource = CreateWindowW(
                L"BUTTON", L"찾아보기",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                430, 50, 100, 30,
                hwnd, (HMENU)ID_BUTTON_SELECT_SOURCE, NULL, NULL
            );
            SendMessage(hBtnSelectSource, WM_SETFONT, (WPARAM)hFont, TRUE);

            // 대상 폴더 레이블
            HWND hLabelDest = CreateWindowW(
                L"STATIC", L"백업 저장 위치:",
                WS_CHILD | WS_VISIBLE | SS_LEFT,
                20, 100, 120, 25,
                hwnd, NULL, NULL, NULL
            );
            SendMessage(hLabelDest, WM_SETFONT, (WPARAM)hFont, TRUE);

            // 대상 폴더 경로 입력
            g_hEditDest = CreateWindowW(
                L"EDIT", g_destPath.c_str(),
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
                20, 130, 400, 30,
                hwnd, (HMENU)ID_EDIT_DEST, NULL, NULL
            );
            SendMessage(g_hEditDest, WM_SETFONT, (WPARAM)hFont, TRUE);

            // 대상 폴더 선택 버튼
            HWND hBtnSelectDest = CreateWindowW(
                L"BUTTON", L"찾아보기",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                430, 130, 100, 30,
                hwnd, (HMENU)ID_BUTTON_SELECT_DEST, NULL, NULL
            );
            SendMessage(hBtnSelectDest, WM_SETFONT, (WPARAM)hFont, TRUE);

            // 백업 버튼
            g_hButtonBackup = CreateWindowW(
                L"BUTTON", L"백업 시작",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                170, 190, 200, 50,
                hwnd, (HMENU)ID_BUTTON_BACKUP, NULL, NULL
            );
            HFONT hFontLarge = CreateFontW(
                24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"맑은 고딕"
            );
            SendMessage(g_hButtonBackup, WM_SETFONT, (WPARAM)hFontLarge, TRUE);

            // 상태 표시 레이블
            g_hStatus = CreateWindowW(
                L"STATIC", L"대기 중...",
                WS_CHILD | WS_VISIBLE | SS_CENTER,
                20, 260, 510, 30,
                hwnd, (HMENU)ID_STATIC_STATUS, NULL, NULL
            );
            SendMessage(g_hStatus, WM_SETFONT, (WPARAM)hFont, TRUE);

            break;
        }

        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case ID_BUTTON_BACKUP: {
                    // 경로 가져오기
                    wchar_t source[MAX_PATH];
                    wchar_t dest[MAX_PATH];
                    GetWindowTextW(g_hEditSource, source, MAX_PATH);
                    GetWindowTextW(g_hEditDest, dest, MAX_PATH);

                    g_sourcePath = source;
                    g_destPath = dest;

                    PerformBackup();
                    break;
                }

                case ID_BUTTON_SELECT_SOURCE: {
                    std::wstring path = BrowseForFolder(hwnd, L"백업할 폴더를 선택하세요");
                    if (!path.empty()) {
                        SetWindowTextW(g_hEditSource, path.c_str());
                        g_sourcePath = path;
                    }
                    break;
                }

                case ID_BUTTON_SELECT_DEST: {
                    std::wstring path = BrowseForFolder(hwnd, L"백업 저장 위치를 선택하세요");
                    if (!path.empty()) {
                        SetWindowTextW(g_hEditDest, path.c_str());
                        g_destPath = path;
                    }
                    break;
                }
            }
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

// 메인 함수
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // COM 초기화
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    // 윈도우 클래스 등록
    const wchar_t CLASS_NAME[] = L"BackupProgramClass";

    WNDCLASSW wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassW(&wc);

    // 윈도우 생성
    g_hWnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"한미 폴더 백업 프로그램",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 570, 340,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (g_hWnd == NULL) {
        return 0;
    }

    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    // 메시지 루프
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CoUninitialize();
    return 0;
}
