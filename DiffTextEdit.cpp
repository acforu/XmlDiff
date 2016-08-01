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

QTextBlock DiffTextEdit::firstBlockInViewport() const
{
	return QPlainTextEdit::firstVisibleBlock();
}

int DiffTextEdit::BlockTopLinePos(int blockNum)
{
	QTextBlock block = document()->findBlockByNumber(blockNum); 

	int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
	//int bottom = top + (int) blockBoundingRect(block).height();

	return top;
}

void DiffTextEdit::paintEvent( QPaintEvent *event )
{
	//{
	//	QPainter painter(viewport());
	//	painter.eraseRect(viewport()->rect());
	//}


	QPlainTextEdit::paintEvent(event);

	
	QPainter painter(viewport());	
	painter.setPen(QPen(Qt::black, 12, Qt::SolidLine, Qt::RoundCap));


	int top = BlockTopLinePos(curDiffBlockNum);
	painter.drawLine(0, top, viewport()->width(), top);


}

//void DiffTextEdit::changeEvent( QEvent *e )
//{
//		qDebug() << "changeEvent" <<endl;
//}
