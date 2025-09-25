#ifndef KFP_KFPARTICLEDRAW_H__
#define KFP_KFPARTICLEDRAW_H__

#include <qahtml/QADraw.h>

class QADB;
class QADBVar;
class TCanvas;
class TPad;
class TH1F;
class TH2F;

class KFParticleDraw : public QADraw
{
 public: 
  KFParticleDraw(const std::string &name = "QAKFParticle_SvtxTrackMap");
  ~KFParticleDraw() override;

  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;
  int DBVarInit();

 private:
  int MakeCanvas(const std::string &name, int num);
  int DrawKinematicGeometricInfo();
  int DrawTriggerInfo();
  TCanvas *TC[2]{};
  TPad *transparent[2]{};
  TPad *Pad[2][6]{};
  const char *histprefix;
};

#endif
