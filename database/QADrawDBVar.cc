#include "QADrawDBVar.h"

#include <cmath>
#include <iostream>
#include <limits>

QADrawDBVar::QADrawDBVar()
{
  m_val.fill(std::numeric_limits<float>::quiet_NaN());
  return;
}

int QADrawDBVar::SetVar(const float rval[2])
{
  for (short int i = 0; i < 2; i++)
  {
    m_val[i] = rval[i];
  }
  return 0;
}

void QADrawDBVar::Print() const
{
  std::cout << "Value: " << m_val[0] << ", Error: " << m_val[1] << std::endl;
  return;
}
