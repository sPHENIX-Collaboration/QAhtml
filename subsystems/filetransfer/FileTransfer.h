#ifndef FILETRANSFER_H__
#define FILETRANSFER_H__

#include <qahtml/QADraw.h>
#include <memory>
#include <vector>
#include <iostream>

#include "FileTransferUtils.h"

class QADrawDB;
class QADrawDBVar;
class TCanvas;
class TGraphErrors;
class TPad;
class TH1;

class FileTransfer : public QADraw
{
 public:
  FileTransfer(const std::string &name = "FileTransferQA");
  ~FileTransfer() override;

  int Draw(const std::string &what = "ALL") override;
  int MakeHtml(const std::string &what = "ALL") override;

 protected:
  int MakeCanvas(const std::string &name);
  int DrawTransfer(const std::string &what = "ALL");
  void CheckFileTransfer();
    TH1* create_subsytems_histogram( const std::string& name, const std::string& title, const subsystem_info_t::list& subsystems) ; 

  TCanvas *TC[1]{};
  TPad *transparent[1]{};
  TPad *Pad[4]{};
  int verbosity = 0;
  std::unique_ptr<TH1> m_h_expected, m_h_ref, m_h_transfered, m_h_transfered_first_segment;
};

#endif
