#include "stdafx.h"
#include "StringBuff.h"
#include "Common.h"


using namespace std;
void StringBuff::AppendChar( char c )
{
	buff.push_back(c);
}

//void StringBuff::AppendStr( const char* str )
//{
//	while(*str != 0)
//	{
//		buff.push_back(*str++);
//	}
//}

void StringBuff::AppendStr( const char* beg, const char* end )
{
	buff.insert(buff.end(),beg,end);
}

void StringBuff::AppendStr( const XmlString& str )
{
	buff.insert(buff.end(),str.beg(),str.end());
}

void StringBuff::FillChars( char c, int count )
{
	buff.assign(count,c);
}

void StringBuff::Clear()
{
	if (buff.size())
	{
		buff[0] = 0;
	}
	buff.clear();
}

void StringBuff::AppendNull()
{
	buff.push_back(0);
}

int StringBuff::Total( char c )
{
	return count(buff.begin(),buff.end(),c);
}

StringBuff::StringBuff()
{
	Clear();
}

void StringBuff::Indent( int indent )
{
	FillChars('\t',indent);
}

void StringBuff::Enter()
{
	AppendChar('\n');
}

int StringBuff::ContentLength()
{
	size_t i = 0;
	for ( ;i < buff.size() && buff[i]!=0; ++i)
	{
		;
	}
	return i;
}

