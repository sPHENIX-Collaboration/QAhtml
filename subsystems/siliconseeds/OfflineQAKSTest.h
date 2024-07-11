// Author: Tanner Mengel
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

   TH1D *GenKSTestSummary(const char * option = "");
   void AddHistogramNames(const std::set<std::string> & names);
   double GetHistoScore(const std::string &name, const char * option = "");

    void Verbosity(unsigned int v) { m_verbosity = v; }
    unsigned int Verbosity() const { return m_verbosity; }

 private:

   std::string m_inputfile{""};
   std::string m_goodrunfile{""};

   
   unsigned int m_verbosity{0};

   TFile * m_inputfileptr{nullptr};
   TFile * m_goodrunfileptr{nullptr};

   std::set<std::string> m_histogram_names{};

   template <typename T>
   double KSTest(T *h1, T *h2, const char * options = "");
  
};



#endif // OFFLINEQAKSTEST_H