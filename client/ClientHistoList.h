#ifndef __CLIENTHISTOLIST_H__
#define __CLIENTHISTOLIST_H__

#include <iostream>
#include <string>

class TNamed;

class ClientHistoList
{
 public:
  ClientHistoList();
  ClientHistoList(const std::string &subsys);

  virtual ~ClientHistoList() {}

  TNamed *Histo() const;
  void Histo(TNamed *Histo);
  std::string SubSystem() const;
  void SubSystem(const std::string &SubSystem);
  void identify(std::ostream& os = std::cout) const;

 protected:
  TNamed* histo;
  std::string subsystem;

};

#endif /* __CLIENTHISTOLIST_H__ */

