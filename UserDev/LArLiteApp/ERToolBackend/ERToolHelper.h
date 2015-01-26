/**
 * \file ERToolHelper.h
 *
 * \ingroup LArLite_ERTool
 * 
 * \brief Class def header for a class ERToolHelper
 *
 * @author kazuhiro
 */

/** \addtogroup LArLite_ERTool

    @{*/
#ifndef LARLITE_ERTOOLHELPER_H
#define LARLITE_ERTOOLHELPER_H

#include <iostream>
#include <map>
#include <TRandom.h>

#include "DataFormat/mctruth.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mctrack.h"

#include "DataFormat/shower.h"
#include "DataFormat/track.h"
#include "DataFormat/vertex.h"
#include "DataFormat/cosmictag.h"
#include "DataFormat/calorimetry.h"
#include "DataFormat/partid.h"

#include "ERTool/Base/EventData.h"
#include "ERTool/Base/Particle.h"

//Use ShowerProfile to calculate Height & Radius from Energy
#include "EMShowerTools/EMShowerProfile.h"

namespace larlite {
  /**
     \class ERToolHelper
     @brief A collection of helper functions to generate SPAData to be used in SPT fmwk.
  */
  class ERToolHelper{
    
  public:
    
    /// Default constructor
    ERToolHelper(){};
    
    /// Default destructor
    virtual ~ERToolHelper(){};

    /// Create MC EventData and ParticleSet
    void FillMCInfo( const event_mctruth&   mci_v,
		     const event_mcshower&  mcs_v,
		     const event_mctrack&   mct_v,
		     ::ertool::EventData&   event_data,
		     ::ertool::ParticleSet& particle_set) const;

    /// Fill Track Info from MC
    void FillTracks( const event_mctrack& mct_v,
		     ::ertool::EventData& res ) const;
    
    /// Fill Track Info from RECO
    void FillTracks ( const event_track&       trk_v,
		      const event_cosmictag&   cos_trk_v,
		      const event_calorimetry& calo_trk_v,
		      const event_partid&      pid_trk_v,
		      ::ertool::EventData& res ) const;

    /// Fill Shower Info from MC
    void FillShowers ( const event_mcshower& mcs_v,
		       ::ertool::EventData& res ) const;

    /// Fill Shower Info from RECO
    void FillShowers ( const event_shower& shw_v,
		       const event_cosmictag& ctag_shw_v,
		       ::ertool::EventData& res ) const;

    /// Fill Vertex Info from MC
    void FillVertices ( const event_vertex& vtx_v, 
			::ertool::EventData& res ) const;
    
    /// Fill Vertex Info from RECO
    void FillVertices ( const event_mctruth& mci_v,
			::ertool::EventData& res) const;

  private:
    
    EMShowerProfile _shrProfiler;

  };
}

#endif
/** @} */ // end of doxygen group 
