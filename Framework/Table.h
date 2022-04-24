#pragma once
#include "CWindow.h"
#include "TextDrawing.h"
#include "Painter.h"
#include "ScrollBar.h"
#include <list>
#include "Edit.h"

using namespace std;

namespace TableClassNS {
	extern wstring ClassName;
	void InitWindowClass();

	struct TableCell {
		TextClass* text = NULL;//������������ ����� � ������
		Frame* childControl=NULL;//������������ �������� � ������, ���� ����
		ComplexPainter* painter = NULL;//����������� ��������(�� ������ ������� � ���� ����� ��� �������� �������)
	};
	enum class CellReaction {EditBox,Pushed,None};//������� �� �������� ���� �� ������
}

class TableClass :public WindowClass {
protected:
	int scrollY = 0;//�������� ������� � ��������� �� ������ �������
	int scrollX = 0;
	TableClassNS::CellReaction reaction=TableClassNS::CellReaction::EditBox;
	EditClass* editBox;//���� ��� �������������� ��������� �����
	bool isEditActive = false;//��� �� ��� ��������� � ������������ edit

	struct TableRowColumn {
		Position pos;//������� ������ ������������� ��� ���������� �������� (�� ������ � WM_SIZE �������� ������������� ������)
		int size;//������ ��� ������ � ���������������� ��������
		int realSize;//������������� �������� � ��������
	};
	list <TableRowColumn> rowInfo;//���������� �� ������ �����
	list <TableRowColumn> columnInfo;
	list<vector<TableClassNS::TableCell>> cellInfo;//������ � ����������� �� ������ (������ �����)

	//���������� ��� ������� ���������
	int lastColumn = -1;
	int lastRowInt = -1;
	list<vector<TableClassNS::TableCell>>::iterator lastRow = cellInfo.end();//����� ������ � ���� �����
	list<TableRowColumn>::iterator rowInfoLast;
	list<TableRowColumn>::iterator columnInfoLast;
	int cutDX = 0, cutDY = 0;//���������� ����� ������� ������
	vector<Frame*> showedChildControls;//������������ ��������

	//��������� ����
	int mouseX=-1, mouseY=-1;
	bool isMoved = false;
	bool isDown = false;
	bool isUp = false;
	int choosedCellRow = -1;//������� �� ������
	int choosedCellCol = -1;//������� �� ������


	void InitStartTablePosition();//��������� ����������, ������ ������ �������� ����(������� �������)
	void CountTableDimension();//������� �������� �������� �����
	void DrawTable(HDC hdc, RECT rect);//������ ������� �������
	
	friend void TableClass_OnPaint(Frame* frame, WPARAM wParam, LPARAM lParam);
	friend void TableClass_OnMove(Frame* frame, WPARAM wParam, LPARAM lParam);
	friend void TableClass_OnLeave(Frame* frame, WPARAM wParam, LPARAM lParam);
	friend void TableClass_OnLDown(Frame* frame, WPARAM wParam, LPARAM lParam);
	friend void TableClass_OnLUp(Frame* frame, WPARAM wParam, LPARAM lParam); 
	friend void editClose(Frame* edit);
	
public:
	ComplexPainter* painter;//����� �������� ��� ���� �����
	TableClass(Position pos, int x, int y, int w, int h, Frame* parent);
	~TableClass() {};

	void CreateWnd();//���������������� �������
	void AddRow(int index, Position pos, int h);
	void AddColumn(int index,Position pos,int w);
	void AddCellInfo(int row, int col, TableClassNS::TableCell info);
	void ScrollTable(ScrollBarType type,int pos);
	void SetTableReaction(TableClassNS::CellReaction reaction);
};