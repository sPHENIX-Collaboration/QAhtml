#include "OnlProdRunDBodbc.h"

#include <odbc++/connection.h>
#include <odbc++/errorhandler.h>
#include <odbc++/drivermanager.h>
#include <odbc++/resultset.h>
#include <odbc++/preparedstatement.h>

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

OnlProdRunDBodbc::OnlProdRunDBodbc():
  runnumber(0),
  verbosity(0),
  dbname("daq"),
  dbowner("phnxrc"),
  dbpasswd(""),
  runtype("UNKNOWN"),
  eventsinrun(0),
  brunixtime(0),
  erunixtime(0)
{
}

OnlProdRunDBodbc::OnlProdRunDBodbc(const int runno):
  runnumber(0),
  verbosity(0),
  dbname("daq"),
  dbowner("phnxrc"),
  dbpasswd(""),
  runtype("UNKNOWN"),
  eventsinrun(0),
  brunixtime(0),
  erunixtime(0)
{
  FillFromdaqDB(runno);
}


void
OnlProdRunDBodbc::identify() const
{
  cout << "DB Name: " << dbname << endl;
  cout << "DB Owner: " << dbowner << endl;
  cout << "DB Pwd: " << dbpasswd << endl;
  return ;
}

void
OnlProdRunDBodbc::FillFromdaqDB(const int runno)
{
  odbc::Connection *con = 0;
  odbc::Statement* query = 0;
  odbc::ResultSet *rs = 0;
  ostringstream cmd;

  try
    {
      con = odbc::DriverManager::getConnection(dbname.c_str(), dbowner.c_str(), dbpasswd.c_str());
    }
  catch (odbc::SQLException& e)
    {
      cout << __PRETTY_FUNCTION__
           << " Exception caught during DriverManager::getConnection" << endl;
      cout << "Message: " << e.getMessage() << endl;
      goto noopen;
    }

  query = con->createStatement();
  cmd << "SELECT runtype,runstate,eventsinrun,brunixtime,erunixtime FROM RUN WHERE RUNNUMBER = "
      << runno;
  if (verbosity > 0)
    {
      cout << "command: " << cmd.str() << endl;
    }
  try
    {
      rs = query->executeQuery(cmd.str());
    }
  catch (odbc::SQLException& e)
    {
      cout << "Exception caught" << endl;
      cout << "Message: " << e.getMessage() << endl;
    }
  if (rs && rs->next())
    {
      runtype = rs->getString("runtype");
      eventsinrun = rs->getInt("eventsinrun");
      brunixtime = rs->getInt("brunixtime");
      erunixtime = rs->getInt("erunixtime");
    }
  delete rs;
  delete query;
  runnumber = runno;
 noopen:
  delete con;
  return;
}

