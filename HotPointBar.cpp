#include "StdAfx.h"
#include "HotPointBar.h"
#include "Common.h"



HotPointBar::HotPointBar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	curBlock = 0;
	maxBlock = 0;

	//QTimer::singleShot(200, this, SLOT(updateHotPointBar()));

	//QTimer *timer = new QTimer(this);
	//connect(timer, SIGNAL(timeout()), this, SLOT(updateHotPointBar()));
	//timer->start(20);

	curBlockChanged = false;
}

HotPointBar::~HotPointBar()
{

}


void HotPointBar::paintEvent( QPaintEvent *event )
{
	QWidget::paintEvent(event);

	QPainter painter(this);	
	//painter.drawLine(0, 0, width(), 30);
	//qDebug() << "totalblock " << this->totalBlock << this->segments.size() << endl;

	if (maxBlock == 0)
		return;

	FOR_EACH(iter,segments)
	{
		painter.setPen(QPen(Qt::red));
		int x = HOT_SEGMENT_MARGIN;
		int y = BlockToPiexl(iter->first);

		int w = width()- HOT_SEGMENT_MARGIN*2;
		int offset = max(iter->second - iter->first,1);
		int h = max(BlockToPiexl(offset),1);

		painter.setBrush(Qt::red); 
		painter.drawRect(x,y,w,h);
		//qDebug() << x << y << w << h << endl;
	}

	{
		painter.setPen(QPen(Qt::blue,1.5));
		//painter.setBrush(Qt::blue); 
		painter.setBrush(Qt::NoBrush);

		int y = BlockToPiexl(curBlock);
		//qDebug() << "drawRect" << y << endl;
		painter.drawRect(2,y,width()-2,pagePixel);
	}

	curBlockChanged = false;
}

void HotPointBar::SetHotSegments( std::vector<std::pair<int,int>>& segments, int totalBlockCount,int pageBlockCount )
{
	//qDebug() << "SetHotSegments" << totalBlock << segments.size() << endl;
	this->segments = segments;
	this->maxBlock = max(totalBlockCount-1,1);
	this->pageBlockCount = pageBlockCount;
	pagePixel = BlockToPiexl(pageBlockCount) /*- BlockToPiexl(0)*/;
	maxWinUpperPixel = height() - pagePixel;
}

void HotPointBar::NotifyCurBlock( int cur )
{
	if (cur != curBlock)
	{
		//qDebug() << "NotifyCurBlock" << cur << endl;
		curBlock = cur;
		//curBlockChanged = true;
		repaint();
	}
}

void HotPointBar::mousePressEvent( QMouseEvent *event )
{
	QWidget::mousePressEvent(event);
	int y = event->y();

	int targetblock = PixelToBlock(y);
	emit selectBlock(targetblock);
}

int HotPointBar::BlockToPiexl( int block )
{
	int ret;
	if (maxBlock*HOT_SEGMENT_PIXEL_PER_BLOCK >= height())
	{
		ret = float(block)/(maxBlock)* (height());
	}
	else
	{
		ret = block*HOT_SEGMENT_PIXEL_PER_BLOCK;
	}

	//QString status = QString("BlockToPiexl block:%1, ret:%2: maxBlock:%3 height:%4 pagePixel:%5 pageBlockCount:%6")  
	//	.arg(block).arg(ret).arg(maxBlock).arg(height()).arg(pagePixel).arg(pageBlockCount);  
	//qDebug() << status << endl;
	return ret;
}

int HotPointBar::PixelToBlock( int y )
{
	int block;
	y = min(y,maxWinUpperPixel);
	if (y == maxWinUpperPixel)
	{
		//qDebug() << "y == maxWinUpperPixel" << endl;
		block =  maxBlock /*- pageBlockCount*/;
	}
	else
	{
		if (maxBlock*HOT_SEGMENT_PIXEL_PER_BLOCK >= height())
		{
			block = float(y) / height()* (maxBlock);
		}
		else
		{
			block = float(y)/HOT_SEGMENT_PIXEL_PER_BLOCK;
		}

	}
	//qDebug() << "PixelToBlock" << y << block << maxWinUpperPixel << maxBlock << pageBlockCount <<  endl;
	//assert(y == BlockToPiexl(block));
	return block;
}

void HotPointBar::mouseMoveEvent( QMouseEvent *event )
{
	QWidget::mouseMoveEvent(event);
	if(event->buttons() == Qt::LeftButton)
	{
		int y = event->y();
		int targetblock = PixelToBlock(y);
		//qDebug() << "mouseMoveEvent" << y << targetblock << endl;
		//targetblock = min(targetblock,maxBlock-pageBlockCount);
		//int pixel = BlockToPiexl(targetblock);
		emit selectBlock(targetblock);
	}
}

void HotPointBar::updateHotPointBar()
{
//	qDebug() << "updateHotPointBar" <<endl;
	if (curBlockChanged)
	{
		//repaint();
		curBlockChanged = false;
	}
}



