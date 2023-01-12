#ifndef DIFFUI_H
#define DIFFUI_H

#include <QtWidgets/QMainWindow>
#include "ui_diffui.h"
#include "DiffTextEdit.h"
#include "HotPointBar.h"
#include "Common.h"

enum TextFormatType
{
	TextColor_Normal,
	TextColor_Modify,
	TextColor_Pad,
};

enum TexTSide
{
	TextSide_Left,
	TextSide_Right,
	TextSide_Both,
};

enum EncodeType
{
	EncodeType_UTF8,
	EncodeType_ANSI,
};

void UseBeyondCompare( QString fileL, QString fileR );



class XmlDiff;
class StringBuff;

class DiffUI : public QMainWindow
{
	Q_OBJECT

public:
	DiffUI(QWidget *parent = 0);
	~DiffUI();

    void resizeEvent(QResizeEvent *event);


	void BeginEditBlock();
	void EndEditBlock();
	void AppendText(const char* text,TexTSide side, TextFormatType type);
	void AppendText(StringBuff& buff,TexTSide side, TextFormatType type);
	int GetTotalBlocks();

	void AppendLineCompareText(std::wstring str, TexTSide side, DiffType type);
	void ResetLineCompareText();

	void ModifyMarkBegin();
	void ModifyMarkEnd();
	void ClearModifyMark();

	void AppendNewLine();
	void HighLightDiffBlocks(int startblockNum);


	bool Diff( std::string file1, std::string file2);
	void ClearText();
	void UseBeyondCompare();
	void UpdateHotSegments();
	void MoveToBlock(int block);

	int PageLineCount();
	int VisualContentHeight();

	QString GetModifyText(int startBlock, bool isLeft);

	std::wstring ToWstring(const std::string& str);
private slots: 
	void onLScrollContentChanged();
	void onRScrollContentChanged();
	void onValueChanged(int);
	void onLTextUpdateRequest(const QRect & rect, int dy);
	void onRTextUpdateRequest(const QRect & rect, int dy);
	void nextDiffLine();
	void prevDiffLine();
	void switchApp();
	void exitApp();
	void switchUTF8();
	void switchANSI();
	void hideUnchanged();
	void showSelectBlock(int block);
	//void verticalScrollbarValueChanged(int value);
	void updateHotPointBar();

	
private:
	Ui::DiffUIClass ui;
    DiffTextEdit *textEditL;
    DiffTextEdit *textEditR;

	QPlainTextEdit* textEditLineCompareUp;
	QPlainTextEdit* textEditLineCompareDown;

	QTextCursor* cursorTextEditLineCompareUp;
	QTextCursor* cursorTextEditLineCompareDown;


	QAction * nextModifyAction;
	QAction * prevModifyAction;
	QWidget* drawLineCanvas;

	int curHighLightBeginBlockNum;

	HotPointBar* hotPointBar;
private:
	QTextCursor* cursorL;
	QTextCursor* cursorR;
	std::set<int> ModifyBegTags;
	std::set<int> ModifyEndTags;
	XmlDiff* diffInst;
	QString file1,file2;
	EncodeType encodeType;
};

#endif // DIFFUI_H
