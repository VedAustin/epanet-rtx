//
//  PointRecord.cpp
//  epanet-rtx
//
//  Created by the EPANET-RTX Development Team
//  See README.md and license.txt for more information
//  

#include <iostream>

#include "PointRecord.h"
#include "boost/foreach.hpp"

using namespace RTX;

PointRecord::PointRecord() {
  _nextKey = 0;
  _connectionString = "";
}

std::ostream& RTX::operator<< (std::ostream &out, PointRecord &pr) {
  return pr.toStream(out);
}

std::ostream& PointRecord::toStream(std::ostream &stream) {
  stream << "Point Record - connection: " << this->connectionString() << std::endl;
  return stream;
}

void PointRecord::setConnectionString(const std::string& connection) {
  _connectionString = connection;
}
const std::string& PointRecord::connectionString() {
  return _connectionString;
}


std::string PointRecord::registerAndGetIdentifier(std::string recordName) {
  // register the recordName internally and generate a unique key identifier
  
  // check to see if it's there first
  if (_keys.find(recordName) == _keys.end()) {
    _keys[recordName] = _nextKey;
    _names[_nextKey] = recordName;
    _nextKey++;
  }
  
  return recordName;
}

int PointRecord::identifierForName(std::string recordName) {
  if (_keys.find(recordName) != _keys.end()) {
    return _keys[recordName];
  }
  else return -1;
}

string PointRecord::nameForIdentifier(int identifier) {
  if (_names.find(identifier) != _names.end()) {
    return _names[identifier];
  }
  else return string("");
}