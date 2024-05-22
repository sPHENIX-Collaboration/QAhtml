#include "QADraw.h"

#include <iostream>

QADraw::QADraw(const std::string &name)
  : Fun4AllBase(name)
{
  return;
}

int QADraw::Draw(const std::string & /*what*/)
{
  std::cout << "Draw not implemented by daughter class" << std::endl;
  return -1;
}

int QADraw::MakeHtml(const std::string & /*what*/)
{
  std::cout << "MakeHtml not implemented by daughter class" << std::endl;
  return -1;
}
