#! /bin/csh
 	
if(! $?QA_MAIN ) then
  echo "QA_MAIN environment variable not set, exiting"
  exit -1
endif

if (! $?QA_HTMLDIR) then
  setenv ONLPROD_HTMLDIR /common/s6/htmltest/OnlProd
endif

if (-f ${OPT_SPHENIX}/bin/setup_local.csh ) then
  source ${OPT_SPHENIX}/bin/setup_local.csh $ONLMON_MAIN
else
  echo no ${OPT_SPHENIX}/bin/setup_local.sh
  exit -1
endif
