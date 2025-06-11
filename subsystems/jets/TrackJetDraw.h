// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#ifndef TRACK_JET_DRAW_H
#define TRACK_JET_DRAW_H

#include "BaseJetDrawer.h"
#include "JetKinematicDrawer.h"
#include "JetSeedDrawer.h"
#include <qahtml/QADrawClient.h>
#include <qahtml/QADraw.h>
#include <jetqa/JetQADefs.h>
#include <map>
#include <memory>
#include <string>

class TFile;

// ============================================================================
//! Draw track jet QA histograms
// ============================================================================
/*! A QAhtml subsystem to draw track jet QA histograms
 *  and generate relevant HTML pages.
 */
class TrackJetDraw : public QADraw
{
  public:

    // ctor/dtor
    TrackJetDraw(const std::string& name = "TrackJetQA",
                 const std::string& type = "track_antikt",
                 const bool debug = false);
    ~TrackJetDraw();

    // setters
    void SetDoDebug(const bool debug) {m_do_debug = debug;}
    void SetJetType(const std::string& type) {m_jet_type = type;}

    // getters 
    bool GetDoDebug() const {return m_do_debug;}
    std::string GetJetType() const {return m_jet_type;}

    // inherited public methods
    int Draw(const std::string& what = "ALL") override;
    int MakeHtml(const std::string& what = "ALL") override;

    // other public methods
    void SaveCanvasesToFile(TFile* file);

  private:

    ///! name of subsystem
    std::string m_name;

    ///! type of jet input, used in histogram names
    std::string m_jet_type;

    ///! turn debugging statements on/off
    bool m_do_debug;

    ///! components to do actual histogram drawing
    std::map<std::string, std::unique_ptr<BaseJetDrawer>> m_drawers;

    ///! triggers we want to draw
    ///!   - FIXME dynamically allocate based on pp vs. pau
    std::vector<uint32_t> m_vecTrigToDraw = {
      JetQADefs::GL1::MBDNSJet1,
      JetQADefs::GL1::MBDNSJet2,
      JetQADefs::GL1::MBDNSJet3,
      JetQADefs::GL1::MBDNSJet4
    };

    ///! resolutions we want to draw
    ///!   - FIXME initialize in ctor
    std::vector<uint32_t> m_vecResToDraw = {
      JetDrawDefs::JetRes::R02,
      JetDrawDefs::JetRes::R03,
      JetDrawDefs::JetRes::R04,
      JetDrawDefs::JetRes::R05
    };

};  // end TrackJetDraw

#endif
