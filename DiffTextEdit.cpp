#include "StdAfx.h"
#include "DiffTextEdit.h"

DiffTextEdit::DiffTextEdit(QWidget *parent)
	: QPlainTextEdit(parent)
{
	ui.setupUi(this);
}

DiffTextEdit::~DiffTextEdit()
{

}

void DiffTextEdit::scrollContentsBy( int dx, int dy )
{
	QPlainTextEdit::scrollContentsBy(dx,dy);
	emit scrollContentChanged();
}
