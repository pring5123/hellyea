# 한미 폴더 백업 프로그램

C:\hanmi 폴더를 백업하는 간단한 GUI 백업 프로그램입니다.

## 주요 기능

- **GUI 인터페이스**: 사용하기 쉬운 Windows GUI 프로그램
- **폴더 선택**: 백업할 폴더와 저장 위치를 자유롭게 선택 가능
- **자동 타임스탬프**: 백업할 때마다 타임스탬프가 포함된 폴더 생성
- **재귀 복사**: 하위 폴더와 파일을 모두 백업
- **진행 상황 표시**: 백업 진행 상황을 실시간으로 확인

## 시스템 요구사항

- Windows 7 이상
- C++ 컴파일러 (Visual Studio 또는 MinGW)
- CMake 3.15 이상

## 빌드 방법

### 방법 1: 빌드 스크립트 사용 (권장)

```batch
build.bat
```

### 방법 2: 수동 빌드

#### Visual Studio 사용

```batch
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

#### MinGW 사용

```batch
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

빌드가 완료되면 `build\bin\HanmiBackup.exe` 파일이 생성됩니다.

## 사용 방법

1. `HanmiBackup.exe` 실행
2. 백업할 폴더 경로 확인/변경 (기본값: `C:\hanmi`)
3. 백업 저장 위치 확인/변경 (기본값: `C:\hanmi_backup`)
4. "백업 시작" 버튼 클릭
5. 백업 완료 대화상자 확인

## 백업 구조

백업은 다음과 같은 구조로 저장됩니다:

```
C:\hanmi_backup\
├── 20260113_143025\  (타임스탬프 형식: YYYYMMDD_HHMMSS)
│   └── [백업된 파일들]
├── 20260113_150130\
│   └── [백업된 파일들]
└── ...
```

## 프로젝트 구조

```
hellyea/
├── src/
│   └── main.cpp          # 메인 프로그램 소스 코드
├── CMakeLists.txt        # CMake 빌드 설정
├── build.bat             # Windows 빌드 스크립트
└── README.md             # 이 파일
```

## 기술 스택

- **언어**: C++ (C++17)
- **GUI**: Windows API
- **빌드 시스템**: CMake

## 특징

### 안전한 백업

- 원본 파일은 절대 수정하지 않음
- 타임스탬프로 여러 버전 관리 가능
- 백업 실패 시 오류 메시지 표시

### 사용자 친화적

- 직관적인 GUI 인터페이스
- 한글 지원
- 실시간 진행 상황 표시
- 폴더 찾아보기 기능

## 문제 해결

### 빌드 오류

- CMake가 설치되어 있는지 확인하세요
- C++ 컴파일러(Visual Studio 또는 MinGW)가 설치되어 있는지 확인하세요
- PATH 환경 변수가 올바르게 설정되어 있는지 확인하세요

### 실행 오류

- Windows 7 이상에서 실행하세요
- 백업할 폴더에 읽기 권한이 있는지 확인하세요
- 백업 저장 위치에 쓰기 권한이 있는지 확인하세요

## 라이선스

이 프로젝트는 자유롭게 사용, 수정, 배포할 수 있습니다.
