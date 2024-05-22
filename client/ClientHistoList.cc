#include "ClientHistoList.h"

#include <TNamed.h>

#include <string>

ClientHistoList::ClientHistoList(const std::string &subsys)
  : subsystem(subsys)
{
}

TNamed *ClientHistoList::Histo() const
{
  return histo;
}

void ClientHistoList::Histo(TNamed *Histo)
{
  histo = Histo;
  return;
}

std::string ClientHistoList::SubSystem() const
{
  return subsystem;
}

void ClientHistoList::SubSystem(const std::string &SubSystem)
{
  subsystem = SubSystem;
  return;
}

void ClientHistoList::identify(std::ostream &os) const
{
  os << "id" << std::endl;
  return;
}
