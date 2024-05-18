#ifndef SCALERS_H
#define SCALERS_H

class Scalers
{
 public:
  Scalers();
  Scalers(unsigned long long ir, unsigned long long il, unsigned long long is);
  virtual ~Scalers() {}

  unsigned long long ScalerRaw() const { return scalerraw; }
  unsigned long long ScalerLive() const { return scalerlive; }
  unsigned long long ScalerScaled() const { return scalerscaled; }
  void ScalerRaw(const unsigned long long i) { scalerraw = i; }
  void ScalerLive(const unsigned long long i) { scalerlive = i; }
  void ScalerScaled(const unsigned long long i) { scalerscaled = i; }

 protected:
  unsigned long long scalerraw;
  unsigned long long scalerlive;
  unsigned long long scalerscaled;
};

#endif
