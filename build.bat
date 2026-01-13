@echo off
REM 한미 폴더 백업 프로그램 빌드 스크립트

echo ========================================
echo 한미 폴더 백업 프로그램 빌드 시작
echo ========================================
echo.

REM 빌드 디렉토리 생성
if not exist build mkdir build
cd build

REM CMake 설정 및 빌드
echo CMake 설정 중...
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

if %ERRORLEVEL% NEQ 0 (
    echo CMake 설정 실패!
    echo.
    echo Visual Studio가 설치되어 있다면:
    echo cmake .. -G "Visual Studio 17 2022"
    echo.
    echo 또는 MinGW가 설치되어 있다면:
    echo cmake .. -G "MinGW Makefiles"
    pause
    exit /b 1
)

echo.
echo 빌드 중...
cmake --build . --config Release

if %ERRORLEVEL% NEQ 0 (
    echo 빌드 실패!
    pause
    exit /b 1
)

echo.
echo ========================================
echo 빌드 완료!
echo 실행 파일: build\bin\HanmiBackup.exe
echo ========================================
echo.

cd ..
pause
