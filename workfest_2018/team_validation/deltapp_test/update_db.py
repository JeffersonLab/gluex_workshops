#!/usr/bin/env python
import MySQLdb
import os
import datetime

# extract info from context
try:
    run = os.environ['RUN']
except:
    print "Could not find run number!  Exiting..."
    os.exit(0)

try:
    bms_osname = os.environ['BMS_OSNAME']
except:
    bms_osname = ""

try:
    #datestr = os.environ['B1Pi_TEST_DIR'].split('/')[-2]
    datestr = os.getwd().split('/')[-2]
except:
    now = datetime.datetime.now()
    datestr = now.strftime('%Y-%m-%d')

# read in values
vals = []
with open("data.txt") as f:
    for line in f:
        vals.append(line.strip())

# do database stuff
db_conn = MySQLdb.connect("hallddb.jlab.org", "datmon", "", "data_monitoring")
db = db_conn.cursor()

# put something into the display configuration for testing
config = '{ "delta_yield": {"lowLimit": 700} }'

db.execute("INSERT INTO deltapp_test_data (platform,date_generated,run,delta_yield,config,comment)  VALUES ('%s','%s',%s,%s,'%s','%s')"%(bms_osname,datestr,run,vals[0],config,""))
#db.execute("INSERT INTO deltapp_test_data (platform,date_generated,run,delta_yield,delta_mass,config,comment)  VALUES ('%s','%s',%s,%s,%s,'%s','%s')"%(bms_osname,datestr,run,vals[0],vals[1],vals[2],config,""))

db_conn.commit()
db_conn.close()
