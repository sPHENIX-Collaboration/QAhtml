#ifndef QA_CLIENT_CLIENTHISTOLIST_H
#define QA_CLIENT_CLIENTHISTOLIST_H

#include <iostream>
#include <string>

class TNamed;

class ClientHistoList
{
 public:
  ClientHistoList() = default;
  ClientHistoList(const std::string &subsys);

  virtual ~ClientHistoList() {}

  TNamed *Histo() const;
  void Histo(TNamed *Histo);
  std::string SubSystem() const;
  void SubSystem(const std::string &SubSystem);
  void identify(std::ostream &os = std::cout) const;

 protected:
  TNamed *histo{nullptr};
  std::string subsystem{"UNKNOWN"};
};

#endif /* QA_CLIENT_CLIENTHISTOLIST_H */
