#include "stdafx.h"
#include "diffui.h"

DiffUI::DiffUI(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	textEditL = new DiffTextEdit(ui.centralWidget);
	textEditL->setLineWrapMode(QPlainTextEdit::NoWrap);
	textEditL->setObjectName(QString::fromUtf8("textEditL"));


	textEditR = new DiffTextEdit(ui.centralWidget);
	textEditR->setLineWrapMode(QPlainTextEdit::NoWrap);
	textEditR ->setObjectName(QString::fromUtf8("textEditR"));

	//QScrollBar* bar = new QScrollBar(this);
	//textEditL->setVerticalScrollBar(bar);
	//textEditR->setVerticalScrollBar(bar);
	//

	//QScrollBar* hbar = new QScrollBar(this);
	//textEditL->setHorizontalScrollBar(hbar);
	//textEditR->setHorizontalScrollBar(hbar);

	cursorL = new QTextCursor(textEditL->document());
	cursorR = new QTextCursor(textEditR->document());


	//textEditPlain = new QPlainTextEdit(ui.centralWidget);
	//textEditPlain->setLineWrapMode(QPlainTextEdit::NoWrap);
	//textEditPlain ->setObjectName(QString::fromUtf8("textEditPlaint"));

	//cursorL = new QTextCursor(textEditPlain->document());
	//cursorR = cursorL;

	QObject::connect(textEditL,SIGNAL(updateRequest(const QRect & , int )),this,SLOT(onLTextUpdateRequest(const QRect & , int )));
	QObject::connect(textEditR,SIGNAL(updateRequest(const QRect & , int )),this,SLOT(onRTextUpdateRequest(const QRect & , int )));

}

DiffUI::~DiffUI()
{
}

void DiffUI::resizeEvent( QResizeEvent *event )
{
//	qDebug() << "resizeEvent" <<ui.centralWidget->width() << endl;
	textEditL->setGeometry(QRect(0, 0, ui.centralWidget->width()*0.5, ui.centralWidget->height()));
	textEditR->setGeometry(QRect(ui.centralWidget->width()*0.5, 0, ui.centralWidget->width()*0.5, ui.centralWidget->height()));

	//textEditPlain->setGeometry(QRect(0, 0, ui.centralWidget->width()*0.5, ui.centralWidget->height()));
}

void DiffUI::AppendText( const char* text,TexTSide side, TextFormatType type )
{
	//textEditL->setUpdatesEnabled( false );
	//textEditPlain->appendPlainText(text);
	//textEditL->setUpdatesEnabled( true );
	//return;

	//if (side == TextSide_Left)
	//{
	//	textEditL->insertPlainText(text);
	//}
	//else
	//{
	//	textEditR->insertPlainText(text);
	//}
	//return;

	QTextCursor* cursor = nullptr;
	if (side == TextSide_Left)
		cursor = cursorL;
	else
		cursor = cursorR;

	 QTextBlockFormat bf = cursor->blockFormat();
	 if (type == TextColor_Modify)
	 {
		bf.setBackground(QBrush(QColor(255,227,227)));
	 }
	 else
	 {
		bf.clearBackground();
	 }

	 cursor->setBlockFormat(bf);

	 QTextCharFormat format;
	 QString the_text;
	 /* some computations to get the text to insert and
	 set the local character formatting, which varies inside
	 a line, which is why I don't use the third, optional argument
	 to insertText
	 */
	 //cursor->beginEditBlock();
	 cursor->insertText(text,format);
	 cursor->insertText("\n");
	 //cursor->endEditBlock();


	 //cursor.movePosition(QTextCursor::Down);
}

void DiffUI::BeginEditBlock()
{
	 cursorL->beginEditBlock();
	 cursorR->beginEditBlock();
}

void DiffUI::EndEditBlock()
{
	 cursorL->endEditBlock();
	 cursorR->endEditBlock();
}

void DiffUI::onLScrollContentChanged()
{
	textEditR->horizontalScrollBar()->setSliderPosition(textEditL->horizontalScrollBar()->sliderPosition());
	textEditR->verticalScrollBar()->setSliderPosition(textEditL->verticalScrollBar()->sliderPosition());
}

void DiffUI::onRScrollContentChanged()
{
	textEditL->horizontalScrollBar()->setSliderPosition(textEditR->horizontalScrollBar()->sliderPosition());
	textEditL->verticalScrollBar()->setSliderPosition(textEditR->verticalScrollBar()->sliderPosition());
}

void DiffUI::onValueChanged(int value)
{
	qDebug() << value << endl;
}

void DiffUI::onLTextUpdateRequest( const QRect & rect, int dy )
{
	static int prevVBarPos = 0;
	int newVBarPos = textEditL->verticalScrollBar()->sliderPosition();
	if (newVBarPos != prevVBarPos)
	{
		prevVBarPos = newVBarPos;
		textEditR->verticalScrollBar()->setSliderPosition(newVBarPos);
	}


	static int prevHBarPos = 0;
	int newHBarPos = textEditL->horizontalScrollBar()->sliderPosition();
	if (newHBarPos != prevHBarPos)
	{
		prevHBarPos = newHBarPos;
		textEditR->horizontalScrollBar()->setSliderPosition(newHBarPos);
	}
}

void DiffUI::onRTextUpdateRequest( const QRect & rect, int dy )
{
	static int prevVBarPos = 0;
	int newVBarPos = textEditR->verticalScrollBar()->sliderPosition();
	if (newVBarPos != prevVBarPos)
	{
		prevVBarPos = newVBarPos;
		textEditL->verticalScrollBar()->setSliderPosition(newVBarPos);
	}


	static int prevHBarPos = 0;
	int newHBarPos = textEditR->horizontalScrollBar()->sliderPosition();
	if (newHBarPos != prevHBarPos)
	{
		prevHBarPos = newHBarPos;
		textEditL->horizontalScrollBar()->setSliderPosition(newHBarPos);
	}
}

