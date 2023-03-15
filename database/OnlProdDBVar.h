#ifndef ONLPRODDBVAR_H__
#define ONLPRODDBVAR_H__

#include <array>
#include <ctime>

class OnlProdDBVar
{
 public:
  OnlProdDBVar();
  virtual ~OnlProdDBVar() {};

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
  int runnumber;
  time_t startvaltime;
  time_t endvaltime;
  std::array<float,2> m_val;

};

#endif
