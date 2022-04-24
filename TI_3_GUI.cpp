#include "framework.h"
#include "Framework/WindowFramework.h"
#include <commdlg.h>
#include <string>
#include <fstream>
#include <cstdlib>

using namespace std;

#pragma comment(lib,"Framework/StaticLib1.lib")
//#pragma comment(lib,"Comdlg32.lib")

void InitWindow();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lCmdLine, int nCmdShow) {
	InitWindowFramework();
	AddFontFamily(L"BalsamiqSans-Regular.ttf");
	InitWindow();
	StartWindowLoop();
	FreeFontResourses();
	FinallizeFramework();
	return 0;
}

const int MAX_EXAMPLE_BYTES = 200;//число байтов для примера

vector<int64_t> inputFlow;
vector<int64_t> outputFlow;
vector<int64_t> keyFlow;

///АЛГОРИТМЫ

struct EuclidResult {
	int64_t x;
	int64_t y;
	int64_t NOD;
};

void EuclidEx(int64_t a, int64_t b, EuclidResult& res) {//расширенный алгоритм Евклида
	int64_t d0 = a;
	int64_t d1 = b;//d1<d0 - для инверсно мультипликативных
	int64_t x0 = 1;
	int64_t x1 = 0;//
	int64_t y0 = 0;
	int64_t y1 = 1;
	while (d1 > 1) {
		int64_t q = d0 / d1;//сколько целых частей d1 в d0
		int64_t d2 = d0 % d1;
		int64_t x2 = x0 - q * x1;
		int64_t y2 = y0 - q * y1;
		d0 = d1; d1 = d2;
		x0 = x1;  x1 = x2;
		y0 = y1; y1 = y2;
	}
	res.x = x1;
	res.y = y1;
	res.NOD = (d1 == 1) ? 1 : d0;
}

int FastPower(int a, int power, int module) {//алгоритм быстрого возведения в степень
	int64_t a1 = a; int64_t power1 = power;
	int64_t result = 1;
	while (power1 != 0) {
		while (power1 % 2 == 0) {
			power1 /= 2;
			a1 = a1 * a1 % module;
		}
		power1--;
		result = result * a1 % module;
	}
	return result;
}

int Decoder(int64_t p, int64_t q, int64_t b, int64_t c) {
	int64_t n = p * q;
	int64_t D = (b * b + 4 * c) % n;
	int64_t Mp = FastPower(D, (p + 1) / 4, p);
	int64_t Mq = FastPower(D, (q + 1) / 4, q);
	EuclidResult euclid;
	EuclidEx(p, q, euclid);
	int64_t f1 = q * euclid.y;
	int64_t f2 = p * euclid.x;
	int64_t root[4];
	root[0] = (Mp * f1 + Mq * f2) % n;
	root[1] = ((n - Mp) * f1 + Mq * f2) % n;
	root[2] = (Mp * f1 + (n - Mq) * f2) % n;
	root[3] = ((n - Mp) * f1 + (n - Mq) * f2) % n;

	for (int i = 0; i < 4; i++) {
		int64_t numerator = -b + root[i];
		if (numerator % 2 != 0) numerator += n;
		if ((numerator) >= 0) {//математический модуль
			root[i] = (numerator / 2) % n;
		}
		else {
			root[i] = (numerator / 2 + n) % n;
		}
		if (root[i] <= 255) return root[i];
	}
	return 0;
}

int DecoderForExample(int64_t p, int64_t q, int64_t b, int64_t c) {
	int64_t n = p * q;
	int64_t D = (b * b + 4 * c) % n;
	int64_t Mp = FastPower(D, (p + 1) / 4, p);
	int64_t Mq = FastPower(D, (q + 1) / 4, q);
	EuclidResult euclid;
	EuclidEx(p, q, euclid);
	int64_t f1 = q * euclid.y;
	int64_t f2 = p * euclid.x;
	int64_t root[4];
	root[0] = (Mp * f1 + Mq * f2) % n;
	root[1] = ((n - Mp) * f1 + Mq * f2) % n;
	root[2] = (Mp * f1 + (n - Mq) * f2) % n;
	root[3] = ((n - Mp) * f1 + (n - Mq) * f2) % n;

	for (int i = 0; i < 4; i++) {
		int64_t numerator = -b + root[i];
		if (numerator % 2 != 0) numerator += n;
		if ((numerator) >= 0) {//математический модуль
			root[i] = (numerator / 2) % n;
		}
		else {
			root[i] = (numerator / 2 + n) % n;
		}
		keyFlow.push_back(root[i]);
	}
	return 0;
}

int32_t GetBytesPerCode(int n) {
	int resBit = 0;
	do {
		n >>= 1;
		resBit++;
	} while (n != 0);
	int Bytes = resBit / CHAR_BIT;
	if (resBit % CHAR_BIT != 0) Bytes++;
	return Bytes;
}

#define MAX_BUF_SIZE 10000

void CodeFile(wstring inFilePath, wstring inFileName, int p, int q, int b, wstring outName) {//файлы должны быть открыты заранее
	ifstream inFile; inFile.open(inFilePath, ios::in | ios::binary);
	int n = p * q;
	int32_t bytesPerCode = GetBytesPerCode(n);//максимальное кол-во байтов для кодирования информации

	ofstream outFile; outFile.open(outName, ios::out | ios::binary | ios::trunc);
	//запишем имя изначального файла в заголовок
	int32_t nameSize = inFileName.size() * sizeof(wchar_t);
	outFile.write((char*)&nameSize, sizeof(nameSize));//записать длину имени файла
	outFile.write((char*)&inFileName[0], nameSize);//имя файла в мультибайтах
	outFile.write((char*)&bytesPerCode, sizeof(bytesPerCode));//записать размер шифросимволов

	bool isWriteExample = true;
	static unsigned char buffer[MAX_BUF_SIZE];//буфер для потока
	unsigned char* outBuffer = new unsigned char[MAX_BUF_SIZE * bytesPerCode];
	int writeBytesToEx;
	while (inFile) {
		inFile.read((char*)buffer, MAX_BUF_SIZE);
		streamsize readBytes = inFile.gcount();
		//запишем поток для примера
		if (isWriteExample) {
			writeBytesToEx = MAX_EXAMPLE_BYTES <= readBytes ? MAX_EXAMPLE_BYTES : readBytes;
			for (int i = 0; i < writeBytesToEx; i++) inputFlow.push_back(buffer[i]);
			//isWriteExample = false;
		}
		//кодирование потока
		for (int i = 0; i < readBytes; i++) {
			int64_t m = buffer[i];
			int64_t code = (m * (m + b)) % n;//кодирование символа
			//запись для примера
			if (isWriteExample && writeBytesToEx) {
				outputFlow.push_back(code);
			}
			//ЗАПИСЬ КОДА,НАЧИНАЯ С МЛАДШИХ БАЙТОВ
			for (int j = 0; j < bytesPerCode; j++) {
				char next = (char)code;
				outBuffer[i * bytesPerCode + j] = next;
				code >>= CHAR_BIT;
			}
			
		}
		if (isWriteExample) isWriteExample = false;//только один буфер записываем
		outFile.write((char*)outBuffer, readBytes * bytesPerCode);
		if (!outFile) break;
	}
	delete[] outBuffer;
	inFile.close();
	outFile.close();
}

wstring DecodeFile(wstring inFileName, int p, int q, int b) {
	ifstream inFile; inFile.open(inFileName, ios::in | ios::binary);
	int n = p * q;
	//считывание имени шифрованного файла
	int32_t nameSize;
	inFile.read((char*)&nameSize, sizeof(nameSize));//
	wstring fileName_w;
	for (int i = 0; i < nameSize / sizeof(wchar_t); i++) {
		wchar_t ch; inFile.read((char*)&ch, sizeof(wchar_t));
		fileName_w.push_back(ch);
	}
	ofstream outFile; outFile.open(fileName_w, ios::out | ios::binary | ios::trunc);
	//считывание размера шифросимволов
	int32_t bytesPerCode;
	inFile.read((char*)&bytesPerCode, sizeof(bytesPerCode));//записать размер шифросимволов

	bool isWriteExample = true; int writeBytesToEx;
	static unsigned char outBuffer[MAX_BUF_SIZE];//
	int inpBufSize = MAX_BUF_SIZE * bytesPerCode;
	unsigned char* buffer = new unsigned char[inpBufSize];
	while (inFile) {
		inFile.read((char*)buffer, inpBufSize);
		streamsize readBytes = inFile.gcount();
		int outIndex = 0;
		int i = 0;
		if (isWriteExample) {
			writeBytesToEx = MAX_EXAMPLE_BYTES <= readBytes/bytesPerCode ? MAX_EXAMPLE_BYTES : readBytes / bytesPerCode;
		}
		while (i < readBytes) {
			int64_t code = 0;
			for (int j = 0; j < bytesPerCode; j++) {
				code |= ((int64_t)buffer[i] << (j * CHAR_BIT));
				i++;
			}
			int64_t decoded = Decoder(p, q, b, code);
			outBuffer[outIndex++] = (char)decoded;
			if (isWriteExample && writeBytesToEx) {
				inputFlow.push_back(code);
				outputFlow.push_back(decoded);
				writeBytesToEx--;
			}
		}
		outFile.write((char*)outBuffer, outIndex);
		if (!outFile) break;
		if(isWriteExample) isWriteExample = false;
	}
	//delete[] fileName;
	delete[] buffer;
	inFile.close();
	outFile.close();
	return fileName_w;
}

bool CheckSimple(int64_t number) {
	if (number % 2 != 0) {
		int s = 0;//количесвто степеней двойки
		int64_t N = number - 1;
		while (N % 2 == 0) {
			s++;
			N /= 2;
		}
		int64_t a = rand();
		int64_t res = FastPower(a, N, number);
		if (res == 1) {
			return true;
		}
		int64_t r = rand() % s;
		res = FastPower(a, FastPower(2, r, number) * N, number);
		if (res == number - 1) {
			return true;
		}
	}
	return false;
}

//Параметры алгоритма
wstring inputFilePath;//полный путь
wstring inputFileName;//только имя

int64_t P;
int64_t Q;
int64_t N;
int64_t B;

//GDI

const wstring fontFamily = L"Balsamiq Sans";

const int wMain = 1000;
const int hMain = 730;
WindowClass* mainWindow;
LabelClass* lblInputFile;
LabelClass* lblOutputFile;
EditClass* editP;
EditClass* editQ;
EditClass* editN;
EditClass* editB;
EditClass* editSource;
EditClass* editOutput;
EditClass* editRoot;

//цвета
const COLORREF mainColor = RGB(203, 242, 247);
const COLORREF captionColor = RGB(144, 233, 245);
const COLORREF editColor = RGB(121, 167, 237);
const COLORREF editColorFocus = RGB(141, 181, 242);
const COLORREF closeStaticColor = RGB(207, 27, 48);
const COLORREF closeMoveColor = RGB(235, 47, 69);
const COLORREF closePushColor = RGB(168, 13, 32);

const COLORREF colorBtnStatic = RGB(250, 242, 25);
const COLORREF colorBtnMove = RGB(227, 220, 16);
const COLORREF colorBtnDown = RGB(191, 186, 25);
const COLORREF colorBtnBorder1 = RGB(219, 117, 44);
const COLORREF colorBtnBorder2 = RGB(179, 83, 14);

void SetBasicFont(Frame* ctl) {
	ctl->font->SetFontFamily(fontFamily);
	ctl->font->SetFontSize(16);
}

void EditFocus(Frame* edit) {
	edit->SetBackgroundColor(editColorFocus);
}

void EditKillFocus(Frame* edit) {
	edit->SetBackgroundColor(editColor);
}

void SetEditCustom(Frame* edit) {
	edit->SetBackgroundColor(editColor);
	edit->SetBorderRadius(20);
	edit->eventHandler->SetFocusEvent(FocusEvents::OnFocus, EditFocus);
	edit->eventHandler->SetFocusEvent(FocusEvents::OnKillFocus, EditKillFocus);
	SetBasicFont(edit);
}

//стилизированные обработчкики
void SetMouseEventsBtn(Frame* btn,BasicEvent move, BasicEvent push, BasicEvent leave, BasicEvent click=nullptr) {
	btn->eventHandler->SetMouseEvent(MouseEvents::OnMove, move);
	btn->eventHandler->SetMouseEvent(MouseEvents::OnLBDown, push);
	btn->eventHandler->SetMouseEvent(MouseEvents::OnLeave, leave);
	btn->eventHandler->SetMouseEvent(MouseEvents::OnClick, click);
}

void btnCloseMove(Frame* btn) {
	btn->SetBackgroundColor(closeMoveColor);
}
void btnClosePush(Frame* btn) {
	btn->SetBackgroundColor(closePushColor);
}
void btnCloseLeave(Frame* btn) {
	btn->SetBackgroundColor(closeStaticColor);
}

void btnMove(Frame* btn) {
	btn->font->SetTextColor(RGB(209, 67, 6));
	btn->SetBackgroundColor(colorBtnMove);
}
void btnPush(Frame* btn) {
	btn->SetBackgroundColor(colorBtnDown);
}
void btnLeave(Frame* btn) {
	btn->font->SetTextColor(RGB(0, 0, 0));
	btn->SetBackgroundColor(colorBtnStatic);
}

void SetBasicBtnCustom(Frame* btn) {
	btn->SetBackgroundColor(colorBtnStatic);
	Border br;
	br.color = colorBtnBorder1;
	br.style = BorderStyle::solid;
	br.width = 4;
	btn->SetBorder(br, BorderNum::top);
	btn->SetBorder(br, BorderNum::left);
	br.width = 6;
	br.color = colorBtnBorder2;
	btn->SetBorder(br, BorderNum::right);
	btn->SetBorder(br, BorderNum::btm);
	btn->SetBorderRadius(6);

	SetBasicFont(btn);
	SetMouseEventsBtn(btn, btnMove, btnPush, btnLeave, btnMove);
}

//обраотчкики
void btnCloseInstance(Frame* btn) {
	StopWindowLoop();
}

void OpenFileEvent(Frame* btn) {
	btnMove(btn);
	static OPENFILENAME ofl;
	const int MAX_LEN = 256;
	WCHAR fileName[MAX_LEN] = { 0 };
	ofl.lStructSize = sizeof(OPENFILENAME);
	ofl.hInstance = GetModuleHandle(NULL);
	ofl.hwndOwner = mainWindow->hWnd;
	ofl.lpstrFile = fileName;
	ofl.nMaxFile = MAX_LEN;
	if (GetOpenFileName(&ofl)) {
		inputFilePath = fileName;
		inputFileName = &fileName[ofl.nFileOffset];
		wstring caption = L"Исходный файл:  " + inputFileName;
		lblInputFile->SetCaption(caption);
	}
}

bool GetNumberFromString(const wstring& number,int64_t& result) {
	if (number.size()) {
		for (auto ch : number) {
			if (ch < L'0' || ch>L'9') return false;
		}
		result = stoi(number);
		if (result < 0) return false;
		return true;
	}
	else {
		return false;
	}
}

bool CheckNumberPQ(int64_t& number, EditClass* edit) {
	wstring text;
	edit->GetText(text);
	if (text.size() > 6 || !GetNumberFromString(text, number) ) {
		MessageBox(mainWindow->hWnd, L"Некорректный ввод числа", L"Ooops", MB_OK | MB_ICONWARNING);
		edit->SetBackgroundColor(RGB(255, 0, 0));
		return false;
	}
	if (!CheckSimple(number)) {
		MessageBox(mainWindow->hWnd, L"Число не простое (либо вероятностный алгоритм ошибся)", L"Ooops", MB_OK | MB_ICONWARNING);
		edit->SetBackgroundColor(RGB(255, 0, 0));
		return false;
	}
	if (number % 4 != 3) {
		MessageBox(mainWindow->hWnd, L"Число при делении на 4 должно давать остаток 3", L"Ooops", MB_OK | MB_ICONWARNING);
		edit->SetBackgroundColor(RGB(255, 0, 0));
		return false;
	}
	return true;
}


bool isThereProblem(int64_t p,int64_t q,int64_t b) {
	if (p < 3) return true;
	if (q < 3511) return true;
	if (b > 10533 || b == 0) return true;
	return false;
}

extern int exampleOffset;
void btnNextStepExample(Frame* btn);

void btnStartCoder(Frame* btn) {
	if (!inputFilePath.size()) {
		MessageBox(mainWindow->hWnd, L"Входной файл не выбран", L"Ooops", MB_OK | MB_ICONWARNING);
		return;
	}
	if (!CheckNumberPQ(P, editP)) {
		return;
	}
	if (!CheckNumberPQ(Q, editQ)) {
		return;
	}
	N = P * Q;
	editN->SetText(to_wstring(N));
	wstring text;
	editB->GetText(text);
	if (text.size()>6 || !GetNumberFromString(text, B)) {
		MessageBox(mainWindow->hWnd, L"Некорректный ввод числа", L"Ooops", MB_OK | MB_ICONWARNING);
		editB->SetBackgroundColor(RGB(255, 0, 0));
		return;
	}
	if (B >= N) {
		MessageBox(mainWindow->hWnd, L"Число B больше, чем N", L"Ooops", MB_OK | MB_ICONWARNING);
		editB->SetBackgroundColor(RGB(255, 0, 0));
		return;
	}
	int ans = IDOK;
	if (isThereProblem(P, Q, B)) {
		ans = MessageBox(mainWindow->hWnd, L"При последующей дешифрации возможна неоднозначность! Хотите продолжить?", L"Wsrning", MB_OKCANCEL | MB_ICONWARNING);
	}
	if (ans == IDOK) {
		wstring outFileName = L"Output.txt";
		lblOutputFile->SetCaption(L"Выходной файл: " + outFileName);
		exampleOffset = -1;
		inputFlow.clear();
		outputFlow.clear();
		keyFlow.clear();
		CodeFile(inputFilePath, inputFileName, P, Q, B, outFileName);
		editSource->SetText(L"");
		editOutput->SetText(L"");
		editRoot->SetText(L"");
		btnNextStepExample(nullptr);
	}
}

void btnStartDecode(Frame* btn) {
	if (!inputFilePath.size()) {
		MessageBox(mainWindow->hWnd, L"Входной файл не выбран", L"Ooops", MB_OK | MB_ICONWARNING);
		return;
	}
	if (!CheckNumberPQ(P, editP)) {
		return;
	}
	if (!CheckNumberPQ(Q, editQ)) {
		return;
	}
	N = P * Q;
	editN->SetText(to_wstring(N));
	wstring text;
	editB->GetText(text);
	if (text.size() > 6 || !GetNumberFromString(text, B)) {
		MessageBox(mainWindow->hWnd, L"Некорректный ввод числа", L"Ooops", MB_OK | MB_ICONWARNING);
		editB->SetBackgroundColor(RGB(255, 0, 0));
		return;
	}
	if (B >= N) {
		MessageBox(mainWindow->hWnd, L"Число B больше, чем N", L"Ooops", MB_OK | MB_ICONWARNING);
		editB->SetBackgroundColor(RGB(255, 0, 0));
		return;
	}
	int ans=IDOK;
	if (isThereProblem(P, Q, B)) {
		ans = MessageBox(mainWindow->hWnd, L"При дешифрации возможна неоднозначность! Хотите продолжить?", L"Wsrning", MB_OKCANCEL | MB_ICONWARNING);
	}
	if (ans == IDOK) {
		exampleOffset = -1;
		inputFlow.clear();
		outputFlow.clear();
		keyFlow.clear();
		wstring outFileName = DecodeFile(inputFilePath, P, Q, B);
		lblOutputFile->SetCaption(L"Выходной файл: " + outFileName);
		editSource->SetText(L"");
		editOutput->SetText(L"");
		editRoot->SetText(L"");
		//сгенерируем ключи для вывода
		for (int i = 0; i < inputFlow.size(); i++) {
			DecoderForExample(P, Q, B, inputFlow[i]);
		}
		btnNextStepExample(nullptr);
	}
}

int exampleOffset = -1;
const int maxCharsToShow = 20;

void btnNextStepExample(Frame* btn) {
	if (exampleOffset +1 < inputFlow.size()) {
		exampleOffset++;
		wstring textIn = L"";
		wstring textOut=L"";
		int charsToShow = (exampleOffset + maxCharsToShow) < inputFlow.size() ? maxCharsToShow : inputFlow.size() - exampleOffset;
		for (int i = 0; i < charsToShow; i++) {
			textIn += to_wstring(inputFlow[exampleOffset + i]) + L" ";
			textOut += to_wstring(outputFlow[exampleOffset + i])+L" ";
		}
		if (keyFlow.size()) {
			wstring textKey=L"";
			for (int i = 0; i < 4; i++) {//вывод 4 корней для текущего шага
				textKey += to_wstring(keyFlow[exampleOffset * 4 + i]) + L" ";
			}
			editRoot->SetText(textKey);
		}
		editSource->SetText(textIn);
		editOutput->SetText(textOut);
	}
}

//инициализация окна 
void InitWindow() {
	mainWindow = new WindowClass(L"", Window::WindowType::PanelWindow, Position::absoluteAll, 150, 80, wMain, hMain, NULL);
	mainWindow->SetBackgroundColor(mainColor);
	mainWindow->SetBorderRadius(50);

	//ЗАГОЛОВОК
	CaptionClass* caption = new CaptionClass(50, mainWindow);
	caption->SetBackgroundColor(captionColor);
	caption->caption->SetText(L"Криптографические системы с открытым ключом");
	caption->caption->SetTextOffset(15, 0);
	SetBasicFont(caption);
	caption->font->SetFontSize(18);
	Button* closeInstance = new Button(Position::absoluteAll, 950, 10, 30, 30, caption);
	closeInstance->SetBackgroundColor(closeStaticColor);
	closeInstance->caption->SetText(L"");
	closeInstance->SetBorderRadius(30);
	SetMouseEventsBtn(closeInstance, btnCloseMove, btnClosePush, btnCloseLeave);
	closeInstance->eventHandler->SetMouseEvent(MouseEvents::OnClick, btnCloseInstance);
	caption->AddChild(closeInstance);
	mainWindow->AddChild(caption);

	//ПАНЕЛЬ УПРАВЛЕНИЯ
	WindowClass* controlPanel = new WindowClass(L"", Window::WindowType::PanelWindow, Position::absolutePosH, 0, 50, 100, 90, mainWindow);
	controlPanel->SetBackgroundColor(mainColor);
	Button* btnOpenFile = new Button(Position::absoluteAll, 30, 20, 200, 60, controlPanel);
	btnOpenFile->caption->SetText(L"Открыть файл");
	SetBasicBtnCustom(btnOpenFile);
	btnOpenFile->eventHandler->SetMouseEvent(MouseEvents::OnClick, OpenFileEvent);
	controlPanel->AddChild(btnOpenFile);
	Button* btnCode = new Button(Position::absoluteAll, 250, 20, 280, 60, controlPanel);
	btnCode->caption->SetText(L"Зашифровать файл");
	SetBasicBtnCustom(btnCode);
	btnCode->eventHandler->SetMouseEvent(MouseEvents::OnClick, btnStartCoder);
	controlPanel->AddChild(btnCode);
	Button* btnDecode = new Button(Position::absoluteAll, 550, 20, 280, 60, controlPanel);
	btnDecode->caption->SetText(L"Расшифровать файл");
	SetBasicBtnCustom(btnDecode);
	btnDecode->eventHandler->SetMouseEvent(MouseEvents::OnClick, btnStartDecode);
	controlPanel->AddChild(btnDecode);
	mainWindow->AddChild(controlPanel);

	//ГЛАВНАЯ ПАНЕЛЬ
	WindowClass* mainPanel = new WindowClass(L"", Window::WindowType::PanelWindow, Position::absolutePosSpaceDown, 0, 140, 100, 1,mainWindow);
	mainPanel->SetBackgroundColor(mainColor);
	//
	lblInputFile = new LabelClass(L"Исходный файл: ", Position::absolutePosH, 20, 10, 100, 40, mainPanel);
	lblInputFile->SetBackgroundColor(mainColor);
	SetBasicFont(lblInputFile);
	mainPanel->AddChild(lblInputFile);
	//
	lblOutputFile = new LabelClass(L"Выходной файл: ", Position::absolutePosH, 20, 50, 100, 40, mainPanel);
	lblOutputFile->SetBackgroundColor(mainColor);
	SetBasicFont(lblOutputFile);
	mainPanel->AddChild(lblOutputFile);
	//
	LabelClass* lblInfo = new LabelClass(L"Параметры шифрования для метода Рабина", Position::absolutePosH, 0, 100, 100, 40, mainPanel);
	lblInfo->SetTextAlign(TextAlign::Center);
	lblInfo->SetBackgroundColor(mainColor);
	SetBasicFont(lblInfo);
	mainPanel->AddChild(lblInfo);
	//
	LabelClass* lblP = new LabelClass(L"Число P:", Position::absoluteAll, 20, 150, 110, 50, mainPanel);
	lblP->SetBackgroundColor(mainColor);
	SetBasicFont(lblP);
	mainPanel->AddChild(lblP);
	//
	editP = new EditClass(L"", Position::absoluteAll, 140, 150, 300, 50, mainPanel);
	SetEditCustom(editP);
	mainPanel->AddChild(editP);
	//
	LabelClass* lblQ = new LabelClass(L"Число Q:", Position::absoluteAll, 460, 150, 110, 50, mainPanel);
	lblQ->SetBackgroundColor(mainColor);
	SetBasicFont(lblQ);
	mainPanel->AddChild(lblQ);
	//
	editQ = new EditClass(L"", Position::absoluteAll, 580, 150, 300, 50, mainPanel);
	SetEditCustom(editQ);
	mainPanel->AddChild(editQ);
	//
	LabelClass* lblB = new LabelClass(L"Число B:", Position::absoluteAll, 20, 210, 110, 50, mainPanel);
	lblB->SetBackgroundColor(mainColor);
	SetBasicFont(lblB);
	mainPanel->AddChild(lblB);
	//
	editB = new EditClass(L"", Position::absoluteAll, 140, 210, 300, 50, mainPanel);
	SetEditCustom(editB);
	mainPanel->AddChild(editB);
	//
	LabelClass* lblN = new LabelClass(L"Число N:", Position::absoluteAll, 460, 210, 110, 50, mainPanel);
	lblN->SetBackgroundColor(mainColor);
	SetBasicFont(lblN);
	mainPanel->AddChild(lblN);
	//
	editN = new EditClass(L"", Position::absoluteAll, 580, 210, 300, 50, mainPanel);
	SetEditCustom(editN);
	editN->SetEnable(false); editN->eventHandler->SetFocusEvent(FocusEvents::OnFocus, nullptr);
	mainPanel->AddChild(editN);
	//
	LabelClass* lblInfo2 = new LabelClass(L"Анализ данных", Position::absolutePosH, 0, 270, 100, 40, mainPanel);
	lblInfo2->SetTextAlign(TextAlign::Center);
	lblInfo2->SetBackgroundColor(mainColor);
	SetBasicFont(lblInfo2);
	mainPanel->AddChild(lblInfo2);
	//
	LabelClass* lblSource = new LabelClass(L"Исходный поток:", Position::absoluteAll, 20, 320, 210, 50, mainPanel);
	lblSource->SetBackgroundColor(mainColor);
	SetBasicFont(lblSource);
	mainPanel->AddChild(lblSource);
	//
	editSource = new EditClass(L"", Position::absoluteAll, 240, 320, 600, 50, mainPanel);
	SetEditCustom(editSource);
	editSource->SetEnable(false); editSource->eventHandler->SetFocusEvent(FocusEvents::OnFocus, nullptr);
	mainPanel->AddChild(editSource);
	//
	LabelClass* lblOutput = new LabelClass(L"Полученный поток:", Position::absoluteAll, 20, 380, 210, 50, mainPanel);
	lblOutput->SetBackgroundColor(mainColor);
	SetBasicFont(lblOutput);
	lblOutput->font->SetFontSize(14);
	mainPanel->AddChild(lblOutput);
	//
	editOutput = new EditClass(L"", Position::absoluteAll, 240, 380, 600, 50, mainPanel);
	SetEditCustom(editOutput);
	editOutput->SetEnable(false); editOutput->eventHandler->SetFocusEvent(FocusEvents::OnFocus, nullptr);
	mainPanel->AddChild(editOutput);
	//
	LabelClass* lblRoot = new LabelClass(L"Корни квадратичного сравнения:", Position::absoluteAll, 20, 440, 440, 50, mainPanel);
	lblRoot->SetBackgroundColor(mainColor);
	SetBasicFont(lblRoot);
	mainPanel->AddChild(lblRoot);
	//
	editRoot = new EditClass(L"", Position::absoluteAll, 460, 440, 400, 50, mainPanel);
	SetEditCustom(editRoot);
	editRoot->SetEnable(false); editRoot->eventHandler->SetFocusEvent(FocusEvents::OnFocus, nullptr);
	mainPanel->AddChild(editRoot);
	//
	Button* btnNextStep = new Button(Position::absoluteAll, (wMain-300)/2, 500, 300, 60, mainPanel);
	btnNextStep->caption->SetText(L"Следующий шаг");
	SetBasicBtnCustom(btnNextStep);
	btnNextStep->eventHandler->SetMouseEvent(MouseEvents::OnClick, btnNextStepExample);
	mainPanel->AddChild(btnNextStep);
	mainWindow->AddChild(mainPanel);

	mainWindow->CreateWnd();
}