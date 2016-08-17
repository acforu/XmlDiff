#include "StdAfx.h"
#include "HotPointBar.h"
#include "Common.h"


HotPointBar::HotPointBar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	curBlock = 0;
	maxBlock = 0;
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
		painter.drawRect(2,y,width()-2,4);
	}
}

void HotPointBar::SetHotSegments( std::vector<std::pair<int,int>>& segments, int totalBlock )
{
	//qDebug() << "SetHotSegments" << totalBlock << segments.size() << endl;
	this->segments = segments;
	this->maxBlock = max(totalBlock-1,1);
}

void HotPointBar::NotifyCurBlock( int cur )
{
	curBlock = cur;
	repaint();
}

void HotPointBar::mousePressEvent( QMouseEvent *event )
{
	QWidget::mousePressEvent(event);
	int y = event->y();

	int targetblock = PixelToBlock(y);
	int pixel = BlockToPiexl(targetblock);
	emit selectBlock(targetblock);
}

int HotPointBar::BlockToPiexl( int block )
{
	if (maxBlock*HOT_SEGMENT_PIXEL_PER_BLOCK >= height())
	{
		return float(block)/(maxBlock)*height();
	}
	else
	{
		return block*HOT_SEGMENT_PIXEL_PER_BLOCK;
	}
}

int HotPointBar::PixelToBlock( int y )
{
	int maxPixel = BlockToPiexl(maxBlock);
	y = min(y,maxPixel);
	return float(y) / maxPixel* (maxBlock);
}

void HotPointBar::mouseMoveEvent( QMouseEvent *event )
{
	QWidget::mouseMoveEvent(event);
	if(event->buttons() == Qt::LeftButton)
	{
		int y = event->y();
		int targetblock = PixelToBlock(y);
		int pixel = BlockToPiexl(targetblock);
		emit selectBlock(targetblock);
	}
}



