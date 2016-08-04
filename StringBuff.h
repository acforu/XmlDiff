#pragma once

class StringBuff
{
public:
	void AppendChar(char c);
	void AppendStr(const char* str);
	void AppendStr(const char* beg, const char* end);
	void FillChars(char c, int count);
	void Clear();
	void AppendNull();
	int Total(char c);

	const char* Data()
	{
		return &buff[0];
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

friend class StringBuffIterator;

private:
	std::vector<char> buff;
};


