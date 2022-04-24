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
		TextClass* text = NULL;//отображаемый текст в €чейке
		Frame* childControl=NULL;//отображаемый контролл в €чейке, если надо
		ComplexPainter* painter = NULL;//собственный художник(не забыть удалить у всех €чеек при удалении таблицы)
	};
	enum class CellReaction {EditBox,Pushed,None};//реакци€ на нажатиек мыши по €чейке
}

class TableClass :public WindowClass {
protected:
	int scrollY = 0;//скролинг таблицы в ѕ–ќ÷≈Ќ“ј’ от высоты таблицы
	int scrollX = 0;
	TableClassNS::CellReaction reaction=TableClassNS::CellReaction::EditBox;
	EditClass* editBox;//поле дл€ реадктировани€ текстовых €чеек
	bool isEditActive = false;//был ли уже перенесен и сфокусирован edit

	struct TableRowColumn {
		Position pos;//смотрит только относительные или абсолютные величины (не забыть в WM_SIZE измен€ть относительные €чейки)
		int size;//ширина или высота в пользовательских единицах
		int realSize;//пересчитанна€ величина в пиксел€х
	};
	list <TableRowColumn> rowInfo;//информаци€ об высоте строк
	list <TableRowColumn> columnInfo;
	list<vector<TableClassNS::TableCell>> cellInfo;//€чейки в зависимости от строки (строки €чеек)

	//информаци€ дл€ быстрой отрисовки
	int lastColumn = -1;
	int lastRowInt = -1;
	list<vector<TableClassNS::TableCell>>::iterator lastRow = cellInfo.end();//будет играть в роли флага
	list<TableRowColumn>::iterator rowInfoLast;
	list<TableRowColumn>::iterator columnInfoLast;
	int cutDX = 0, cutDY = 0;//обрезанные куски текущей €чейки
	vector<Frame*> showedChildControls;//отображенные контролы

	//состо€ни€ мыши
	int mouseX=-1, mouseY=-1;
	bool isMoved = false;
	bool isDown = false;
	bool isUp = false;
	int choosedCellRow = -1;//выбрана ли €чейка
	int choosedCellCol = -1;//выбрана ли €чейка


	void InitStartTablePosition();//установка итераторов, €чейки левого верхнего угла(дорога€ функци€)
	void CountTableDimension();//рассчет реальных размеров €чеек
	void DrawTable(HDC hdc, RECT rect);//рисует текущую таблицу
	
	friend void TableClass_OnPaint(Frame* frame, WPARAM wParam, LPARAM lParam);
	friend void TableClass_OnMove(Frame* frame, WPARAM wParam, LPARAM lParam);
	friend void TableClass_OnLeave(Frame* frame, WPARAM wParam, LPARAM lParam);
	friend void TableClass_OnLDown(Frame* frame, WPARAM wParam, LPARAM lParam);
	friend void TableClass_OnLUp(Frame* frame, WPARAM wParam, LPARAM lParam); 
	friend void editClose(Frame* edit);
	
public:
	ComplexPainter* painter;//общий художник дл€ всех €чеек
	TableClass(Position pos, int x, int y, int w, int h, Frame* parent);
	~TableClass() {};

	void CreateWnd();//переопределенна€ функци€
	void AddRow(int index, Position pos, int h);
	void AddColumn(int index,Position pos,int w);
	void AddCellInfo(int row, int col, TableClassNS::TableCell info);
	void ScrollTable(ScrollBarType type,int pos);
	void SetTableReaction(TableClassNS::CellReaction reaction);
};