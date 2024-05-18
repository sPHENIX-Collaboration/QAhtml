#include "QADraw.h"

#include <iostream>

using namespace std;

QADraw::QADraw(const string &name): Fun4AllBase(name)
{
  return;
}

int 
QADraw::Draw(const string &/*what*/)
{
  cout << "Draw not implemented by daughter class" << endl;
  return -1;
}

int 
QADraw::MakeHtml(const string &/*what*/)
{
  cout << "MakeHtml not implemented by daughter class" << endl;
  return -1;
}

