//
//  Point.cpp
//  epanet-rtx
//
//  Created by the EPANET-RTX Development Team
//  See README.md and license.txt for more information
//  

#include "Point.h"

using namespace RTX;

Point::Point() {
  _time = 0;
  _value = 0;
  _qual = Point::missing;
  _confidence = 0;
}


Point::Point(time_t time, double value, Qual_t qual, double confidence) {
  _time = time;
  _value = value;
  _qual = qual;
  _confidence = confidence;
}

Point::~Point() {
}


#pragma mark - Operators

Point Point::operator+(const Point& point) const {
  Point result = *this;
  result += point;
  return result;
}

Point Point::operator+=(const Point& point) {
  if (point.time() != this->_time) {
    std::cerr << "point times do not match: " << (point.time() - this->_time) << "s gap." << std::endl;
  }
  this->_value += point.value();
  (this->_confidence += point.confidence()) /= 2.; // TODO - figure  out confidence intervals.
  return *this;
}

Point Point::operator*(const double factor) const {
  double value;
  double confidence;
  time_t time;
  
  value = (this->_value * factor);
  confidence = (this->_confidence * factor);  // TODO -- figure out confidence intervals.
  time = this->_time;
  
  return Point(time, value, Point::good, confidence);
}

Point Point::operator/(const double factor) const {
  double value;
  double confidence;
  time_t time;
  
  value = (this->_value / factor);
  confidence = (this->_confidence / factor);  // TODO -- figure out confidence intervals.
  time = this->_time;
  
  return Point(time, value, Point::good, confidence);
}





#pragma mark - Public Methods

time_t Point::time() const {
  return _time;
}

double Point::value() const {
  return _value;
}

Point::Qual_t Point::quality() const {
  return _qual;
}

double Point::confidence() const {
  return _confidence;
}


std::ostream& RTX::operator<< (std::ostream &out, Point &point) {
  out << "(" << point._time << "," << point._value << "," << point._qual << ")";
  return out;
}



#pragma mark - Class Methods

Point::sharedPointer Point::convertPoint(const Point& point, const Units& fromUnits, const Units& toUnits) {
  double value = Units::convertValue(point._value, fromUnits, toUnits);
  double confidence = Units::convertValue(point._confidence, fromUnits, toUnits);
  Point::sharedPointer returnPoint(new Point(point.time(), value, Point::good, confidence));
  return returnPoint;
}



