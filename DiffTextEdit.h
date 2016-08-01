#ifndef DIFFTEXTEDIT_H
#define DIFFTEXTEDIT_H

#include <QPlainTextEdit>
#include "ui_DiffTextEdit.h"

class DiffTextEdit : public QPlainTextEdit
{
	Q_OBJECT

public:
	DiffTextEdit(QWidget *parent = 0);
	~DiffTextEdit();

    virtual void scrollContentsBy(int dx, int dy);

    QTextBlock firstBlockInViewport() const;
    int BlockTopLinePos(int blockNum);

	int CurDiffBlockNum() const { return curDiffBlockNum; }
	void CurDiffBlockNum(int val) { curDiffBlockNum = val; }

protected:
	void paintEvent(QPaintEvent *event);
	//void changeEvent(QEvent *e);
signals:
	void scrollContentChanged();

private slots:
	void update();
private:
	Ui::DiffTextEdit ui;
	int deltaX;
	int deltaY;
	int curDiffBlockNum;
	
};

#endif // DIFFTEXTEDIT_H
