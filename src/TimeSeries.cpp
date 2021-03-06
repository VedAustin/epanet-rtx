//
//  TimeSeries.cpp
//  epanet-rtx
//
//  Created by the EPANET-RTX Development Team
//  See README.md and license.txt for more information
//  

#include <boost/foreach.hpp>

#include "TimeSeries.h"
#include "PersistentContainer.h"
#include "IrregularClock.h"

using namespace RTX;


TimeSeries::TimeSeries() : _units(1) {
  _name = "";  
  _cacheSize = 1000; // default cache size
  _hasClock = false;
  _points.reset( new PointContainer() );
  _clock.reset( new IrregularClock(_points) );
  
}

TimeSeries::~TimeSeries() {
  // empty Dtor
}

std::ostream& RTX::operator<< (std::ostream &out, TimeSeries &ts) {
  return ts.toStream(out);
}


#pragma mark - Public Methods


void TimeSeries::setName(const std::string& name) {
  _name = name;
}

std::string TimeSeries::name() {
  return _name; 
}

void TimeSeries::insert(Point thisPoint) {
  _points->insertPoint(thisPoint);
}

void TimeSeries::insertPoints(std::vector<Point> points) {
  _points->insertPoints(points);
  // TODO
  // check for size of point cache, pop a member if it's too large 
  // (first decide which side to pop from)
}

bool TimeSeries::isPointAvailable(time_t time) {
  return ( _points->isPointAvailable(time) );
}

Point TimeSeries::point(time_t time) {
  Point myPoint;
  time = clock()->validTime(time);
  
  if (_points->isPointAvailable(time)) {
    myPoint = _points->findPoint(time);
  }
  
  return myPoint;
}

// get a range of points from this TimeSeries' point method
std::vector< Point > TimeSeries::points(time_t start, time_t end) {
  // container for points in this range
  std::vector< Point > points;
  
  // sanity
  if ((start == end) || (start < 0) || (end < 0)) {
    return points;
  }
  // simple optimization
  _points->hintAtRange(start, end);
  
  std::vector<time_t> timeList = _clock->timeValuesInRange(start, end);
  
  BOOST_FOREACH(time_t time, timeList) {
    // check the time
    if (! (time >= start && time <= end) ) {
      // skip this time
      std::cerr << "time out of bounds. ignoring." << std::endl;
      continue;
    }
    Point aNewPoint;
    if ( !points.empty() && points.back().time() == time) {
      std::cerr << "duplicate time detected" << std::endl;
    }
    aNewPoint = point(time);
    
    if (!aNewPoint.isValid()) {
      //std::cerr << "bad point" << std::endl;
    }
    else {
      points.push_back(aNewPoint);
    }
  }
  
  /*
  Point aPoint;

  time_t queryTime;

 
  
  
  queryTime = start;
  while (queryTime <= end) {
    aPoint = pointAfter(queryTime);
    if (!aPoint) {
      break;
    }
    points.push_back(aPoint);
    queryTime = aPoint->time();
  }
   */
    
  return points;
}

std::pair< Point, Point > TimeSeries::adjacentPoints(time_t time) {
  Point previous, next;
  previous = pointBefore(time);
  next = pointAfter(time);
  return std::make_pair(previous, next);
}

Point TimeSeries::pointBefore(time_t time) {
  Point myPoint;
  
  myPoint = point(clock()->timeBefore(time));
  
  /* / if not, we depend on the PointRecord to tell us what the previous point is.
  else {
    myPoint = _points->pointBefore(time);
    myPoint = point(myPoint->time());
  }
   */
  
  return myPoint;
}

Point TimeSeries::pointAfter(time_t time) {
  Point myPoint;
  
  myPoint = point(clock()->timeAfter(time));

  
  /* / if not, we depend on the PointRecord to tell us what the next point is.
  else {
    myPoint = _points->pointAfter(time);  
    myPoint = point(myPoint->time());   // funny, but we have to call the point() method on the time that we discover here. this will call the most-derived point method.
  }
  */
  
  return myPoint;
}

double TimeSeries::value(time_t time) { 
  if (this->point(time).isValid()) {
    return this->point(time).value();
  }
  else return 0.;
}

Point::Qual_t TimeSeries::quality(time_t time) {
  return this->point(time).quality();
}

time_t TimeSeries::period() {
  if (_clock) {
    return _clock->period();
  }
  else {
    return 0;
  }
}

void TimeSeries::setCache(PointContainer::sharedPointer cache) {
  if(_points) {
    _points->reset();
  }
  _points = cache;
  // if my clock is irregular, then re-set it with the current pointRecord as the master synchronizer.
  if (!_clock->isRegular()) {
    _clock.reset( new IrregularClock(_points) );
  }
}

void TimeSeries::newCacheWithPointRecord(PointRecord::sharedPointer pointRecord) {
  // create new PersistentContainer and swap it in
  PointContainer::sharedPointer myPointContainer( new PersistentContainer(this->name(), pointRecord) );
  setCache(myPointContainer);
}

void TimeSeries::resetCache() {
  _points->reset();
}

void TimeSeries::setClock(Clock::sharedPointer clock) {
  _hasClock = (clock ? true : false);
  _clock = clock;
}

Clock::sharedPointer TimeSeries::clock() {
  return _clock;
}

void TimeSeries::setUnits(Units newUnits) {
  _units = newUnits;
}

Units TimeSeries::units() {
  return _units;
}

#pragma mark - Protected Methods

std::ostream& TimeSeries::toStream(std::ostream &stream) {
  stream << "Time Series: \"" << _name << "\"\n";
  if (_clock) {
    stream << "clock: " << *_clock;
  }
  stream << "Units: " << _units << std::endl;
  stream << "Cached Points:" << std::endl;
  stream << *_points;
  return stream;
}

bool TimeSeries::isCompatibleWith(TimeSeries::sharedPointer otherSeries) {
  
  // basic check for compatible regular time series.
  Clock::sharedPointer theirClock = otherSeries->clock(), myClock = this->clock();
  
  return (myClock->isCompatibleWith(theirClock) && (units().isDimensionless() || units().isSameDimensionAs(otherSeries->units())));
}



