#!/usr/bin/env python
# exercise vs tabular dataframe output format
# Author D. Rousseau LAL, Jul 2019


import sys
import os
import gzip
import copy
import pandas as pd
import time

print ("using pandas version :",pd.__version__)

debug=False
gz=True

url = "http://opendata.cern.ch/record/328/files/atlas-higgs-challenge-2014-v2.csv.gz"
filename="atlas-higgs-challenge-2014-v2.csv.gz"

from urllib.request import urlretrieve


# download from open data portal if does not exist
if not os.path.exists(filename):
        print("downloading " + filename + "...", end='')
        t0 = time.time()        
        urlretrieve(url, filename)
        print ("... took : ",time.time() - t0)

t0 = time.time()
data = pd.read_csv(filename)
print ("Reading: ",filename," took ", time.time() - t0," s. Size : ",os.path.getsize(filename)/2**20," MB")
pd.options.display.max_columns = None
pd.options.display.width=200

if debug:
    print ("only take first few rows for debug")
    data=data.head(50)

# original file is rounded to 3 decimal except the two weight columns
# need to apply same rounding on dataframe again, otherwise some time I get 0.1239999999
# and the rewrotten csv file is significantly larger
columns=list(data.keys())
## columns=['EventId', 'DER_mass_MMC', 'DER_mass_transverse_met_lep',
##        'DER_mass_vis', 'DER_pt_h', 'DER_deltaeta_jet_jet', 'DER_mass_jet_jet',
##        'DER_prodeta_jet_jet', 'DER_deltar_tau_lep', 'DER_pt_tot', 'DER_sum_pt',
##        'DER_pt_ratio_lep_tau', 'DER_met_phi_centrality',
##        'DER_lep_eta_centrality', 'PRI_tau_pt', 'PRI_tau_eta', 'PRI_tau_phi',
##        'PRI_lep_pt', 'PRI_lep_eta', 'PRI_lep_phi', 'PRI_met', 'PRI_met_phi',
##        'PRI_met_sumet', 'PRI_jet_num', 'PRI_jet_leading_pt',
##        'PRI_jet_leading_eta', 'PRI_jet_leading_phi', 'PRI_jet_subleading_pt',
##        'PRI_jet_subleading_eta', 'PRI_jet_subleading_phi', 'PRI_jet_all_pt',
##        'Weight', 'Label', 'KaggleSet', 'KaggleWeight']

#build a dictionary with the number of decimal to round
columnformat={}
for c in columns:
    if c in ["Weight", "KaggleWeight"]: # "Weight","Label", "KaggleSet", "KaggleWeight"
        columnformat[c]=12 # full precision (but not enough to match completely the format)
    else:
        columnformat[c]=3 # float or int
data=data.round(columnformat)        

#print (data.head(5))

outfile="atlas-higgs-challenge-2014-v2-out1"    
outfilehdf=outfile+".hdf"

#output file is not automatically overwritten, remove if exists
if os.path.exists(outfilehdf):
    os.remove(outfilehdf)

#hdfcommand='data.to_hdf(outfilehdf, index=False, key="higgs")' # default compression
hdfcommand='data.to_hdf(outfilehdf, index=False, key="higgs", complib="zlib",complevel=9)' # maximal compression
print("Writing hdf with command: ",hdfcommand)
t0 = time.time()
exec(hdfcommand)
t1 = time.time()
print ("Writing: ",outfilehdf," took ", t1 - t0," s. Size : ",os.path.getsize(outfilehdf)/2**20," MB")
redatahdf=pd.read_hdf(outfilehdf)
print ("Reading back: ",outfilehdf," took ", time.time() - t1," s")
print (" ")


outfilecsv=outfile+".csv"
if gz:
    outfilecsv+=".gz"

csvcommand='data.to_csv(outfilecsv, index=False, compression="infer")' # works but cannot specify which column,float_format='%.3f')
print("Writing csv with command: ",csvcommand)
t0 = time.time()
exec(csvcommand)
t1 = time.time()
print ("Writing: ",outfilecsv," took ", time.time() - t0," s. Size : ",os.path.getsize(outfilecsv)/2**20," MB")                
redatacsv=pd.read_csv(outfilecsv)
print ("Reading back: ",outfilecsv," took ", time.time() - t1," s")





print("Done.")
