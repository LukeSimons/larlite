/**
 * \file GeoLine.h
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class Line
 *
 * @author David Caratelli
 */

/** \addtogroup GeoAlgo

    @{*/
#ifndef BASICTOOL_GEOLINE_H
#define BASICTOOL_GEOLINE_H

#include "GeoHalfLine.h"

namespace geoalgo {
  /**
     \class Line
     @brief Representation of a 3D infinite line.
     Defines an infinite 3D line by having 2 points which completely determine the line
     along which the line extends. It hides the point attributes from users for   \n
     protecting the dimensionality.
  */
  class Line {
    
  public:
    
    /// Default constructor
    Line() 
      : _pt1(3)
      , _pt2(3)
    {}

    /// Alternative ctor (1)
    Line(const double x1, const double y1, const double z1,
	 const double x2, const double y2, const double z2)
      : _pt1 (x1, y1, z1)
      , _pt2 (x2, y2, z2)
    { check_and_raise(_pt1,_pt2); }

    /// Altenartive ctor (2)
    Line(const Point_t& pt1, const Point_t& pt2)
      : _pt1 ( pt1 )
      , _pt2 ( pt2 )
    { check_and_raise(pt1,pt2); }

    /// Alternative ctor using template (3)
    template <class T, class U> Line(const T& pt1, const U& pt2)
      : Line(Point_t(pt1), Point_t(pt2))
    {}

    /// Default destructor
    virtual ~Line(){};

    /// Start getter
    const Point_t& Pt1() const { return _pt1; }

    /// Direction getter
    const Point_t& Pt2() const { return _pt2; }
    
    /// Pt1 setter
    void Pt1(const double x, const double y, const double z)
    { 
      _pt1[0] = x; 
      _pt1[1] = y; 
      _pt1[2] = z; 
      check_and_raise(_pt1,_pt2);
    }

    /// Pt2 setter
    void Pt2(const double x, const double y, const double z)
    { 
      _pt2[0] = x; 
      _pt2[1] = y; 
      _pt2[2] = z; 
      check_and_raise(_pt1,_pt2);
    }

    /// Pt1 setter template
    template<class T>
    void Pt1(const T& pt1)
    { 
      _pt1 = Point_t(pt1); 
      check_and_raise(_pt1,_pt2);
    }
    
    /// Pt2 setter template
    template<class T>
    void Pt2(const T& pt2)
    { 
      _pt2 = Vector_t(pt2);
      check_and_raise(_pt1,_pt2);
    }

  protected:

    void check_and_raise(const Point_t& p1, const Point_t& p2) const
    { 
      if(p1.size()!=3) throw GeoAlgoException("<<check_and_raise>> Pt1 is not 3 dimensional point!");
      if(p2.size()!=3) throw GeoAlgoException("<<check_and_raise>> Pt2 is not 3 dimensional point!");
      if(p1 == p2) throw GeoAlgoException("<<check_and_raise>> Two identical points not allowed for Line ctor!"); 
    }

    /// First point denoting infinite line
    Point_t  _pt1;

    /// Second point denoting infinite line
    Vector_t _pt2;

  };

  /**
     \class DirectedLine
     @brief Representation of a 3D infinite line.
     Defines an infinite 3D line with a point and a direction.
     Line points are constructed like this:
     (pt, dir) -> (pt, pt+dir)
     It hides the point attributes from users for protecting the dimensionality.
  */
  class DirectedLine : public Line {

  public:

    /// Alternative ctor (1)
    DirectedLine(const double x, const double y, const double z,
		 const double dirx, const double diry, const double dirz)
      : Line( x, y, z, x+dirx, y+diry, z+dirz)
    { check_and_raise(_pt1,_pt2); }
    
    /// Altenartive ctor (2)
    DirectedLine(const Point_t& pt, const Vector_t& dir)
      : Line( pt, pt+dir)
    { check_and_raise(_pt1,_pt2); }

    /// Alternative ctor (3)
    DirectedLine(const HalfLine& l)
      : Line( l.Start(), l.Start()+l.Dir() )
    { check_and_raise(_pt1,_pt2); }

    /// Alternative ctor using template (3)
    template <class T, class U> DirectedLine(const T& pt, const U& dir)
      : Line(Point_t(pt), Point_t(pt+dir))
      {}
    
    Vector_t Dir() const
    { return _pt2 - _pt1; }
    
  };
  
  typedef Line Line_t;
  typedef DirectedLine DirectedLine_t;
}
#endif
/** @} */ // end of doxygen group 
