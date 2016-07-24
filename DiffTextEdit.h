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

signals:
	void scrollContentChanged();

private slots:
	void update();
private:
	Ui::DiffTextEdit ui;
	int deltaX;
	int deltaY;
};

#endif // DIFFTEXTEDIT_H
