#include "stdafx.h"
#include "diffui.h"
#include "XmlDiff.h"
#include "StringBuff.h"
#include <QScrollBar>
#include <locale>
#include <codecvt>
#include "MyersDiff.h"
//#pragma optimize("",off)


bool GbkToUnicode_NoLimit(const char* pcMbSrc, std::wstring* pwUniStrDest)
{
	if (!pcMbSrc || !pwUniStrDest) return false;

	const unsigned int CP_GBK = 936;

	int dwBefore = MultiByteToWideChar(CP_GBK, 0, pcMbSrc, -1, NULL, 0);
	if (dwBefore <= 0)
	{
		return false;
	}

	wchar_t* pwBuffer = (wchar_t*)malloc((dwBefore + 1) * sizeof(wchar_t));
	if (NULL == pwBuffer)
	{
		return false;
	}

	int dwAfter = MultiByteToWideChar(CP_GBK, 0, pcMbSrc, -1, pwBuffer, dwBefore);

	if (dwBefore == dwAfter)
	{
		pwBuffer[dwBefore] = L'\0';
		*pwUniStrDest = (wchar_t*)pwBuffer;
	}
	free(pwBuffer);
	return dwBefore == dwAfter;
}

bool Utf8ToUnicode_NoLimit(const char* pcUtf8Src, std::wstring* pwUniStrDest)
{
	if (!pcUtf8Src || !pwUniStrDest) return false;

	int dwBefore = MultiByteToWideChar(CP_UTF8, 0, pcUtf8Src, -1, NULL, 0);
	if (dwBefore <= 0)
	{
		return false;
	}

	wchar_t* pwBuffer = (wchar_t*)malloc((dwBefore + 1) * sizeof(wchar_t));
	if (NULL == pwBuffer)
	{
		return false;
	}

	int dwAfter = MultiByteToWideChar(CP_UTF8, 0, pcUtf8Src, -1, pwBuffer, dwBefore);
	if (dwBefore == dwAfter)
	{
		pwBuffer[dwBefore] = L'\0';
		*pwUniStrDest = pwBuffer;
	}

	free(pwBuffer);

	return dwBefore == dwAfter;
}

bool UnicodeToUtf8_NoLimit(const wchar_t* pwUniSrc, std::string* pUtf8StrDest)
{
	if (!pwUniSrc || !pUtf8StrDest) return false;

	int dwBefore = WideCharToMultiByte(CP_UTF8, 0, pwUniSrc, -1, NULL, 0, NULL, NULL);
	if (dwBefore <= 0)
	{
		return false;
	}

	char* pcBuffer = (char*)malloc((dwBefore + 1) * sizeof(char));
	if (NULL == pcBuffer)
	{
		return false;
	}

	int dwAfter = WideCharToMultiByte(CP_UTF8, 0, pwUniSrc, -1, pcBuffer, dwBefore * sizeof(char), NULL, NULL);

	if (dwBefore == dwAfter)
	{
		pcBuffer[dwBefore] = '\0';
		*pUtf8StrDest = pcBuffer;
	}

	free(pcBuffer);

	return dwBefore == dwAfter;
}


DiffUI::DiffUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	textEditL = new DiffTextEdit(ui.centralWidget);
	textEditL->setLineWrapMode(QPlainTextEdit::NoWrap);
	textEditL->setObjectName(QString::fromUtf8("textEditL"));


	textEditR = new DiffTextEdit(ui.centralWidget);
	textEditR->setLineWrapMode(QPlainTextEdit::NoWrap);
	textEditR ->setObjectName(QString::fromUtf8("textEditR"));

	textEditLineCompareUp = new QPlainTextEdit(ui.centralWidget);
	textEditLineCompareUp->setLineWrapMode(QPlainTextEdit::NoWrap);
	textEditLineCompareUp->setObjectName(QString::fromUtf8("textEditLineCompareUp"));

	//textEditLineCompareDown = new QPlainTextEdit(ui.centralWidget);
	//textEditLineCompareDown->setLineWrapMode(QPlainTextEdit::NoWrap);
	//textEditLineCompareDown->setObjectName(QString::fromUtf8("textEditLineCompareDown"));

	QFont font = QFont("Microsoft YaHei UI", 11, QFont::Normal, false);
	//QFont font = QFont("SimSun", 12, QFont::Normal, false);
	
	//QFont font = QFont("Courier New", 11, QFont::Normal, false);
	//textEditL->setFont(font);
	//textEditR->setFont(font);

	//QFont font = textEditL->font();
	//font.setPointSize(11);
	textEditL->setFont(font);
	textEditR->setFont(font);

	QFont lineCompareFont = QFont("Microsoft YaHei UI", 14, QFont::Normal, false);
	textEditLineCompareUp->setFont(lineCompareFont);
	//textEditLineCompareDown->setFont(lineCompareFont);

	const int tabStop = 10;  
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


	textEditLineCompareUp->setReadOnly(true);
	textEditLineCompareUp->setUndoRedoEnabled(false);

	//textEditLineCompareDown->setReadOnly(true);
	//textEditLineCompareDown->setUndoRedoEnabled(false);

	cursorL = new QTextCursor(textEditL->document());
	cursorR = new QTextCursor(textEditR->document());

	cursorTextEditLineCompareUp = new QTextCursor(textEditLineCompareUp->document());
	//cursorTextEditLineCompareDown = new QTextCursor(textEditLineCompareDown->document());

	
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


	
	hotPointBar = new HotPointBar(ui.centralWidget);
	hotPointBar->setObjectName(QString::fromUtf8("hotPointBar"));

	QObject::connect(hotPointBar,SIGNAL(selectBlock(int)),this,SLOT(showSelectBlock(int)));

	//connect(textEditL->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(verticalScrollbarValueChanged(int)));


	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateHotPointBar()));
	timer->start(20);
}

DiffUI::~DiffUI()
{
}

void DiffUI::resizeEvent( QResizeEvent *event )
{
//	qDebug() << "resizeEvent" <<ui.centralWidget->width() << endl;

	const int barWidth = 50;
	const int lineCompareTextAreaHeight = 120;
	const int lineCompareBarHeight = 20;

	const int textWidth = (ui.centralWidget->width() - barWidth) *0.5;
	const int textHeight = ui.centralWidget->height() - lineCompareTextAreaHeight - lineCompareBarHeight;
	textEditL->setGeometry(QRect(barWidth, 0, textWidth, textHeight));
	textEditR->setGeometry(QRect(barWidth+ textWidth, 0, textWidth, textHeight));

	hotPointBar->setGeometry(QRect(0, 0, barWidth, textHeight - 20));
	textEditLineCompareUp->setGeometry(QRect(0, textHeight, ui.centralWidget->width(), lineCompareTextAreaHeight));
}

void DiffUI::AppendText( const char* text,TexTSide side, TextFormatType type )
{
	if (side == TextSide_Both)
	{
		AppendText(text,TextSide_Left,type);
		AppendText(text,TextSide_Right,type);
		return;
	}

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


QString DiffUI::AppendLineCompareText(std::wstring str, TexTSide side, DiffType type, bool singleDiffType)
{	
	//QTextCursor* cursor = nullptr;
	//if (side == TextSide_Left)
	//{
	//	cursor = cursorTextEditLineCompareUp;
	//}
	//else
	//{
	//	cursor = cursorTextEditLineCompareDown;
	//}

	std::string utf8Str;
	bool ret = UnicodeToUtf8_NoLimit(str.c_str(), &utf8Str);
	assert(ret);

	QString text;
	QString originalText = QString::fromUtf8(utf8Str.c_str()).toHtmlEscaped();
		//QString::fromWCharArray(str.c_str()).toHtmlEscaped();

	if (singleDiffType)
	{
		if (type == DiffType_Del)
		{
			text = QString("<span style='color:black; '><s>%1</s></span>").arg(originalText);
		}
		else
		{
			text = QString("<span  style='color:black; '>%1</span>").arg(originalText);
		}
	}
	else
	{
		if (type == DiffType_Add || type == DiffType_Modify)
		{
			text = QString("<span  style='color:red; '><strong>%1</strong></span>").arg(originalText);
		}
		else if (type == DiffType_Del)
		{
			text = QString("<span  style='color:red; '><strong><s>%1</s></strong></span>").arg(originalText);
		}
		else
		{
			text = QString("<span  style='color:black; '>%1</span>").arg(originalText);
		}
	}
	

	return text;

	//cursor->insertHtml(text);
}

void DiffUI::ResetLineCompareText()
{
	textEditLineCompareUp->document()->clear();
	textEditLineCompareUp->moveCursor(QTextCursor::Start);

	//textEditLineCompareDown->document()->clear();
	//textEditLineCompareDown->moveCursor(QTextCursor::Start);
}

void DiffUI::MoveToBlock( int block )
{
	
	//int curBlockNum  = textEditL->firstBlockInViewport().blockNumber();


	//qDebug() << "MoveToBlock " << block << endl;

	int lineCount = PageLineCount();

	//qDebug() << "lineCount " << lineCount << endl;

	std::wstring lineTextLeft, lineTextRight;

	if (block < textEditL->document()->blockCount())
	{
		textEditL->moveCursor(QTextCursor::End);
		QTextCursor cursorL(textEditL->document()->findBlockByNumber(block)); 
		cursorL.movePosition(QTextCursor::Up,QTextCursor::MoveAnchor,lineCount*0.777);

		
		//std::cout << str << std::endl;

		textEditL->setTextCursor(cursorL);

		auto str = GetModifyText(block, true);
		if (!str.isEmpty())
		{
			std::string log = str.toLocal8Bit().constData();
			lineTextLeft = GBKToUnicode(log);
		}
	}

	if (block < textEditR->document()->blockCount())
	{
		textEditR->moveCursor(QTextCursor::End);
		QTextCursor cursorR(textEditR->document()->findBlockByNumber(block)); 
		cursorR.movePosition(QTextCursor::Up,QTextCursor::MoveAnchor,lineCount*0.777);

		textEditR->setTextCursor(cursorR);

		auto str = GetModifyText(block, false);
		if (!str.isEmpty())
		{
			std::string log = str.toLocal8Bit().constData();
			lineTextRight = GBKToUnicode(log);
		}
	}

	HighLightDiffBlocks(block);

	

	std::vector<wchar_t> charVecL(lineTextLeft.begin(),lineTextLeft.end());
	std::vector<wchar_t> charVecR(lineTextRight.begin(), lineTextRight.end());

	auto compare = [](const wchar_t& a, const wchar_t& b)
	{
		return a == b;
	};

	MyersDiff::DiffResult<wchar_t> result = MyersDiff::DiffNodes<wchar_t>(charVecL,charVecR, compare);
	bool singleDiffType = result.results.size() == 1;
	QString leftLine, rightLine;
	for (auto& block : result.results)
	{
		//leftLine.clear();
		if (block.type == DiffType_Add)
		{
			std::wstring text(block.first.begin(),block.first.end());
			rightLine += AppendLineCompareText(text, TextSide_Right, DiffType_Add, singleDiffType);
		}
		else if (block.type == DiffType_Del)
		{
			std::wstring text(block.first.begin(), block.first.end());
			leftLine += AppendLineCompareText(text, TextSide_Left, DiffType_Del, singleDiffType);
		}
		else if (block.type == DiffType_Modify)
		{
			std::wstring textL(block.first.begin(), block.first.end());
			leftLine += AppendLineCompareText(textL, TextSide_Left, DiffType_Modify, singleDiffType);

			std::wstring textR(block.second.begin(), block.second.end());
			rightLine += AppendLineCompareText(textR, TextSide_Right, DiffType_Modify, singleDiffType);
		}
		else
		{
			std::wstring text(block.first.begin(), block.first.end());
			leftLine += AppendLineCompareText(text, TextSide_Left, DiffType_Unchanged, singleDiffType);
			rightLine += AppendLineCompareText(text, TextSide_Right, DiffType_Unchanged, singleDiffType);
		}
	}
	ResetLineCompareText();
	cursorTextEditLineCompareUp->insertHtml(leftLine + "<br>" + rightLine);
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

void DiffUI::UpdateHotSegments()
{
	assert(ModifyBegTags.size() == ModifyEndTags.size());
	auto beg = ModifyBegTags.begin();
	auto end = ModifyEndTags.begin();

	std::vector<std::pair<int,int>> segments;

	while (beg != ModifyBegTags.end())
	{
		segments.push_back(make_pair(*beg++,*end++));
	}

	hotPointBar->SetHotSegments(segments,GetTotalBlocks(),PageLineCount());
	hotPointBar->repaint();
}

void DiffUI::showSelectBlock( int block )
{
	int lineCount = PageLineCount();

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

	//hotPointBar->NotifyCurBlock(block);
}

//void DiffUI::verticalScrollbarValueChanged( int value )
//{
//	//hotPointBar->NotifyCurBlock(textEditL->firstBlockInViewport().blockNumber());
//}

int DiffUI::PageLineCount()
{
	float lineCount = float(VisualContentHeight()) /textEditL->fontMetrics().lineSpacing() - 1;
	//qDebug() << "PageLineCount" << VisualContentHeight() << textEditL->fontMetrics().lineSpacing()  << lineCount << endl;
	return lineCount;
}

int DiffUI::VisualContentHeight()
{
	int textHeight = textEditL->height();
	if (textEditL->horizontalScrollBar()->isVisible())
	{
		textHeight -= textEditR->horizontalScrollBar()->sizeHint().height() ;
	}
	return textHeight;
}

QString DiffUI::GetModifyText(int startBlock, bool isLeft)
{
	QString ret;

	auto iter = ModifyEndTags.upper_bound(startBlock);
	if (iter == ModifyEndTags.end())
		return ret;

	int endBlock = *iter;
	DiffTextEdit* textEdit = isLeft ? textEditL : textEditR;

	for (int i = startBlock; i < endBlock; ++i)
	{
		auto textBlock = textEdit->document()->findBlockByNumber(i);
		ret += textBlock.text();
	}

	return ret;
}


const static int kMaxCharacterLength = 1024 * 5;

bool Utf8ToUnicode(const char* pcUtf8Src, std::wstring* pwUniStrDest)
{
	if (!pcUtf8Src || !pwUniStrDest) return false;

	wchar_t buffer[kMaxCharacterLength];

	int dwBefore = MultiByteToWideChar(CP_UTF8, 0, pcUtf8Src, -1, NULL, 0);
	if (dwBefore < 0 || dwBefore >= kMaxCharacterLength)
	{
		return false;
	}

	wchar_t* pwBuffer = buffer;

	int dwAfter = MultiByteToWideChar(CP_UTF8, 0, pcUtf8Src, -1, pwBuffer, dwBefore);
	if (dwBefore == dwAfter)
	{
		pwBuffer[dwBefore] = L'\0';
		*pwUniStrDest = pwBuffer;
	}

	return dwBefore == dwAfter;
}

std::wstring DiffUI::GBKToUnicode(const std::string& str) //GBK
{
	std::wstring ret;
	bool res = GbkToUnicode_NoLimit(str.c_str(), &ret);
	assert(res);
	return ret;
}

void DiffUI::updateHotPointBar()
{
	hotPointBar->NotifyCurBlock(textEditL->firstBlockInViewport().blockNumber());
}

void UseBeyondCompare( QString fileL, QString fileR )
{
	QString params = QString(" ") + "\"" + fileL + "\" \"" +fileR + "\"";
	QString appPath = QCoreApplication::applicationDirPath() + QString("/Beyond Compare 4/BCompare.exe");
	qDebug() << appPath;
	ShellExecuteA(NULL, "open", appPath.toLocal8Bit().constData(),params.toLocal8Bit().constData(), NULL, SW_SHOWNORMAL);		
}

