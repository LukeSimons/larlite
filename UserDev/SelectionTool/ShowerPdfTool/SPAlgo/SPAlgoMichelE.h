/**
 * \file SPAlgoMichelE.h
 *
 * \ingroup SPAlgo
 * 
 * \brief Class def header for a class SPAlgoMichelE
 *
 * @author davidkaleko
 */

/** \addtogroup SPAlgo

    @{*/

#ifndef SELECTIONTOOL_SPALGOMICHELE_H
#define SELECTIONTOOL_SPALGOMICHELE_H

#include "SPAlgo/SPAlgoSingleE.h"
#include "SPTBase/SPAlgoBase.h"

namespace sptool {

  /**
     \class SPAlgoMichelE
     User custom SPAFilter class made by davidkaleko
   */
  class SPAlgoMichelE : public SPAlgoBase {
  
  public:

    /// Default constructor
    SPAlgoMichelE();

    /// Default destructor
    virtual ~SPAlgoMichelE(){};

    /// Reset function
    virtual void Finalize();

    /// What to do before event-loop begins
    virtual void ProcessBegin();

    /// What to do once event-loop is over
    virtual void ProcessEnd(TFile* fout);

    void InitializeHistos();

    /// Override the sptool::SPTBase::LoadParams function
    virtual void LoadParams(std::string fname="",size_t version=kINVALID_SIZE);

    /// Function to evaluate input showers and determine a score
    virtual SPArticleSet Reconstruct(const SPAData &data);
    
    
  protected:
    
    SPAlgoSingleE _alg_singleE;

    TH1F* michel_energy;

  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
