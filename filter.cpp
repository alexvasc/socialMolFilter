#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <unistd.h>
#include "JsonParser.h"
#include <math.h>

using namespace std;


double dist(string smile1, string smile2)
{
  int s1[5];
  int s2[5]; //stores number of C, N, O, P, S in this order

  for (int i=0;i<5;i++)
  {
    s1[i]=0;
    s2[i]=0;
  }

  for (string::iterator it = smile1.begin(); it !=smile1.end();it++)
    switch (*it)
    {
    case 'C': s1[0]++; break;
    case 'N': s1[1]++; break;
    case 'O': s1[2]++; break;
    case 'P': s1[3]++; break;
    case 'S': s1[4]++; break;
    }
  
  for (string::iterator it = smile2.begin(); it !=smile2.end();it++)
    switch (*it)
    {
    case 'C': s2[0]++; break;
    case 'N': s2[1]++; break;
    case 'O': s2[2]++; break;
    case 'P': s2[3]++; break;
    case 'S': s2[4]++; break;
    }
  
  double dist = 0;
  for (int i=0;i<5;i++)
    dist += (s1[i]-s2[i]) * (s1[i]-s2[i]);
  return sqrt(dist);
}

//outputs a new Json corresponding to the filtered old one
Json* filter(string smile1, Json* curJson, double thresh)
{
  Json* filtered;
  filtered = new Json();
  for (list<Node*>::iterator it=curJson->NodeList.begin(); it!= curJson->NodeList.end();it++)
  {
    if (dist(smile1,(*it)->name) < thresh)
      filtered->NodeList.push_back(new Node(*it));
    if ((*it)->isReaction)
      filtered->NodeList.push_back(new Node(*it));
  }

  //Now we have all the right nodes, so we have to get rid of the links that don't make sense anymore...
  for (list<Link*>::iterator it=curJson->LinkList.begin(); it!= curJson->LinkList.end();it++)
  {
    cout << "Current list source: " << (*it)->source << endl;
    cout << "Current list target: " << (*it)->target << endl;
    bool sourceIsGood = false; //whether the source of the link is a node in the new NodeList
    bool targetIsGood = false; //whether the target of the link is a node in the new NodeList
    for (list<Node*>::iterator newit=filtered->NodeList.begin(); newit!= filtered->NodeList.end();newit++)
    {
      if ((*it)->source == 1 && (*it)->target == 3)
	  cout << "   comparing to node with id " << (*newit)->id << endl;
      if ((*it)->source == (*newit)->id)
        {sourceIsGood = true;cout << "good source" << endl;}
      if ((*it)->target == (*newit)->id)
	{targetIsGood = true; cout << "good target" << endl;}
    }
    if (sourceIsGood && targetIsGood) 
    {
      filtered->LinkList.push_back(new Link(*it));
      cout << " ... found!" << endl;
    }
    else
      cout << "..... not found!" << endl;
  }
  filtered->rebalance();
  return filtered;
}

//returns the terminal output induced by terinal command "command"
string getTerminalOutput(string command)
{
  FILE *in;
  char buff[512];

  if(!(in = popen(command.c_str(), "r")))
  {
    cout << "Error calling " << command << endl;
  }

  stringstream ans;
  while(fgets(buff, sizeof(buff), in) != NULL){
    ans << buff;
  }
  pclose(in);

  return ans.str();
}


int main(int argc, char* argv[])
{
  double dist;
  if (argc < 2)
  {
    cout << "setting distance to 1" << endl;
    dist = 1;
  }
  else dist = atoi(argv[1]);

  Json test("../Molecules/AB/LZXFCXXLZCGV-UHFFFAOYSA-N.json.bak");
  string smile1 = "OP(=O)O"; //corresponds to the json file above


  Json* filtered = filter(smile1, &test, dist);
  int counter =0;
  for (list<Link*>::iterator it = filtered->LinkList.begin(); it!=filtered->LinkList.end();it++)
  {
    counter++;
    cout << (*it)->toString() << endl;
  }
  cout << "I count a total of " << counter << " links  in the FILTERED Json file  ../Molecules/AB/LZXFCXXLZCGV-UHFFFAOYSA-N.json" << endl << endl;
  
  counter=0;
  for (list<Node*>::iterator it = filtered->NodeList.begin(); it!=filtered->NodeList.end();it++)
  {
    counter++;
    cout << (*it)->toString() << endl;
  }
  cout << "I count a total of " << counter << " nodes in the FITLERED Json file  ../Molecules/AB/LZXFCXXLZCGV-UHFFFAOYSA-N.json" << endl << endl;

  ofstream outputFile("../Molecules/AB/LZXFCXXLZCGV-UHFFFAOYSA-N.json");
  filtered->toFile(&outputFile);
  outputFile.close();

  /*  list<string> directoryNames;
  stringstream terminalOut(getTerminalOutput("ls"));
  while (terminalOut.good())
  {
    string tmp;
    terminalOut >> tmp;
    if (tmp.find(".") == string::npos) //anything without a '.' (extension) is a directory
      directoryNames.push_back(tmp);
  }
  directoryNames.pop_back(); //last one is always a blank space?!

  cout << "There are " << directoryNames.size() << " directories" << endl;


  list<string> smilesFiles;
  //for each directory, go into the directory and read the smiles string...
  for (list<string>::iterator it = directoryNames.begin(); it != directoryNames.end(); it++)
  {
    chdir((*it).c_str());
    stringstream ss(getTerminalOutput("ls"));
    while (ss.good())
    {
      string tmp;
      ss >> tmp;
      if (tmp.find(".") == string::npos) // we are looking forhte files without a '.'
	smilesFiles.push_back(tmp);
    }
    smilesFiles.pop_back();
    chdir("../");
  }

  for (list<string>::iterator it = smilesFiles.begin(); it != smilesFiles.end(); it++)
    cout << *it << endl;
  */
}
