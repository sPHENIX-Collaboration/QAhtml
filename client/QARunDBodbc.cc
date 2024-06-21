#include "QARunDBodbc.h"

#include <odbc++/connection.h>
#include <odbc++/drivermanager.h>
#include <odbc++/errorhandler.h>
#include <odbc++/preparedstatement.h>
#include <odbc++/resultset.h>
#include <odbc++/types.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

QARunDBodbc::QARunDBodbc()
  : runnumber(0)
  , verbosity(0)
  , dbname("daq")
  , dbowner("phnxrc")
  , dbpasswd("")
  , runtype("UNKNOWN")
  , eventsinrun(0)
  , brunixtime(0)
  , erunixtime(0)
{
}

QARunDBodbc::QARunDBodbc(const int runno)
  : runnumber(0)
  , verbosity(0)
  , dbname("daq")
  , dbowner("phnxrc")
  , dbpasswd("")
  , runtype("UNKNOWN")
  , eventsinrun(0)
  , brunixtime(0)
  , erunixtime(0)
{
  FillFromdaqDB(runno);
}

void QARunDBodbc::identify() const
{
  std::cout << "DB Name: " << dbname << std::endl;
  std::cout << "DB Owner: " << dbowner << std::endl;
  std::cout << "DB Pwd: " << dbpasswd << std::endl;
  return;
}

void QARunDBodbc::FillFromdaqDB(const int runno)
{
  odbc::Connection* con = nullptr;
  odbc::Statement* query = nullptr;
  odbc::ResultSet* rs = nullptr;
  std::ostringstream cmd;

  try
  {
    con = odbc::DriverManager::getConnection(dbname.c_str(), dbowner.c_str(), dbpasswd.c_str());
  }
  catch (odbc::SQLException& e)
  {
    std::cout << __PRETTY_FUNCTION__
              << " Exception caught during DriverManager::getConnection" << std::endl;
    std::cout << "Message: " << e.getMessage() << std::endl;
    delete con;
    return;
  }

  query = con->createStatement();
  cmd << "SELECT runtype,eventsinrun,brtimestamp,ertimestamp FROM RUN WHERE RUNNUMBER = "
      << runno;
  if (verbosity > 0)
  {
    std::cout << "command: " << cmd.str() << std::endl;
  }
  try
  {
    rs = query->executeQuery(cmd.str());
  }
  catch (odbc::SQLException& e)
  {
    std::cout << "Exception caught" << std::endl;
    std::cout << "Message: " << e.getMessage() << std::endl;
  }
  if (rs && rs->next())
  {
    runtype = rs->getString("runtype");
    eventsinrun = rs->getInt("eventsinrun");
    odbc::Timestamp brtimestamp = rs->getTimestamp("brtimestamp");
    brunixtime = brtimestamp.getTime();
    odbc::Timestamp ertimestamp = rs->getTimestamp("ertimestamp");
    erunixtime = ertimestamp.getTime();
    brtimestr = brtimestamp.toString(); // timestamp is in 'America/New_York' time zone
  }
  delete rs;
  delete query;
  runnumber = runno;
  delete con;
  return;
}
