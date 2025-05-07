// Derek Anderson <ruse-traveler@github.com>

#ifndef BASE_JET_DRAWER_H
#define BASE_JET_DRAWER_H

// ============================================================================
//! Base class for jet QA components
// ============================================================================
/*! Base class to define common functionality across
 *  the different components of the jet QA. Each
 *  jet QA module should have a corresponding
 *  "drawer" class inheriting from this one.
 */
class BaseJetDrawer
{

  protected:

    /* TODO inheritable things will go here */

  public:

    /* TODO common interface things will go here */

    // ------------------------------------------------------------------------
    //! default ctor/dtor
    // ------------------------------------------------------------------------
    BaseJetDrawer()  {};
    ~BaseJetDrawer() {};

};  // end BaseJetDrawer

#endif
