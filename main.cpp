#include "stdafx.h"
#include "diffui.h"
#include <QtGui/QApplication>
#include "XmlDiff.h"
#include "windows.h"
#include <QtDebug>
#include <QtGlobal>

void myMessageOutput(QtMsgType, const char * msg)
{
	cout << msg << endl;
}

int main(int argc, char *argv[])
{
	AllocConsole(); 
	freopen("CONOUT$","w+t",stdout); 
	freopen("CONIN$","r+t",stdin); 

	//system("mode con:cols=100 lines=1000");  

	QApplication a(argc, argv);
	//qInstallMsgHandler(myMessageOutput);

	//if (argc < 3)
	//{
	//	return 0;
	//}

	//std::string file1(argv[1]);
	//std::string file2(argv[2]);

	DiffUI w;
	w.showMaximized();

	LARGE_INTEGER frequency;        // ticks per second
	LARGE_INTEGER t1, t2;           // ticks
	double elapsedTime;

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&t1);

	XmlDiff diff;
	w.BeginEditBlock();
//	diff.Diff(".\\test_case\\commodity1\\a.xml",".\\test_case\\commodity1\\b.xml",&w);
	diff.Diff(".\\test_case\\commodity\\a.xml",".\\test_case\\commodity\\b.xml",&w);

	w.EndEditBlock();


	QueryPerformanceCounter(&t2);
	elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart / 1000;


	cout << "cost time: " << elapsedTime << endl;


	return a.exec();
	//FreeConsole();
}
