#include "StdAfx.h"
#include "DiffTextEdit.h"


DiffTextEdit::DiffTextEdit(QWidget *parent)
	: QPlainTextEdit(parent)
{
	ui.setupUi(this);
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(1000);

	Reset();
}

DiffTextEdit::~DiffTextEdit()
{

}

void DiffTextEdit::scrollContentsBy( int dx, int dy )
{
	QPlainTextEdit::scrollContentsBy(dx,dy);	
	//emit scrollContentChanged();
}

//void DiffTextEdit::update()
//{
//}

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

void DiffTextEdit::mousePressEvent(QMouseEvent* event)
{
	QTextCursor tc = cursorForPosition(event->pos());
	tc.block();
	emit selectBlock(tc.block().blockNumber());
}


void DiffTextEdit::paintEvent(QPaintEvent* event)
{
	QPlainTextEdit::paintEvent(event);

	if (diffBeginBlockNum < 0 || diffEndBlockNum < 0)
	{
		return;
	}

	QPainter painter(viewport());
	painter.setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::FlatCap));


	int top = BlockTopLinePos(diffBeginBlockNum);
	painter.drawLine(0, top, viewport()->width(), top);

	int bottom = BlockTopLinePos(diffEndBlockNum);//+ (int) blockBoundingRect(document()->findBlockByNumber(diffEndBlockNum)).height();
	painter.drawLine(0, bottom, viewport()->width(), bottom);

}

QTextBlock DiffTextEdit::lastBlockInViewport()
{
	QTextBlock firstBlock = firstBlockInViewport();
	int ret = firstBlock.blockNumber();
	for(int i= firstBlock.blockNumber() + 1 ; i < this->document()->blockCount(); ++i)
	{
		QTextBlock block = document()->findBlockByNumber(i);

		QRect r1 = this->viewport()->geometry();
		r1.setX(0);
		r1.setY(0);
		QRect r2 = blockBoundingGeometry(block).translated(contentOffset()).toRect();

		//QRect r2 = this->document()->documentLayout()->blockBoundingRect(block).translated(
		//	this->viewport()->geometry().x(), this->viewport()->geometry().y() - (
		//	this->verticalScrollBar()->sliderPosition()
		//	) ).toRect();

		//qDebug() << "lastBlockInViewport" << r1 << r2 <<endl;

		if (r1.intersects(r2)) 
			{ ret = i; }
		else
			break;
	}

	return document()->findBlockByNumber(ret);
}

void DiffTextEdit::Reset()
{
	diffBeginBlockNum = diffEndBlockNum = -1;
}

//void DiffTextEdit::changeEvent( QEvent *e )
//{
//		qDebug() << "changeEvent" <<endl;
//}
