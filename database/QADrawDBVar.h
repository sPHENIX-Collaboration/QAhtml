#ifndef QA_DATABASE_QADRAWDBVAR_H
#define QA_DATABASE_QADRAWDBVAR_H

#include <array>
#include <ctime>

class QADrawDBVar
{
 public:
  QADrawDBVar();
  virtual ~QADrawDBVar() {};

  int SetVar(const float rval[2]);
  void Print() const;
  float GetVar(const int i) const {return m_val[i];}
  void Value(const float f) {m_val[0] = f;}
  float Value() const {return m_val[0];}
  void Error(const float f) {m_val[1] = f;}
  float Error() const {return m_val[1];}
  void Startvaltime(const time_t val) {startvaltime = val;}
  time_t Startvaltime() const {return startvaltime;}
  void Endvaltime(const time_t val) {endvaltime = val;}
  time_t Endvaltime() const {return endvaltime;}
  void RunNumber(const int i) {runnumber = i;}
  int RunNumber() const {return runnumber;}

 protected:
  int runnumber {-1};
  time_t startvaltime {0};
  time_t endvaltime{0};
  std::array<float,2> m_val;

};

#endif
