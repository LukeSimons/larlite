from colored_msg import warning, error, info
import ROOT
orig_level = ROOT.gErrorIgnoreLevel
ROOT.gErrorIgnoreLevel = ROOT.kFatal

libs = [ 'libLArLite_Analysis',
         'libLArLite_DataFormat',
         'libLArLite_LArUtil',
         'libLArLite_Base']
for l in libs:
    continue
    val = ROOT.gSystem.Load(l)
    if val < 0:
        warning('Skipping %s' % l)
    #else:
    #    info('%s %s' % (val,l))
ROOT.gErrorIgnoreLevel = orig_level

try:
    ROOT
    ROOT.larlite
    ROOT.larlite.larlite_base
    ROOT.larutil
    ROOT.larutil.Geometry
    ROOT.larlite.ana_processor
except AttributeError:
    error('No LArLite core library loaded!')
    raise
from ROOT import larlite,larutil


