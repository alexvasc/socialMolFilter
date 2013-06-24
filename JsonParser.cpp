#include "JsonParser.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

Json::Json(string file)
{
  ifstream jsonFile(file.c_str());
  string tmp;
  getline(jsonFile, tmp);
  getline(jsonFile, tmp);

  bool done = false;
  while (!done)
  {
    getline(jsonFile,tmp);
    if (!tmp.compare("        {"))
    {
      //this is a new list element
      getline(jsonFile,tmp);
      int source = atoi(tmp.substr(22,tmp.size()-23).c_str());
      getline(jsonFile,tmp);
      int target = atoi(tmp.substr(22,tmp.size()-23).c_str());
      getline(jsonFile,tmp);
      int val = atoi(tmp.substr(19,1).c_str());
      LinkList.push_back(new Link(source, target, val));
      getline(jsonFile,tmp);
    }
    else
    {
      done = true; // we are done looking at the links
    }
  }
  
  
  getline(jsonFile,tmp);
  //Now we get the nodes....
  done = false;
  while (!done)
  {
    getline(jsonFile,tmp);
    if (!tmp.compare("        {"))
    {
      //this is a new list element
      getline(jsonFile,tmp);
      int group = atoi(tmp.substr(21,tmp.size()-22).c_str());
      getline(jsonFile,tmp);
      int id = atoi(tmp.substr(18,tmp.size()-19).c_str());
      getline(jsonFile,tmp);
      string name = tmp.substr(21,tmp.size()-28); //ignore the last seven characters: they are always '\t\n", '
      getline(jsonFile,tmp);
      string pic = tmp.substr(20,tmp.size()-23); //ignore the last three characters: they are always '", '
      getline(jsonFile,tmp);
      string url = tmp.substr(20,tmp.size()-21); //ignore only the last character: it is always a '"'
      bool isReaction;
      if (group == 1)
	isReaction = false;
      else if (group == 2) isReaction = true;

      NodeList.push_back(new Node(group,id,name,pic,url,isReaction));
      getline(jsonFile,tmp);
    }
    else
      done = true; // we are done looking at the links

    
  }
};

void Json::rebalance()
{
  int mymap[1024];
  for (int i=0;i<1024;i++)
    mymap[i]=0;

  int counter = 0;
  for (list<Node*>::iterator it = NodeList.begin(); it!=NodeList.end();it++)
  {
    mymap[(*it)->id] = counter;
    (*it)->id = counter;
    counter++;
  }

  for (list<Link*>::iterator it = LinkList.begin(); it!=LinkList.end();it++)
  {
    (*it)->source = mymap[(*it)->source];
    (*it)->target = mymap[(*it)->target];
  }
  
};

void Json::toFile(ofstream* fout)
{
  //  ofstream fout(fname.c_str());
  *fout << "{" << endl << "    \"links\": [" << endl;

  for (list<Link*>::iterator it = LinkList.begin(); it!= LinkList.end(); it++)
  {
    *fout << "        {" << endl;
    *fout << "            \"source\": " << (*it)->source << "," << endl;
    *fout << "            \"target\": " << (*it)->target << "," << endl;
    *fout << "            \"val\": " << (*it)->val << endl;
    *fout << "        }";
    if (next(it)!=LinkList.end())
      *fout << "," << endl;
    else *fout << endl;
  }

  *fout << "    ]," << endl;
  *fout << "    \"nodes\": [" << endl;

  for (list<Node*>::iterator it = NodeList.begin(); it!= NodeList.end(); it++)
  {
    *fout << "        {" << endl;
    *fout << "            \"group\": " << (*it)->group << "," << endl;
    *fout << "            \"id\": " << (*it)->id << "," << endl;
    if ((*it)->isReaction)
      *fout << "            \"name\": \"" << (*it)->name << endl;
    else *fout << "            \"name\": \"" << (*it)->name << "\\t\\n\"," << endl;
    *fout << "            \"pic\": \"" << (*it)->pic << "\"," << endl;
    *fout << "            \"url\": \"" << (*it)->url << "\"" << endl;
    *fout << "        }";
    if (next(it)!=NodeList.end())
      *fout << "," << endl;
    else *fout << endl;
  }
  
  *fout << "    ]" << endl;
  *fout << "}" << endl;

};

