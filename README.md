# LogAdmin

# 목차
1. 프로젝트 소개
2. 설계
3. 주요기능
4. 라이브러리 사용법

# 1. 🎊 프로젝트 소개

## LogAdmin은?

`서브 프로그램에서 로그를 추출 및 사용자가 등록한 프로세스가 종료되지 않도록 감시, 종료되면 자동 재실행해 주는 프로그램입니다.`

## 기획 배경

디스플레이 패널 검사 과정에는 제조공정에서 발생하는 각종 결함을 검출하기 위해 다양한 장비들이 사용되고 있습니다.<br>
각각의 검사 장비들이 운영되면서 대량의 로그가 생성이 되는데 이를 작업자가 일일이 확인하기에는 많은 어려움이 발생합니다.<br>
따라서 이를 한 곳에서 관리할 수 있는 프로그램을 만들게 되었습니다.<br>


# 2. 🛠 설계
<details>
<summary><b>설계</b></summary>

<a href="https://ibb.co/MsdS9X0"><img src="https://i.ibb.co/0c52Qwb/123.png" alt="123" border="0"></a>
  
**개발 일정**

---
<a href="https://ibb.co/fv3t2Rn"><img src="https://i.ibb.co/gV0w31P/1.png" alt="1" border="0"></a>

**프로그램 흐름도**

---
<a href="https://ibb.co/gZD37d4"><img src="https://i.ibb.co/fdC2S9D/11.png" alt="11" border="0"></a>

**프로그램 상세 흐름도**
</details>

# 3. 🎇 주요 기능
<details>
<summary><b>주요 기능</b></summary>
<a href="https://ibb.co/YkW1bSF"><img src="https://i.ibb.co/tshv36T/2.png" alt="2" border="0"></a>

**메인 화면** <br>
`1번 버튼`을 통해 디렉토리에서 현재 실행중인 프로세스를 등록한다. 삭제할 수도 있다. <br>
등록이 되면 로그를 레벨에 따라 볼 수 있고 등록한 프로세스 별로도 볼 수 있다. <br>
검색 기능도 제공이 된다. <br>

---
<a href="https://ibb.co/gjkQpWp"><img src="https://i.ibb.co/QbyhRHR/3.png" alt="3" border="0"></a>

**프로세스 등록 후** <br>
왼쪽은 두 개의 프로세스를 등록한 모습이고 재실행된 횟수를 확인할 수 있다. <br>
로그 레벨에 따라 색상을 다르게 하였다. <br>
오른쪽은 로그 필터를 적용한 모습이다. <br>
  
---
<a href="https://ibb.co/D1phS4c"><img src="https://i.ibb.co/dfgFzBC/4.png" alt="4" border="0"></a>

**로그 저장 화면** <br>
로그 보존 기간과 파일형식 그리고 로그 레벨과 프로세스를 선택해서 저장할 수 있고 저장 기간을 일 or 시간 으로 선택할 수 있다. <br>
저장된 로그를 열어서 확인할 수도 있다.
  

---
<a href="https://ibb.co/cX9FRZP"><img src="https://i.ibb.co/1JNqBP3/5.png" alt="5" border="0"></a>

**로그 자동삭제, 버전확인** <br>
일정 기간이 지나면 로그를 자동으로 삭제를 해준다. <br>
또한 등록된 프로그램의 버전도 확인할 수 있다.

---
<a href="https://ibb.co/41hvmHg"><img src="https://i.ibb.co/tLVjm9P/6.png" alt="6" border="0"></a>

**로그를 생성하는 프로그램 화면** <br>
좌측 샘플사진에 해당하는 것을 찾고 threshold -> pattern matching -> image centering 까지 한 모습이다. <br>
우측은 검사를 할 sample list가 있고, 하단에는 셈플의 위치와 유사도 등을 토대로 로그를 생성한다.
</details>

# 4. 📚 라이브러리 사용법

## 1. OpenCV
**1-1) Setting**
Project -> Properties -> C/C++ -> General -> Additional Include	Directories ->
 C:\opencv\build\include;

Project -> Properties -> Linker -> General -> Additional Library Directories ->
 C:\opencv\build\x64\vc15\lib;

Project -> Properties -> Linker -> Input -> Additional Dependencies ->
 opencv_world3416d.lib ; opencv_world3416.lib;

Project -> Properties -> Build Events -> Post-Build Event ->
 copy "C:\opencv\build\x64\vc15\bin\opencv_world3416d.dll"
 copy "C:\opencv\build\x64\vc15\bin\opencv_world3416.dll"

**1-2) Add Header**
 #include <opencv2/opencv.hpp>

 using namespace cv;



## 2. XLAutomation
**2-1) Add cpp, h file**
XLAutomation.cpp, XLAutomation.h, XLEzutomation.cpp, XLEzutomation.h

**2-2) Use of functions**
CXLEzAutomation xls(FALSE);

xls.OpenExcelFile(path) -> Open the excel file.
xls.SetCellValue(x, y, data); -> Add the data.
xls.SaveFileAspa(filename.GetPathName()); -> Save file.
xls.ReleaseExcel(); -> Release the excel file.

**2-3) Add header**
 #include “XLEzAutomation.h”


## 3. ChartDierector
**3-1) Add folder, cpp, h file to the project path**
ChartViewer.cpp, ChartViewer.h, include, lib64

**3-2) Add dll to the Debug folder**
chartdir70.dll -> in the lib64 folder

**3-3) setting**
Project -> Properties -> Linker -> Input -> Additional Dependencies ->
 lib\chartdir70.dll;

**3-4) Add cpp th the project**
ChartViewer.cppm ChartViewer.h, 
(bchartdir.h, chartdir.h, FinanceChart.h, memblock.h)-> in the include folder

**3-5) Add header**
 #include“ChartViewer.h”
