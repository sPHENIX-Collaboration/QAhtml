#include "Scalers.h"

Scalers::Scalers()
  : scalerraw(0)
  , scalerlive(0)
  , scalerscaled(0)
{
}

Scalers::Scalers(unsigned long long ir, unsigned long long il, unsigned long long is)
  : scalerraw(ir)
  , scalerlive(il)
  , scalerscaled(is)
{
}
