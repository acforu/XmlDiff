#pragma once

struct XmlString;
class StringBuff
{
public:
	StringBuff();
	void AppendChar(char c);
	void AppendStr( const XmlString& str );
	void AppendStr(const char* beg, const char* end);
	void FillChars(char c, int count);
	void Clear();
	void AppendNull();
	void Indent(int indent);
	void Enter();
	int Total(char c);
	int ContentLength();

	const char* Data()
	{
		return &buff[0];
	}

	size_t Length()
	{
		return buff.size();
	}

	void Reserve(size_t size)
	{
		buff.reserve(size);
	}

	class Iterator
	{
	public:
		Iterator(StringBuff* str, int pos = 0):str(str),pos(pos)
		{

		}

		char& operator*()
		{
			if (pos >= str->buff.size())
			{
				str->buff.resize(pos+1);
			}
			return str->buff[pos];
		}

		Iterator& operator++()
		{
			++pos;
			return *this;
		}

		char* operator->(){return &(operator*());}

		Iterator operator++(int)
		{
			Iterator copy(*this);
			++pos;
			return copy;
		}

	private:
		StringBuff* str;
		int pos;
	};

	Iterator Begin()
	{
		return Iterator(this);
	}

	const std::vector<char>& Buff()
	{
		return buff;
	}

friend class StringBuffIterator;

private:
	std::vector<char> buff;
};


