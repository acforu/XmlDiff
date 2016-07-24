#include "StdAfx.h"
#include "DiffTextEdit.h"


DiffTextEdit::DiffTextEdit(QWidget *parent)
	: QPlainTextEdit(parent)
{
	ui.setupUi(this);
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(1000);

	deltaX = deltaY = 0;


}

DiffTextEdit::~DiffTextEdit()
{

}

void DiffTextEdit::scrollContentsBy( int dx, int dy )
{
	QPlainTextEdit::scrollContentsBy(dx,dy);	
	//emit scrollContentChanged();
}

void DiffTextEdit::update()
{
	//if(deltaX != 0 || deltaY != 0)
	//{
	//	QPlainTextEdit::scrollContentsBy(deltaX,deltaY);		
	//	deltaX = deltaY = 0;
	//}
}
