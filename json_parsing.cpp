#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

template<typename T>
class tree
{
public:
	tree<T>* previous = nullptr;
	map<T, tree*> branches;
	vector<T> content;
	tree()
	{
	}
	
	void addBranch(T branch)
	{
		branches[branch] = nullptr;
		content.push_back(branch);
	}

	void tieBranch(T branch, tree* &branchPtr)
	{
		branches[branch] = branchPtr;
		branchPtr = nullptr;
	}

	~tree()
	{
		deleteAllBranches();
	}

	void deleteAllBranches()
	{
		for (auto it = branches.begin(); it != branches.end(); it++)
		{
			if (it->second != 0)
			{
				delete it->second;
			}
		}
		content.clear();
		previous = nullptr;
		branches.clear();
	}
};

class json_object
{
public:
	ofstream file;
	int tabCount = 0;
	tree<string>* contents;

	json_object()
	{
		file.open("outp.json");
		contents = new tree<string>();
		initAllBranches();
	}

	json_object(tree<string>* &inpTree)
	{
		file.open("outp.json");
		contents = inpTree;
		inpTree = nullptr;
		initAllBranches();
	}

	void initAllBranches(tree<string>* prev = 0)
	{
		for (auto it = contents->branches.begin(); it != contents->branches.end(); it++)
		{
			if (it->second != 0)
			{
				initAllBranches(contents);
			}
		}
		contents->previous = prev;
	}

	void hopIn(string branch)
	{
		contents = contents->branches[branch];
	}

	void goBack()
	{
		if (contents->previous != 0)
		{
			contents = contents->previous;
		}
	}

	void goToStart()
	{
		while (contents->previous != 0)
		{
			contents = contents->previous;
		}
	}

	void openClass()
	{
		contents->addBranch("{");
		string str = "";
		initTabulation(str);
		str += "{";
		file << str << endl;
		tabCount += 1;
	}

	void closeClass(bool contin = false)
	{
		tabCount -= 1;
		contents->addBranch("}");
		string str = "";
		initTabulation(str);
		str += "}";
		if (contin)
		{
			str += ",";
		}
		file << str << endl;
	}

	void initTabulation(string& inp)
	{
		for (int i = 0; i < tabCount; i++)
		{
			inp += "\t";
		}
	}

	void addProperties(string propName, string prop, bool separator = false)
	{
		string inp;
		initTabulation(inp);

		inp += "\""+ propName+ "\": \""+ prop +"\"";
		if (separator)
		{
			inp += ",";
		}

		contents->addBranch(propName+":"+ prop);

		file << inp << endl;
	}

	void createClass(string className)
	{
		string inp;
		initTabulation(inp);

		inp += "\"" + className + "\": {";
		file << inp << endl;

		contents->addBranch(className + ":");

		tree<string>* tr = new tree<string>();

		contents->tieBranch(className + ":", tr);
		contents->previous = contents;
		hopIn(className + ":");
		tabCount += 1;
	}
	
	void openArray(string arrayName)
	{
		contents->addBranch(arrayName+ ": [");
		string str = "";
		initTabulation(str);
		str += arrayName+ ": [";
		file << str << endl;
		tabCount += 1;
	}

	void closeArray(bool contin = false)
	{
		tabCount -= 1;
		contents->addBranch("]");
		string str = "";
		initTabulation(str);
		str += "]";
		if (contin)
		{
			str += ",";
		}
		file << str << endl;
	}

	void addArrayElement(string arrayElement, bool separator = false)
	{
		string str = "";
		initTabulation(str);
		str += "\"" + arrayElement + "\"";
		if (separator)
		{
			str += ",";
		}
		file << str << endl;
		contents->addBranch(arrayElement);
	}

	~json_object()
	{
		file.close();
		goToStart();
		delete contents;
	}
};


int main()
{
	json_object obj;

	//tabulation in this code sample represents actual tabulation 
	//in the result json file
	obj.openClass();
		obj.addProperties("name","john",true); //true adds "," at the end of line
		obj.createClass("Job");
			obj.addProperties("state", "employed");
		obj.closeClass(true);
		obj.openArray("awards");
			obj.addArrayElement("best employee of the month", true);
			obj.addArrayElement("best employee of the year");
		obj.closeArray();
	obj.closeClass();
}

