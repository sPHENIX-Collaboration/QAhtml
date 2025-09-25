// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#ifndef EXAMPLE_DRAWER_H
#define EXAMPLE_DRAWER_H

#include "BaseJetDrawer.h"

// ============================================================================
//! Example Drawer
// ============================================================================
/*! This class serves as an example of how to write
 *  a "Drawer" (or "Component") class for a given
 *  Jet QA module. Note that each jet QA module in
 *  coresoftware should have a corresponding Drawer
 *  here!
 *
 *  The header file should look identical to this
 *  except for the name of the class & corresponding
 *  strings. The implementation file is where you'll
 *  make changes to actually implement the drawing
 *  logic/what to draw.
 */
class ExampleDrawer : public BaseJetDrawer
{
  public:

    // ctor/dtor
    ExampleDrawer(const std::string& name = "Example", //!< the name of drawer
                  const std::string& modu = "JetDraw", //!< what QAhtml module is calling it
                  const std::string& type = "towersub1_antikt", //!< the type of jet being drawn
                  const std::string& pref = "h_example", //!< prefix of histograms from QA module
                  const bool debug = false //!< turn debugging messages on/off
                  );
    ~ExampleDrawer();

    // inherited public methods
    // - NOTE: these lines should NOT be changed!
    int Draw(const std::vector<uint32_t> vecTrigToDraw,
             const std::vector<uint32_t> vecResToDraw) override;
    int MakeHtml(const std::vector<uint32_t> vecTrigToDraw,
                 const std::vector<uint32_t> vecResToDraw,
                 const QADraw& subsystem) override;

  private:

    // inherited private methods
    // - NOTE: likewise! Do NOT change this!
    void DoDrawing(const uint32_t trig, const uint32_t res) override;

};  // end ExampleDrawer

#endif
