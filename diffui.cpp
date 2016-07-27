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

	const int tabStop = 4;  // 4 characters
	QFontMetrics metrics(textEditL->font());
	textEditL->setTabStopWidth(tabStop * metrics.width(' '));
	textEditR->setTabStopWidth(tabStop * metrics.width(' '));

	textEditL->setReadOnly(true);
	textEditL->setUndoRedoEnabled(false);

	QPalette p = textEditL->palette(); // define pallete for textEdit.. 
	p.setColor(QPalette::Base, Qt::white); // set color "Red" for textedit base
	//p.setColor(QPalette::Text, Qt::blue); // set text color which is selected from color pallete
	textEditL->setPalette(p);
	textEditR->setPalette(p);



	//textEditL->setLineWrapMode(QPlainTextEdit::NoWrap);
	//textEditL->setCenterOnScroll(true);

	textEditR->setReadOnly(true);
	textEditR->setUndoRedoEnabled(false);
	//textEditR->setLineWrapMode(QPlainTextEdit::NoWrap);
	//textEditR->setCenterOnScroll(true);

	//textEditL->setMaximumBlockCount(20);
	//textEditR->setMaximumBlockCount(20);
	//textEditL->settab

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

	nextModifyAction = new QAction(QIcon("./resource/Right-96.png"), tr("next diff(ctrl+2)"), this); 
	//nextModifyAction->setShortcuts(QKeySequence::Open);
	//nextModifyAction->setStatusTip(tr("Open an existing file"));
	//nextModifyAction->setWhatsThis(tr("next diff"));
	nextModifyAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_2));
	connect(nextModifyAction, SIGNAL(triggered()), this, SLOT(nextDiffLine()));


	prevModifyAction = new QAction(QIcon("./resource/Left-96.png"), tr("prev diff(ctrl+1)"), this); 
	prevModifyAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_1));
	//nextModifyAction->setShortcuts(QKeySequence::Open);
	//nextModifyAction->setStatusTip(tr("Open an existing file"));
	//nextModifyAction->setWhatsThis(tr("next diff"));

	connect(prevModifyAction, SIGNAL(triggered()), this, SLOT(prevDiffLine()));

	ui.mainToolBar->addAction(prevModifyAction);
	ui.mainToolBar->addAction(nextModifyAction);
	ui.mainToolBar->setFixedHeight(48);
	ui.mainToolBar->setIconSize(QSize(48,48));
	
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
	//if (side == TextSide_Left)
	//{
	//	qDebug() << "block prev " << GetTotalBlocks() << endl;
	//}
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
		 //236,236,255 blue
		 //217,255,217 green
		 //255,227,227 red
		 if (side == TextSide_Left)
		 {
			bf.setBackground(QBrush(QColor(236,236,255)));
		 }
		 else
		 {
			bf.setBackground(QBrush(QColor(217,255,217)));
		 }
	 }
	 else
	 {
		 bf.clearBackground();
	 }




	 cursor->setBlockFormat(bf);

	 QTextCharFormat format;
	 format.setFontPointSize(12);
	 if (type == TextColor_Modify)
	 {
		//format.setFontWeight(QFont::Bold);
	 }
	 QString the_text;
	 /* some computations to get the text to insert and
	 set the local character formatting, which varies inside
	 a line, which is why I don't use the third, optional argument
	 to insertText
	 */
	 //cursor->beginEditBlock();
	 cursor->insertText(QString::fromUtf8(text),format);
	 //textEditL->appendPlainText(text);
	
	 //cursor->insertText("\n");
	 //cursor->endEditBlock();

	 //if (side == TextSide_Left)
	 //{
		// qDebug() << "content: " << QString::fromUtf8(text) << endl;
		// qDebug() << "block after " << GetTotalBlocks() << endl;
	 //}
	 
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

int DiffUI::GetTotalBlocks()
{
	//assert(textEditL->blockCount() == textEditR->blockCount());
	return textEditL->blockCount();
}

void DiffUI::MoveToBlock( int block )
{
	//textEditL->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
	//QTextCursor cursor = textEditL->textCursor();
	////cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, block);
	//cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, 5);
	//textEditL->setTextCursor(cursor);
	//return;
	int curBlockNum  = textEditL->firstBlockInViewport().blockNumber();

	qDebug() << "firstBlockVisable " << curBlockNum << endl;
	qDebug() << "MoveToBlock " << block << endl;

	if (block < textEditL->document()->blockCount())
	{
		textEditL->moveCursor(QTextCursor::End);
		QTextCursor cursorL(textEditL->document()->findBlockByNumber(block)); 
		textEditL->setTextCursor(cursorL);
	}

	if (block < textEditR->document()->blockCount())
	{
		textEditR->moveCursor(QTextCursor::End);
		QTextCursor cursorR(textEditR->document()->findBlockByNumber(block)); 
		textEditR->setTextCursor(cursorR);
	}

}

void DiffUI::nextDiffLine()
{
	QTextBlock block = textEditL->firstBlockInViewport();
	int curBlockNum  = block.blockNumber();
	auto iter = ModifyTags.upper_bound(curBlockNum);
	if (iter != ModifyTags.end())
	{
		MoveToBlock(*iter);
	}
}

void DiffUI::MarkModifyTag()
{
	ModifyTags.insert(GetTotalBlocks()-1);
}

void DiffUI::prevDiffLine()
{
	QTextBlock block = textEditL->firstBlockInViewport();
	int curBlockNum  = block.blockNumber();
	auto iter = ModifyTags.lower_bound(curBlockNum);
	if (iter != ModifyTags.begin())
	{
		int target = *(--iter);
		MoveToBlock(target);
	}
}

