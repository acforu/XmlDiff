#ifndef DIFFUI_H
#define DIFFUI_H

#include <QtGui/QMainWindow>
#include "ui_diffui.h"
#include "DiffTextEdit.h"

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
};

class XmlDiff;

class DiffUI : public QMainWindow
{
	Q_OBJECT

public:
	DiffUI(QWidget *parent = 0, Qt::WFlags flags = 0);
	~DiffUI();

    void resizeEvent(QResizeEvent *event);


	void BeginEditBlock();
	void EndEditBlock();
	void AppendText(const char* text,TexTSide side, TextFormatType type);
	int GetTotalBlocks();
	void MoveToBlock(int block);
	void ModifyMarkBegin();
	void ModifyMarkEnd();
	void AddNewLine();
	void HighLightDiffBlocks(int startblockNum);


	bool Diff( std::string file1, std::string file2);

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

private:
	Ui::DiffUIClass ui;
    DiffTextEdit *textEditL;
    DiffTextEdit *textEditR;
    //QPlainTextEdit  *textEditPlain;

	QAction * nextModifyAction;
	QAction * prevModifyAction;
	QWidget* drawLineCanvas;

	int curHighLightBeginBlockNum;
private:
	QTextCursor* cursorL;
	QTextCursor* cursorR;
	std::set<int> ModifyBegTags;
	std::set<int> ModifyEndTags;
	XmlDiff* diffInst;
	QString file1,file2;
};

#endif // DIFFUI_H
