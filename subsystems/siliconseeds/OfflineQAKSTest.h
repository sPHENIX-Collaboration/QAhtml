#ifndef OFFLINEQAKSTEST_H
#define OFFLINEQAKSTEST_H

#include <string>
#include <set>

class TH1D;
class TFile;

class OfflineQAKSTest
{

 public:

   OfflineQAKSTest(const std::string & inputfile, const std::string & goodrunfile = "");
   ~OfflineQAKSTest();

   TH1D * GenKSTestSummary();
   void AddHistogramNames(const std::set<std::string> & names);

 private:

   std::string m_inputfile{""};
   std::string m_goodrunfile{""};

   TFile * m_inputfileptr{nullptr};
   TFile * m_goodrunfileptr{nullptr};

   std::set<std::string> m_histogram_names{};

   template <typename T>
   double KSTest(T *h1, T *h2, const char * options = "N");
  
};



#endif // OFFLINEQAKSTEST_H