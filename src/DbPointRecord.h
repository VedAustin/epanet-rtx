//
//  DbPointRecord.h
//  epanet-rtx
//
//  Created by the EPANET-RTX Development Team
//  See README.md and license.txt for more information
//  

#ifndef epanet_rtx_DbPointRecord_h
#define epanet_rtx_DbPointRecord_h

#include "PointRecord.h"
#include "rtxExceptions.h"

namespace RTX {
  
  /*! \class DbPointRecord
   \brief A persistence layer for databases
   
   Base class for database-connected PointRecord classes.
   
   */
  
  class DbPointRecord : public PointRecord {
  public:
    RTX_SHARED_POINTER(DbPointRecord);
    DbPointRecord() {};
    virtual ~DbPointRecord() {};
    
    //exceptions specific to this class family
    class RtxDbConnectException : public RtxException {
    public:
      virtual const char* what() const throw()
      { return "Could not connect to database.\n"; }
    };
    class RtxDbRetrievalException : public RtxException {
      virtual const char* what() const throw()
      { return "Could not retrieve data.\n"; }
    };
    
  };

  
}

#endif
