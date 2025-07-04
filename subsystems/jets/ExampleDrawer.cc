// Jennifer James <jennifer.l.james@vanderbilt.edu>, McKenna Sleeth, Derek Anderson, Mariia Mitrankova

#include "ExampleDrawer.h"

#include <qahtml/QADrawClient.h>

// ctor/dtor ==================================================================

// ----------------------------------------------------------------------------
//! Default ctor
// ----------------------------------------------------------------------------
/*! The drawer's constructor. You should NOT need to
 *  change this.
 */
ExampleDrawer::ExampleDrawer(const std::string& name,
                             const std::string& modu,
                             const std::string& type,
                             const std::string& pref,
                             const bool debug)
  : BaseJetDrawer(name, modu, type, pref, debug) {};

// ----------------------------------------------------------------------------
//! Default dtor
// ----------------------------------------------------------------------------
/*! The drawer's destructor. You should NOT need to
 *  change this.
 */
ExampleDrawer::~ExampleDrawer() {};

// inherited public methods ===================================================

// ----------------------------------------------------------------------------
//! Run histogram drawing for each trigger and resolution
// ----------------------------------------------------------------------------
/*! This method defines the drawing logic. For most cases -- i.e.
 *  you're histogramming some quantity to be filled for each jet
 *  (and thus for each resolution parameter and each trigger) --
 *  you do NOT need to change this.
 *
 *  Below, we loop over each resolution parameter to analyze
 *  and each trigger to analyze. For every combination, we then
 *  run `DoDrawing`, which does the actual plotting. Note the
 *  `m_plots.AddRow()` and `m_plots.AddColumn()`. These allocate
 *  space inside of `m_plots` -- a matrix of all the canvases
 *  we've drawn -- for the new combination of resolution + trigger.
 *
 *  But there are some modules that plot quantities at the event
 *  level, both as a function of trigger and NOT as a function of
 *  trigger. See the `EventRhoDrawer`, `BeamBackgroundDrawer`, and
 *  `StatusMapDrawer` for more examples.
 */
int ExampleDrawer::Draw(const std::vector<uint32_t> vecTrigToDraw,
                        const std::vector<uint32_t> vecResToDraw)
{
  // emit debugging message
  if (m_do_debug)
  {
    std::cout << "  -- Drawing component " << m_name << std::endl;
  }

  // loop over resolutions
  int nDraw = 0;
  for (const uint32_t resToDraw : vecResToDraw)
  {
    // add a row for resolution
    m_plots.AddRow();

    // now loop over triggers
    for (const uint32_t trigToDraw : vecTrigToDraw)
    {
      // add a column for trigger and draw hists
      m_plots.AddColumn();
      DoDrawing(trigToDraw, resToDraw);
      ++nDraw;
    }
  }
  return nDraw;
}

// ----------------------------------------------------------------------------
//! Generate HTML pages for each trigger and resolution
// ----------------------------------------------------------------------------
/*! Similar to `Draw`, this defines how to turn each canvas we've made
 *  into an HTML page. Also like `Draw`, for most cases you won't need
 *  to change this EXCEPT for the directory name (same comments in the
 *  code).
 *
 *  For examples of how to do this for event-wise quantities or things
 *  that aren't differential in trigger, see the examples mentioned
 *  in `Draw`.
 *
 *  The directory structure of the Jet QAhtml pages is like so:
 *    <trigger>/
 *      <event-wise component>/
 *        <event plot 1>
 *        <event plot 2>
 *        <etc>
 *      <resolution>/
 *        <jet-wise component>/
 *          <jet plot 1>
 *          <jet plot 2>
 *          <etc>
 */
int ExampleDrawer::MakeHtml(const std::vector<uint32_t> vecTrigToDraw,
                            const std::vector<uint32_t> vecResToDraw,
                            const QADraw& subsystem)
{
  // emit debugging messages
  if (m_do_debug)
  {
    std::cout << "  -- Creating HTML pages for " << m_name << std::endl;
  }

  // instantiate draw client
  QADrawClient* cl = QADrawClient::instance();

  // loop over resolutions and triggers
  for (std::size_t iRes = 0; iRes < vecResToDraw.size(); ++iRes)
  {
    // now loop over triggers
    for (std::size_t iTrig = 0; iTrig < vecTrigToDraw.size(); ++iTrig)
    {
      // grab index & name of trigger being drawn
      const uint32_t idTrig = vecTrigToDraw[iTrig];
      const std::string nameTrig = JetDrawDefs::MapTrigToName().at(idTrig);

      // grab index & name of resolution being drawn
      const uint32_t idRes = vecResToDraw[iRes];
      const std::string nameRes = JetDrawDefs::MapResToName().at(idRes);
      const std::string dirRes = nameTrig + "/" + nameRes;
      const std::string fileRes = nameTrig + "_" + nameRes;

      // make html pages
      //  - NOTE: you NEED to change the `/Example/` in `dir` below
      //  - This sets the subdirectory of where all of the plots from
      //    this drawer will go
      for (const auto& plot : m_plots.GetVecPlotPads(iRes, iTrig))
      {
        const std::string name = plot.canvas->GetName();
        const std::string dir = dirRes + "/Example/" + name; // CHANGE THIS
        const std::string png = cl->htmlRegisterPage(subsystem, dir, name, "png");
        cl->CanvasToPng(plot.canvas, png);
      }
    }
  }
  return 0;
}

// inherited private methods ==================================================

// ----------------------------------------------------------------------------
//! Draw histograms for a given trigger and resolution
// ----------------------------------------------------------------------------
/*! As mentioned above, this method defines what is actually drawn.
 *  Most of the changes you make will be done here!
 *
 *  More detailed comments are left in the code, but the general
 *  gist of what's being done is like so:
 *
 *    1. Connect to the QADrawClient (you'll need to retrieve histograms from it)
 *    2. Load a vector with all of the histograms you're going to draw.
 *       - This will be a vector of `JetDrawDefs::HistAndOpts`, structs that
 *         consolidate the histogram + all of the options we're going to
 *         draw it with
 *    3. For each page (canvas) you want to make you call `DrawHists` and give
 *       it a list of indices.
 *       - These indices correspond to the ENTRY order of the histograms we've
 *         added to the vector in step 2
 *       - It will make a plot canvas with THOSE histograms and add it to
 *         the `m_plots` matrix.
 *    4. And finally draw any additional histograms/profiles on the pads
 *       you've already drawn
 *
 *  Ideally, you souldn't put more than 4 panels/histograms on a given
 *  canvas/html page.
 */
void ExampleDrawer::DoDrawing(const uint32_t trig, const uint32_t res)
{
  // emit debugging message
  if (m_do_debug)
  {
    switch (res)
    {
      case JetDrawDefs::JetRes::R02:
        std::cout << "  -- Drawing dijet histograms (trig = " << trig << ", R = 0.2)" << std::endl;
        break;
      case JetDrawDefs::JetRes::R03:
        std::cout << "  -- Drawing dijet histograms (trig = " << trig << ", R = 0.3)" << std::endl;
        break;
      case JetDrawDefs::JetRes::R04:
        std::cout << "  -- Drawing dijet histograms (trig = " << trig << ", R = 0.4)" << std::endl;
        break;
      case JetDrawDefs::JetRes::R05:
        std::cout << "  -- Drawing dijet histograms (trig = " << trig << ", R = 0.5)" << std::endl;
        break;
      default:
        std::cerr << "Warning: trying to draw dijet histograms with Unknown resolution" << std::endl;
        return;
    }
  }

  // The histogram names are (by design) very long,
  // so it's useful to make a variable which collects
  // all the needed information beforehand.
  const std::string histName = m_hist_prefix
                             + "_"
                             + JetDrawDefs::MapTrigToTag().at(trig)
                             + "_"
                             + m_jet_type
                             + "_"
                             + JetDrawDefs::MapResToTag().at(res);


  // 1. connect to draw client
  QADrawClient* cl = QADrawClient::instance();

  // 2. grab histograms to draw and set options
  //    - Each bracket corresponds to the constructor for a
  //      `HistAndOpts`
  //    - Notice how in every case, we do dynamic cast to a
  //      TH1*? This is making use of the fact that ALL
  //      histograms derive from the TH1, so we can
  //      safely cast them as this without losing any
  //      information.
  JetDrawDefs::VHistAndOpts1D hists = {
    // histogram 0: some variable `varA` (a 1D histogram)
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_varA")), // pointer to the histogram
      "Jet A", // the title we're going to give the histogram
      "A", // the title of the x axis we're going to give it
      "Counts", // the title of the y axis we're going to give it
      "", // the title of the z axis (can be blank for 1D histograms or profiles)
      0.25, // the right margin of the pad we're going to draw on
      true, // make y axis log?
      false // make z axis log?
    },
    // histogram 1: some variable `varB` (also a 1D histogram)
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_varB")),
      "Jet B",
      "B",
      "counts",
      "",
      0.25,
      true,
      false
    },
    // histogram 2: `varB` vs. `varA` (a 2D histogram)
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_varsBvsA")),
      "Jet B vs. A",
      "A",
      "AB",
      "Counts",
      0.25,
      false, // NOTE: we are NOT making the y axis log
      true // but we ARE making the z axis log
    },
    // histogram 3: the profile of `varB` vs. `varA` (a TProfile)
    {
      dynamic_cast<TH1*>(cl->getHisto(histName + "_varsBvsA_pfx")),
      "Jet B vs. A",
      "A",
      "AB",
      "Counts",
      0.25,
      false,
      true
    }
  };

  // 3. Now we draw all of these on a single page
  //    - You will likely only need to change the 1st
  //      2 arguments (see inline comments below)
  //    - For event-wise modules, see the examples mentioned
  //      above
  //    More details:
  //    ```
  //    DrawHists("ExampleJetAB", // THIS is the name of the page you're going to draw on
  //              {0, 1, 2}, // THESE are the indices of the histograms you're going to
  //                         // draw IN THE ORDER you want them drawn
  //              hists, // you don't need to change this, this is just passing the vector
  //                     // of hists to the method
  //              trig, // this is the index of the trigger currently being drawn
  //              res // and this is the index of the resolution currently being drawn
  //              );
  //    ```
  DrawHists("ExampleJetAB", {0, 1, 2}, hists, trig, res);

  // 4. Finally, draw profiles on relevant pads.
  //    - Again, you should really only need to change the 1st 2
  //      arguments.
  //    - In this example, we're going to draw the profile (index
  //      3 in the `hists` vector) on the last pad drawn (index 3
  //      in the canvas), which has the 2D histogram in it.
  //    - NOTE: you can access a specific pad with:
  //        m_plots.GetPlotPad(iPad, res, trig);
  //    More details:
  //    ```
  //    DrawHistOnPad(3, // THIS is the index of the histogram/profile you're going to
  //                     // draw. Note that it is ENTRY order, so should be between 0
  //                     // and hists.size() - 1
  //                  3, // THIS is the index of the PAD you're going to draw the
  //                     // histogram/profile on. These range from 1 (not 0!) to
  //                     // the number of pads on the canvas (ideally not more than 4)
  //                  hists, // you don't need to change this, this is just passing the
  //                         // vector of hists to the method
  //                  m_plots.GetBackPlotPad() // and this is just grabbing the last
  //                                           // pad drawn
  //                  );
  DrawHistOnPad(3, 3, hists, m_plots.GetBackPlotPad());

  // and now we exit
  return;
}
