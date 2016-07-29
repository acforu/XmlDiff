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
	AllocConsole(); 
	freopen("CONOUT$","w+t",stdout); 
	freopen("CONIN$","r+t",stdin); 

	system("mode con:cols=100 lines=1000");  

	QApplication a(argc, argv);
	qInstallMsgHandler(myMessageOutput);

	//if (argc < 3)
	//{
	//	return 0;
	//}





	Profiler profile;

	DiffUI w;
	w.showMaximized();
	XmlDiff diff;

	w.BeginEditBlock();
	if (argc >= 3)
	{
		//std::string file1(argv[1]);
		//std::string file2(argv[2]);
		diff.Diff(argv[1],argv[2],&w);
	}
	else
	{
		//	diff.Diff(".\\test_case\\commodity1\\a.xml",".\\test_case\\commodity1\\b.xml",&w);
		//diff.Diff(".\\test_case\\commodity\\a.xml",".\\test_case\\commodity\\b.xml",&w);
		diff.Diff(".\\test_case\\quest1\\a.xml",".\\test_case\\quest1\\b.xml",&w);
	}
	

	w.EndEditBlock();


	profile.Stop();

	return a.exec();
	//FreeConsole();
}
