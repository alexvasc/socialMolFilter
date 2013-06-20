#include <fstream>
#include <string>
#include <sstream>
#include <list>

using namespace std;

class Link
{
 public:
  int source, target, val;

  Link(int s, int t, int v)
  {
    source = s;
    target = t;
    val = v;
  }

  Link(Link* o)
  {
    source = o->source;
    target = o->target;
    val = o->val;
  }

  string toString()
  {
    stringstream ss;
    ss << "Source: " << source << endl << "Target: " << target << endl;
    return ss.str();
  }

};

class Node
{
 public:
  int group, id;
  string name, pic, url;
  bool isReaction;

  Node(int g, int i, string n, string p, string u, bool r)
  {
    group = g;
    id = i;
    name = n;
    pic = p;
    url = u;
    isReaction = r;
  }

  Node(Node* o)
  {
    group = o->group;
    id = o->id;
    name = o->name;
    pic = o->pic;
    url = o->url;
    isReaction = o->isReaction;
  }

  string toString()
  {
    stringstream ss;
    ss << "Group: " << group << endl << "ID: " << id << endl << "Name: " << name << endl 
       << "Pic: " << pic << endl << "Url: " << url << endl;
    return ss.str();
  }

};


class Json
{
 public:
  list<Link*> LinkList;
  list<Node*> NodeList;

  Json(){}

  Json(string file);
  void rebalance();
  void toFile(ofstream* fout);
};
