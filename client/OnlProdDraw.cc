#include "OnlProdDraw.h"

#include <iostream>

using namespace std;

OnlProdDraw::OnlProdDraw(const string &name): Fun4AllBase(name)
{
  return;
}

int 
OnlProdDraw::Draw(const string &/*what*/)
{
  cout << "Draw not implemented by daughter class" << endl;
  return -1;
}

int 
OnlProdDraw::MakeHtml(const string &/*what*/)
{
  cout << "MakeHtml not implemented by daughter class" << endl;
  return -1;
}

