//
//  Junction.cpp
//  epanet-rtx
//
//  Created by the EPANET-RTX Development Team
//  See README.md and license.txt for more information
//  

#include <iostream>

#include "junction.h"

using namespace RTX;

Junction::Junction(const std::string& name) : Node(name) {
  //_demand = 0;
  //_baseDemand = 0;
  setType(JUNCTION);
  
  // we have nothing yet
  _doesHaveQualitySource = false;
  _doesHaveBoundaryFlow = false;
  _doesHaveHeadMeasure = false;
  _doesHaveQualityMeasure = false;
  
  _headState.reset(new TimeSeries());
  _qualityState.reset(new TimeSeries());
  _demandState.reset(new TimeSeries());
  
  _headState->setUnits(RTX_METER);
  _qualityState->setUnits(RTX_SECOND);
  _demandState->setUnits(RTX_LITER_PER_SECOND);
  
  _headState->setName("N " + name + " head");
  _qualityState->setName("N " + name + " quality");
  _demandState->setName("N " + name + " demand");
  
  _baseDemand = 0;
}
Junction::~Junction() {
  
}

double Junction::baseDemand() {
  return _baseDemand;
}

void Junction::setBaseDemand(double demand) {
  _baseDemand = demand;
}

void Junction::setRecord(PointRecord::sharedPointer record) {
  _headState->newCacheWithPointRecord(record);
  _qualityState->newCacheWithPointRecord(record);
  _demandState->newCacheWithPointRecord(record);
}

// states - these recieve simulation results from the Model containing class
TimeSeries::sharedPointer Junction::head() {
  return _headState;
}
TimeSeries::sharedPointer Junction::quality() {
  return _qualityState;
}
TimeSeries::sharedPointer Junction::demand() {
  return _demandState;
}

// measured demand (boundary flow condition)
bool Junction::doesHaveBoundaryFlow() {
  return _doesHaveBoundaryFlow;
}
void Junction::setBoundaryFlow(TimeSeries::sharedPointer flow) {
  _doesHaveBoundaryFlow = (flow ? true : false);
  _boundaryFlow = flow;
}
TimeSeries::sharedPointer Junction::boundaryFlow() {
  return _boundaryFlow;
}

// head measurement
bool Junction::doesHaveHeadMeasure() {
  return _doesHaveHeadMeasure;
}
void Junction::setHeadMeasure(TimeSeries::sharedPointer headMeasure) {
  _doesHaveHeadMeasure = (headMeasure ? true : false);
  _headMeasure = headMeasure;
}
TimeSeries::sharedPointer Junction::headMeasure() {
  return _headMeasure;
}

// quality measurement
bool Junction::doesHaveQualityMeasure() {
  return _doesHaveQualitySource;
}
void Junction::setQualityMeasure(TimeSeries::sharedPointer quality) {
  _doesHaveQualityMeasure = (quality ? true : false);
  _qualityMeasure = quality;
}
TimeSeries::sharedPointer Junction::qualityMeasure() {
  return _qualityMeasure;
}

// quality boundary condition
bool Junction::doesHaveQualitySource() {
  return _doesHaveQualitySource;
}
void Junction::setQualitySource(TimeSeries::sharedPointer quality) {
  _doesHaveQualitySource = (quality ? true : false);
  _qualityBoundary = quality;
}
TimeSeries::sharedPointer Junction::qualitySource() {
  return _qualityBoundary;
}
