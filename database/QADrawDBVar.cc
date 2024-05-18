#include "QADrawDBVar.h"
#include <iostream>
#include <cmath>

using namespace std;

#ifndef NAN
static const float NAN = sqrt( -1.);
#endif

QADrawDBVar::QADrawDBVar()
{
  m_val.fill(NAN);
  return ;
}

int
QADrawDBVar::SetVar(const float rval[2])
{
  for (short int i = 0; i < 2 ;i++)
    {
      m_val[i] = rval[i];
    }
  return 0;
}

void
QADrawDBVar::Print() const
{
  cout << "Value: " << m_val[0] << ", Error: " << m_val[1] << endl;
  return ;
}
