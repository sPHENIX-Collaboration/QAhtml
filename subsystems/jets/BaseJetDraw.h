// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#ifndef BASE_JET_DRAW_H
#define BASE_JET_DRAW_H

#include "BaseJetDrawer.h"
#include <qahtml/QADraw.h>
#include <map>
#include <memory>
#include <string>

// ============================================================================
//! Base class for jet QA
// ============================================================================
/*! Base class to define common functionality
 *  across the QAhtml jet subsystems (e.g.
 *  CaloJetDraw and TrackJetDraw). Each type
 *  of jet to QA should have a corresponding
 *  "draw" (NOT a "drawer") class inheriting
 *  from this one.
 */
class BaseJetDraw : public QADraw
{
  public:

    // ctor/dtor
    BaseJetDraw(const std::string& name = "JetQA",
                const std::string& type = "towersub1_antikt",
                const bool debug = false,
                const bool local = false);
    ~BaseJetDraw();

    // getters 
    bool GetDoDebug() const {return m_do_debug;}
    bool GetDoLocal() const {return m_do_local;}
    std::string GetJetType() const {return m_jet_type;}
    std::string GetName() const {return m_name;}

    // inherited public methods
    int Draw(const std::string& what = "ALL") override;
    int MakeHtml(const std::string& what = "ALL") override;

    // other public methods
    void SetDoDebug(const bool debug);
    void SetDoLocal(const bool local);
    void SetJetType(const std::string& type);
    void SaveCanvasesToFile(TFile* file);

  protected:

    // inherited private methods
    void Initialize();

    ///! name of subsystem
    std::string m_name;

    ///! type of jet input, used in histogram names
    std::string m_jet_type;

    ///! turn debugging statements on/off
    bool m_do_debug;

    ///! turn on/off local drawing mode
    bool m_do_local;

    ///! sets p+p (true) vs. Au+Au mode (false)
    bool m_is_pp;

    ///! components to do actual histogram drawing
    std::map<std::string, std::unique_ptr<BaseJetDrawer>> m_drawers;

    ///! triggers we want to draw
    std::vector<uint32_t> m_vecTrigToDraw;

    ///! resolutions we want to draw
    std::vector<uint32_t> m_vecResToDraw;

};  // end BaseJetDraw

#endif
