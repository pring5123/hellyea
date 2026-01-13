@echo off
REM Visual Studio용 한미 폴더 백업 프로그램 빌드 스크립트

echo ========================================
echo 한미 폴더 백업 프로그램 빌드 (Visual Studio)
echo ========================================
echo.

REM 빌드 디렉토리 생성
if not exist build mkdir build
cd build

REM CMake 설정
echo CMake 설정 중...
cmake .. -G "Visual Studio 17 2022" -A x64

if %ERRORLEVEL% NEQ 0 (
    echo CMake 설정 실패!
    echo.
    echo 다른 Visual Studio 버전을 사용한다면:
    echo Visual Studio 2019: cmake .. -G "Visual Studio 16 2019" -A x64
    echo Visual Studio 2022: cmake .. -G "Visual Studio 17 2022" -A x64
    echo.
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
echo 실행 파일: build\bin\Release\HanmiBackup.exe
echo ========================================
echo.

REM 실행 파일을 루트 디렉토리로 복사
if exist bin\Release\HanmiBackup.exe (
    copy bin\Release\HanmiBackup.exe ..\HanmiBackup.exe
    echo 실행 파일을 루트 디렉토리로 복사했습니다.
)

cd ..
pause
