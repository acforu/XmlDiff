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
	void MarkModifyTag();

private slots: 
	void onLScrollContentChanged();
	void onRScrollContentChanged();
	void onValueChanged(int);
	void onLTextUpdateRequest(const QRect & rect, int dy);
	void onRTextUpdateRequest(const QRect & rect, int dy);
	void nextDiffLine();
	void prevDiffLine();

private:
	Ui::DiffUIClass ui;
    DiffTextEdit *textEditL;
    DiffTextEdit *textEditR;
    //QPlainTextEdit  *textEditPlain;

	QAction * nextModifyAction;
	QAction * prevModifyAction;
private:
	QTextCursor* cursorL;
	QTextCursor* cursorR;
	std::set<int> ModifyTags;

};

#endif // DIFFUI_H
