#include "OnlProdDBVar.h"
#include <iostream>
#include <cmath>

using namespace std;

#ifndef NAN
static const float NAN = sqrt( -1.);
#endif

OnlProdDBVar::OnlProdDBVar()
{
  runnumber = -1;
  startvaltime = 0;
  endvaltime = 0;
  for (short i = 0; i < 2; i++)
    {
      m_val[i] = NAN;
    }
  return ;
}

int
OnlProdDBVar::SetVar(const float rval[2])
{
  for (short int i = 0; i < 2 ;i++)
    {
      m_val[i] = rval[i];
    }
  return 0;
}

void
OnlProdDBVar::Print() const
{
  cout << "Value: " << m_val[0] << ", Error: " << m_val[1] << endl;
  return ;
}
