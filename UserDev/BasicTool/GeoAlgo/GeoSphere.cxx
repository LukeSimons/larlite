#ifndef BASICTOOL_GEOSPHERE_CXX
#define BASICTOOL_GEOSPHERE_CXX

#include "GeoSphere.h"

namespace geoalgo {

  Sphere::Sphere() : _center (3)
		   , _radius (0)
  { for(auto& v : _center) v=0; }
    
  Sphere::Sphere(const double& x,const double& y,const double& z,const double& r)
    : Sphere()
  { _center[0] = x; _center[1] = y; _center[2] = z; _radius = r; }

  Sphere::Sphere(const Point_t& center, const double r)
    : _radius (r)
  {compat(center,_radius); _center = center;}

  Sphere::Sphere(const Point_t& pt1, const Point_t& pt2)
  {
    compat(pt1);
    compat(pt2);
    _center = (pt1+pt2)/2.;
    _radius = pt1.Dist(pt2)/2.;
  }

  Sphere::Sphere(const Point_t& A, const Point_t& B, const Point_t& C)
  {
    compat(A);
    compat(B);
    compat(C);
    // any three points are co-planar
    // (if collinear no sphere passing  through all 3)
    // These 3 points make a triangle
    // find the perpendicular bi-sectors to the segments
    // making up the triangle. They will intersect
    // at the sphere's center
    
    // check if collinear. If so return exception
    Vector_t AB(B-A);
    Vector_t AC(C-A);
    Vector_t BC(C-B);
    
    double dABAB = AB.Dot(AB);
    double dACAC = AC.Dot(AC);
    double dABAC = AB.Dot(AC);
    
    double d = dABAB * dACAC - dABAC * dABAC;
    double s,t;
    
    // if d == 0 they lie on one line
    if (d == 0){
      std::cout << "d is 0!" << std::endl;
      double lenAB = AB.Length();
      double lenAC = AC.Length();
      double lenBC = BC.Length();
      // which segment is longest?
      if ( (lenAB > lenAC) && (lenAB > lenBC) ){
	_center = (A+B)/2.;
	_radius = _center.Dist(A);
      }
      else if( lenAC > lenBC ){
	_center = (A+C)/2.;
	_radius = _center.Dist(A);
      }
      else{
	_center = (B+C)/2;
	_radius = _center.Dist(B);
      }
    }// if d == 0
    
    else{
      std::cout << "d not == 0!" << std::endl;
      s = 0.5 * ( dABAB * dACAC - dACAC * dABAC ) / d;
      t = 0.5 * ( dACAC * dABAB - dABAB * dABAC ) / d;
      
      // if s & t both > 0 && 1-s-t also > 0 then P = A + s*(B-A) + t*(C-A) is the center
      if ( (s > 0) && (t > 0) && ((1-s-t) > 0) ){
	_center = A+(B-A)*s+(C-A)*t;
	_radius = _center.Dist(A);
      }
      
      // otherwise only one will be negative. The side it belongs on will be
      // the longest side and will determine the side to take as diameter
      else if (s <= 0){
	// side AB is the one
	_center = (A+C)/2.;
	_radius = _center.Dist(A);
      }
      else if (t <= 0){
	// AC is the side
	_center = (A+B)/2.;
	_radius = _center.Dist(A);
      }
      else{
	_center = (B+C)/2;
	_radius = _center.Dist(B);
      }
    }// else (if d not equal to 0)
    
  }
  
  //  Alternative ctor (4) - 4 Points
  //  http://steve.hollasch.net/cgindex/geometry/sphere4pts.html (TOP)
  Sphere::Sphere(const Point_t& A, const Point_t& B, const Point_t& C, const Point_t& D){
    compat(A);
    compat(B);
    compat(C);
    compat(D);
    
    // get sphere from 3 points (A,B,C)
    Vector_t AB(A-B);
    Vector_t AC(A-C);
    Sphere S(A,B,C);
    Point_t P = S.Center();
    // get the direction perpendicular to plane in which ABC lie (simply cross product of any two segments)
    Vector_t d = AB.Cross(AC);
    d.Normalize();
    // Find E: point on circle ABC that lies on plane passing through direction d and point D
    // to do this from P (center of circle) go in the direction of the PD vector a length = to circle radius
    Vector_t PD(D-P);
    PD.Normalize();
    Vector_t PDcrossd = PD.Cross(d);
    // find vector perpendicular to both d and PDcrossd
    double denom = (d[1] * PDcrossd[2]) - (PDcrossd[1] * d[2]);
    if (denom == 0)
      throw GeoAlgoException("divide by 0 in GeoSphere! Fix Me!");
    double x=1;
    double y,z;
    z = x * ( (d[0] * PDcrossd[1]) - (d[1] * PDcrossd[0]) ) / denom;
    if (d[1] ==0)
      throw GeoAlgoException("divide by 0 in GeoSphere! Fix Me!");
    y = ((d[0] * x) + (d[2] * z))/(-d[1]);
    // get bi-sector direction for AB
    Vector_t EP(x,y,z);
    EP.Normalize();
    Point_t E(P+EP*S.Radius());
    // The circle center will be the point on the line d passing through P that is half-way through E and D
    // this is the intersection of the ED bisector and the line passing through P with direction d
    // Midpoint between E and D is M:
    Point_t M((E+D)/2.);
    // direction of ED line:
    Vector_t ED(E-D);
    ED.Normalize();
    // bi-sector direction trhough M that intersects OP is perpendicular to ED and PDcrossd
    denom = (ED[1] * PDcrossd[2]) - (PDcrossd[1] * ED[2]);
    if (denom == 0)
      throw GeoAlgoException("divide by 0 in GeoSphere! Fix Me!");
    z = x * ( (ED[0] * PDcrossd[1]) - (ED[1] * PDcrossd[0]) ) / denom;
    if (ED[1] ==0)
      throw GeoAlgoException("divide by 0 in GeoSphere! Fix Me!");
    y = ((ED[0] * x) + (ED[2] * z))/(-ED[1]);
    // get bi-sector direction for AB
    Vector_t EDbisector(x,y,z);
    EDbisector.Normalize();
    // now find intersection between line passing through P with direction d
    // and line passing through M with direction EDbisector
    denom = (EDbisector[1]*d[0]-d[1]*EDbisector[0]);
    if (denom == 0)
      throw GeoAlgoException("divide by 0 in GeoSphere! Fix Me!");
    double t2 = ( (d[0] * (P[1]-M[1]))
		  + (d[1] * (M[0]-P[0])) ) / denom;
    Point_t O = M + EDbisector*t2;
    _center = O;
    // radius is distance to any of the 4 points
    _radius = _center.Dist(A);
    //cout distances to check
    //std::cout << "dist to A: " << _center.Dist(A) << "\tB: " << _center.Dist(B) << "\tC: " << _center.Dist(C) << "\tD: " << _center.Dist(D) << std::endl;
  }
  
  
  // Alternative ctor (5) - Set of points
  Sphere::Sphere(const std::vector< ::geoalgo::Point_t>& pts)
    : _center(0,0,0)
    , _radius(0)
  {
    
    switch(pts.size()) {
    case 0:
      break;
    case 1: _center = pts.front();
      break;
    case 2: (*this) = Sphere(pts[0],pts[1]);
      break;
    case 3: (*this) = Sphere(pts[0],pts[1],pts[2]);
      break;
    case 4: (*this) = Sphere(pts[0],pts[1],pts[2],pts[3]);
      break;
    default:
      throw GeoAlgoException("Cannot call Sphere constructor with more than 4 points. Something went wront");	
    }
  }
  
  const Point_t& Sphere::Center() const { return _center; }
  
  const double Sphere::Radius() const { return _radius; }
  
  void Sphere::Center(const double x, const double y, const double z)
  { _center[0] = x; _center[1] = y; _center[2] = z; }

  void Sphere::Center(const Point_t& center)
  { compat(center); _center = center; }

  void Sphere::Radius(const double& r)
  { compat(r); _radius = r; }

  bool Sphere::Contain(const Point_t& p) const
  {
    _center.compat(p);
    return ( p._Dist_(_center) < _radius );
  }
  
  void Sphere::compat(const Point_t& p, const double r) const
  { 
    if(p.size()!=3) throw GeoAlgoException("Only 3D points allowed for sphere"); 
    compat(r);
  }
  
  void Sphere::compat(const double& r) const
  { if(r<0) throw GeoAlgoException("Only positive value allowed for radius"); }
  
}

#endif

