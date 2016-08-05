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
	qInstallMsgHandler(myMessageOutput);

	//if (argc < 3)
	//{
	//	return 0;
	//}


	//qDebug() << "StringDistSift4" << StringDistSift4("ABCDEFG","ABCD1E1F1") <<endl;


	Profiler profile;

	DiffUI w;

	if (argc >= 3)
	{
		QString fileL = QString::fromLocal8Bit(argv[1]);
		QString fileR = QString::fromLocal8Bit(argv[2]);

		bool isXml = true;
		if(QFileInfo(fileL).suffix() != QString("xml") ||
			QFileInfo(fileR).suffix() != QString("xml") )
		{
			isXml = false;
		}

		if (!isXml)
		{
			UseBeyondCompare(fileL,fileR);
			return 0;
		}

		bool ret = w.Diff(argv[1],argv[2]);
		if(!ret)
		{
			UseBeyondCompare(fileL,fileR);
			return 0;
		}
		else
		{
			w.showMaximized();
		}
	}
	else
	{
		//w.Diff(".\\test_case\\commodity1\\a.xml",".\\test_case\\commodity1\\b.xml");
		w.showMaximized();
		w.Diff(".\\test_case\\commodity\\a.xml",".\\test_case\\commodity\\b.xml");
		//w.Diff(".\\test_case\\firstbuy\\a.xml",".\\test_case\\firstbuy\\b.xml");
	}
	



	profile.Stop();

	return a.exec();
	//FreeConsole();
}
