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
	QTextBlock lastBlockInViewport();
    int BlockTopLinePos(int blockNum);

	int DiffBeginBlockNum() const { return diffBeginBlockNum; }
	void DiffBeginBlockNum(int val) { diffBeginBlockNum = val; }

	int DiffEndBlockNum() const { return diffEndBlockNum; }
	void DiffEndBlockNum(int val) { diffEndBlockNum = val; }
protected:
	void paintEvent(QPaintEvent *event);
	//void changeEvent(QEvent *e);
signals:
	void scrollContentChanged();

private slots:
	void update();
private:
	Ui::DiffTextEdit ui;
	int diffBeginBlockNum;
	int diffEndBlockNum;

};

#endif // DIFFTEXTEDIT_H
