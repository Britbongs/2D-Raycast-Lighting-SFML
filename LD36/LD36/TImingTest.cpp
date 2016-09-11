#include "stdafx.h"
#include <unordered_map>
#include <string>
#include <fstream>

#define ELEMENT_TOTAL 100

static const int REPEATS = 100000;

using namespace std;

wstring Keys[ELEMENT_TOTAL];

wstring Values[ELEMENT_TOTAL];

vector<int> UnorderedQueryIndexList;

void FillData();

int main(int argc, char* argv[], char* envp[])
{
	unordered_map<wstring, wstring> WordTable;
	vector<wstring> Words(ELEMENT_TOTAL);

	FillData();

	//setup word table
	for (Int32 i = 0; i < ELEMENT_TOTAL; ++i)
	{
		WordTable.emplace(Keys[i], Values[i]);
	}

	//setup vector
	for (Int32 i = 0; i < ELEMENT_TOTAL; ++i)
	{
		Words[i] = Values[i];
	}

	auto TableTimer = [&]()
	{
		Clock C;
		C.restart();
		wstring S;

		for (Int32 i = 0; i < ELEMENT_TOTAL; ++i)
		{
			wstring QueryString = Keys[UnorderedQueryIndexList[i]];
			S = WordTable[QueryString];
		}

		return C.getElapsedTime();
	};

	auto VectorTimer = [&]()
	{
		Clock C;
		C.restart();
		wstring S;

		for (Int32 i = 0; i < ELEMENT_TOTAL; ++i)
		{
			auto QueryString = Words[UnorderedQueryIndexList[i]];

			for (auto Value : Values)
			{
				if (Value != QueryString)
					continue;
				S = Value;
			}
		}

		return C.getElapsedTime();
	};

	Time AvgTableTime = seconds(0.0), AvgVectorTime = seconds(0.0);

	for (Int32 i = 0; i < REPEATS; ++i)
	{
		AvgTableTime += TableTimer();
		AvgVectorTime += VectorTimer();
	}

	wcout << L"Total Table Time In Milliseconds: " << AvgTableTime.asMilliseconds() << endl;
	wcout << L"Total Time In Milliseconds: " << AvgVectorTime.asMilliseconds() << endl;
	wcout << L"Avg Table Time In Microseconds: " << (float)AvgTableTime.asMicroseconds() / (float)REPEATS << endl;
	wcout << L"Avg Vector Time In Microseconds: " << (float)AvgVectorTime.asMicroseconds() / (float)REPEATS << endl;

	system("pause");

	return EXIT_SUCCESS;
}

void FillData()
{
	InitRand();
	ifstream LoadFile;

	for (Int32 i = 0; i < ELEMENT_TOTAL; ++i)
	{
		Keys[i] = to_wstring(i + 1);
	}

	LoadFile.open("res//Values.txt");
	if (LoadFile.fail())
	{
		wcout << L"Failed to open values.txt";
	}
	else
	{
		Int32 Counter = 0;
		while (!LoadFile.eof() && Counter < ELEMENT_TOTAL)
		{
			string s;
			LoadFile >> s;
			Values[Counter] = String(s).toWideString();
			++Counter;
		}
	}
	LoadFile.close();

	for (Int32 Count = 0; Count < ELEMENT_TOTAL; ++Count)
	{
		Int32 Index = 0;
		bool NewIndex = true;

		do
		{
			if (!NewIndex)
			{
				Index = rand() % ELEMENT_TOTAL;
				NewIndex = true;
			}

			for (auto Num : UnorderedQueryIndexList)
			{
				if (Num != Index)
				{
					continue;
				}
				NewIndex = false;
			}
		} while (!NewIndex);

		UnorderedQueryIndexList.push_back(Index);
	}
	system("pause");
}