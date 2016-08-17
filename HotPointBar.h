#ifndef HOTPOINTBAR_H
#define HOTPOINTBAR_H

#include <QWidget>
#include "ui_HotPointBar.h"

class HotPointBar : public QWidget
{
	Q_OBJECT

public:
	HotPointBar(QWidget *parent = 0);
	~HotPointBar();

	void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

	void SetHotSegments(std::vector<std::pair<int,int>>& segments, int totalBlock);
	void NotifyCurBlock(int cur);

private:
	int BlockToPiexl(int block);
	int PixelToBlock(int y);
Q_SIGNALS:
    void selectBlock(int block);

private:
	Ui::HotPointBar ui;

	std::vector<std::pair<int,int>> segments;
	int maxBlock;
	int curBlock;
};

#endif // HOTPOINTBAR_H
