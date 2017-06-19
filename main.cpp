#include "stdafx.h"
#include "diffui.h"
#include <QtGui/QApplication>
#include "XmlDiff.h"
#include <QtDebug>
#include <QtGlobal>
#include "XmlHelper.h"


void myMessageOutput(QtMsgType, const char * msg)
{
	cout << msg << endl;
	//OutputDebugStringA(msg);
}

int main(int argc, char *argv[])
{
#if ENABLE_CONSOLE 
	AllocConsole(); 
	freopen("CONOUT$","w+t",stdout); 
	freopen("CONIN$","r+t",stdin); 

	system("mode con:cols=100 lines=1000");  
#endif

	QApplication a(argc, argv);

#if ENABLE_CONSOLE 
	qInstallMsgHandler(myMessageOutput);
#endif

	//if (argc < 3)
	//{
	//	return 0;
	//}


	//qDebug() << "StringDistSift4" << StringDistSift4("ABCDEFG","ABCD1E1F1") <<endl;

#ifndef RETAIL
	Profiler profile;
#endif // RETAIL


	DiffUI w;

	if (argc >= 3)
	{
		QString fileL = QString::fromLocal8Bit(argv[1]);
		QString fileR = QString::fromLocal8Bit(argv[2]);

		bool isXml = true;
		if(QFileInfo(fileL).suffix().toLower() != QString("xml") ||
			QFileInfo(fileR).suffix().toLower() != QString("xml") )
		{
			isXml = false;
		}

		if (!isXml)
		{
			UseBeyondCompare(fileL,fileR);
			return 0;
		}

		w.showMaximized();
		bool ret = w.Diff(argv[1],argv[2]);
		if(!ret)
		{
			UseBeyondCompare(fileL,fileR);
			return 0;
		}
	}
	else
	{
#ifdef RETAIL
		return 0;
#else
		w.showMaximized();

		//QString casename = "food";
		//QString casename = "single_value_node";
		QString casename = "commodity";
		//QString casename = "firstbuy";

		QString paramL = QString(".\\test_case\\%1\\a.xml").arg(casename);
		QString paramR = QString(".\\test_case\\%1\\b.xml").arg(casename);

		w.Diff(paramL.toLocal8Bit().constData(),paramR.toLocal8Bit().constData());
		//w.Diff(".\\test_case\\commodity1\\a.xml",".\\test_case\\commodity1\\b.xml");
		//w.Diff(".\\test_case\\commodity\\a.xml",".\\test_case\\commodity\\b.xml");
		//w.Diff(".\\test_case\\firstbuy\\a.xml",".\\test_case\\firstbuy\\b.xml");
		//w.Diff(".\\test_case\\quest1\\a.xml",".\\test_case\\quest1\\b.xml");
		//w.Diff(".\\test_case\\meili\\a.xml",".\\test_case\\meili\\b.xml");
#endif
	}
	


#ifndef RETAIL
	profile.Stop();
#endif

	return a.exec();
	//FreeConsole();
}
