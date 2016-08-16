#include "stdafx.h"
#include "diffui.h"
#include "XmlDiff.h"
#include "StringBuff.h"
//#pragma optimize("",off)

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

	//QFont font = QFont("Microsoft YaHei UI", 11, QFont::Normal, false);
	//QFont font = QFont("Courier New", 11, QFont::Normal, false);
	//textEditL->setFont(font);
	//textEditR->setFont(font);

	//QFont font = QFont("WenQuanYi Zen Hei Mono", 11, QFont::Normal, false);
	QFont font = textEditL->font();
	font.setPointSize(11);
	textEditL->setFont(font);
	textEditR->setFont(font);

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

	nextModifyAction = new QAction(QIcon(":/right.png"), tr("next diff(ctrl+2)"), this); 
	//nextModifyAction->setShortcuts(QKeySequence::Open);
	//nextModifyAction->setStatusTip(tr("Open an existing file"));
	//nextModifyAction->setWhatsThis(tr("next diff"));
	nextModifyAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_2));
	connect(nextModifyAction, SIGNAL(triggered()), this, SLOT(nextDiffLine()));


	prevModifyAction = new QAction(QIcon(":/left.png"), tr("prev diff(ctrl+1)"), this); 
	prevModifyAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_1));
	//nextModifyAction->setShortcuts(QKeySequence::Open);
	//nextModifyAction->setStatusTip(tr("Open an existing file"));
	//nextModifyAction->setWhatsThis(tr("next diff"));

	connect(prevModifyAction, SIGNAL(triggered()), this, SLOT(prevDiffLine()));

	
	QAction* exitAction = new QAction(QIcon(":/close.png"),"Exit",this); 
	exitAction->setShortcut(QKeySequence(Qt::Key_Escape));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(exitApp()));

	QAction* switchAppAction = new QAction(QIcon(":/bc.png"),"beyond compare",this); 
	connect(switchAppAction, SIGNAL(triggered()), this, SLOT(switchApp()));


	QAction* switchHideUnchanged = new QAction(QIcon(":/hide.png"),"Hide Unchanged",this); 
	connect(switchHideUnchanged, SIGNAL(triggered()), this, SLOT(hideUnchanged()));
	switchHideUnchanged->setCheckable(true);

	ui.mainToolBar->addAction(prevModifyAction);
	ui.mainToolBar->addAction(nextModifyAction);
	ui.mainToolBar->addAction(switchAppAction);
	ui.mainToolBar->addAction(switchHideUnchanged);
	ui.mainToolBar->addAction(exitAction);

	ui.mainToolBar->setFixedHeight(48);
	ui.mainToolBar->setIconSize(QSize(48,48));


	curHighLightBeginBlockNum = -1;

	diffInst = new XmlDiff(this);

	encodeType = EncodeType_UTF8;

	ui.actionUtf_8->setChecked(true);
	connect(ui.actionANSI, SIGNAL(triggered()), this, SLOT(switchANSI()));
	connect(ui.actionUtf_8, SIGNAL(triggered()), this, SLOT(switchUTF8()));

	

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
	if (side == TextSide_Both)
	{
		AppendText(text,TextSide_Left,type);
		AppendText(text,TextSide_Right,type);
		return;
	}
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
	 //format.setFontPointSize(12);
	 if (type == TextColor_Modify)
	 {
		format.setFontWeight(QFont::Bold);
	 }
	 QString the_text;
	 /* some computations to get the text to insert and
	 set the local character formatting, which varies inside
	 a line, which is why I don't use the third, optional argument
	 to insertText
	 */
	 //cursor->beginEditBlock();

	 if (encodeType == EncodeType_UTF8)
	 {
		cursor->insertText(QString::fromUtf8(text),format);
	 }
	 else if (encodeType == EncodeType_ANSI)
	 {
		cursor->insertText(QString::fromLocal8Bit(text),format);
	 }

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

void DiffUI::AppendText( StringBuff& buff,TexTSide side, TextFormatType type )
{
	buff.AppendNull();
	AppendText(buff.Data(),side,type);
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

	textEditL->viewport()->update();
	textEditR->viewport()->update();
	
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

	textEditL->viewport()->update();
	textEditR->viewport()->update();
}

int DiffUI::GetTotalBlocks()
{
	//assert(textEditL->blockCount() == textEditR->blockCount());
	return textEditL->blockCount();
}

void DiffUI::MoveToBlock( int block )
{
	
	//int curBlockNum  = textEditL->firstBlockInViewport().blockNumber();


	//qDebug() << "MoveToBlock " << block << endl;

	int lineCount = textEditL->height() /(textEditL->fontMetrics().height() + textEditL->fontMetrics().lineSpacing());

	//qDebug() << "lineCount " << lineCount << endl;

	if (block < textEditL->document()->blockCount())
	{
		textEditL->moveCursor(QTextCursor::End);
		QTextCursor cursorL(textEditL->document()->findBlockByNumber(block)); 
		cursorL.movePosition(QTextCursor::Up,QTextCursor::MoveAnchor,lineCount/2);

		textEditL->setTextCursor(cursorL);
	}

	if (block < textEditR->document()->blockCount())
	{
		textEditR->moveCursor(QTextCursor::End);
		QTextCursor cursorR(textEditR->document()->findBlockByNumber(block)); 
		cursorR.movePosition(QTextCursor::Up,QTextCursor::MoveAnchor,lineCount/2);

		textEditR->setTextCursor(cursorR);
	}

	HighLightDiffBlocks(block);
	/*qDebug() << "firstBlockVisable " << curBlockNum << endl;
	qDebug() << "block top line " << textEditL->BlockTopLinePos(block) << endl;*/

}

void DiffUI::nextDiffLine()
{
	QTextBlock block = textEditL->firstBlockInViewport();
	int curBlockNum  = block.blockNumber();
	curBlockNum = max(curHighLightBeginBlockNum,curBlockNum);
	auto iter = ModifyBegTags.upper_bound(curBlockNum);
	if (iter != ModifyBegTags.end())
	{
		MoveToBlock(*iter);
	}
	else
	{
		if (!ModifyBegTags.empty())
		{
			auto iter = ModifyBegTags.end();
			MoveToBlock(*(--iter));
		}
	}
}

void DiffUI::ModifyMarkBegin()
{
	ModifyBegTags.insert(GetTotalBlocks()-1);
//	qDebug() << "ModifyMarkBegin" << GetTotalBlocks()-1 <<endl;
}

void DiffUI::ModifyMarkEnd()
{
	ModifyEndTags.insert(GetTotalBlocks()-1);
	//qDebug() << "ModifyMarkEnd" << GetTotalBlocks()-1 <<endl;

}
void DiffUI::prevDiffLine()
{
	QTextBlock block = textEditL->lastBlockInViewport();
	int curBlockNum  = block.blockNumber();
	

	//qDebug() << "prevDiffLine" << curBlockNum << curHighLightBeginBlockNum << endl;
	//qDebug() << "prevDiffLine" << curBlockNum << block.text() << endl;

	curBlockNum = min(curBlockNum,curHighLightBeginBlockNum);
	auto iter = ModifyBegTags.lower_bound(curBlockNum);
	if (iter != ModifyBegTags.begin())
	{
		int target = *(--iter);
		MoveToBlock(target);
	}
	else
	{
		if (!ModifyBegTags.empty())
		{
			int target = *(ModifyBegTags.begin());
			MoveToBlock(target);
		}
			
	}
}

void DiffUI::AppendNewLine()
{
	AppendText("\n",TextSide_Left,TextColor_Normal);
	AppendText("\n",TextSide_Right,TextColor_Normal);
}

//void DiffUI::HighLightBlocks( int beg,int end )
//{
//	/*QTextBlock block = textEditL->document()->findBlockByNumber(beg); 
//
//	int top = (int) textEditL->blockBoundingGeometry(block).translated(textEditL->contentOffset()).top();
//	int bottom = top + (int) textEditL->blockBoundingRect(block).height();*/
//
//}


void DiffUI::HighLightDiffBlocks( int startblockNum )
{
	auto iter = ModifyEndTags.upper_bound(startblockNum);
	if (iter == ModifyEndTags.end())
		return;

	int endBlockNum = *iter;

	textEditL->DiffBeginBlockNum(startblockNum);
	textEditR->DiffBeginBlockNum(startblockNum);

	textEditL->DiffEndBlockNum(endBlockNum);
	textEditR->DiffEndBlockNum(endBlockNum);

	//qDebug() << "HighLightDiffBlocks" << startblockNum << endBlockNum << endl;

	curHighLightBeginBlockNum = startblockNum;
}

bool DiffUI::Diff( std::string file1, std::string file2)
{
	this->file1 = file1.c_str();
	this->file2 = file2.c_str();

	bool ret = diffInst->Diff(file1,file2);
	if (ret)
	{
		diffInst->RenderText();

		textEditL->moveCursor(QTextCursor::Start);
		textEditR->moveCursor(QTextCursor::Start);
	}
	return ret;
}

void DiffUI::exitApp()
{
	QApplication::quit();
}

void DiffUI::switchApp()
{
	UseBeyondCompare();

	////system("dir");
	//qDebug() << appPath << endl;

	//qDebug() << params.toLocal8Bit().constData()<< endl;
}

void DiffUI::switchUTF8()
{
	if(encodeType != EncodeType_UTF8)
	{
		encodeType = EncodeType_UTF8;
		ClearText();
		ui.actionANSI->setChecked(false);
		diffInst->RenderText();
	}
}

void DiffUI::switchANSI()
{
	if(encodeType != EncodeType_ANSI)
	{
		encodeType = EncodeType_ANSI;
		ClearText();
		ui.actionUtf_8->setChecked(false);
		diffInst->RenderText();
	}
}

void DiffUI::ClearText()
{
	ClearModifyMark();

	textEditL->clear();
	textEditR->clear();

	textEditL->Reset();
	textEditR->Reset();

	curHighLightBeginBlockNum = -1;
}

void DiffUI::UseBeyondCompare()
{
	::UseBeyondCompare(file1,file2);
}

void DiffUI::hideUnchanged()
{
	diffInst->HideUnchangedNode(!diffInst->HideUnchangedNode());
	ClearText();
	diffInst->RenderText();
}

void DiffUI::ClearModifyMark()
{
	ModifyBegTags.clear();
	ModifyEndTags.clear();
}

void UseBeyondCompare( QString fileL, QString fileR )
{
	QString params = QString(" ") + "\"" + fileL + "\" \"" +fileR + "\"";
	QString appPath = QCoreApplication::applicationDirPath() + QString("//BCompare.exe");
	ShellExecuteA(NULL, "open", appPath.toLocal8Bit().constData(),params.toLocal8Bit().constData(), NULL, SW_SHOWNORMAL);		
}

