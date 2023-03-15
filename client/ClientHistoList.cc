#include "ClientHistoList.h"

#include <TNamed.h>

#include <string>

using namespace std;

ClientHistoList::ClientHistoList()
{
  histo = NULL;
  subsystem = "UNKNOWN";
}

ClientHistoList::ClientHistoList(const string &subsys)
{
  histo = NULL;
  subsystem = subsys;
}

TNamed *ClientHistoList::Histo() const
{
  return histo;
}

void ClientHistoList::Histo(TNamed *Histo)
{
  histo = Histo;
  return ;
}

string ClientHistoList::SubSystem() const
{
  return subsystem;
}

void ClientHistoList::SubSystem(const string &SubSystem)
{
  subsystem = SubSystem;
  return ;
}

void ClientHistoList::identify(ostream& os) const
{
  os << "id" << endl;
  return;
}
