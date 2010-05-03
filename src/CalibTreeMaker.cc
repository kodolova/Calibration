#include "Calibration/CalibTreeMaker/interface/CalibTreeMaker.h"
#include "DataFormats/JetReco/interface/TrackJet.h"
#include "DataFormats/JetReco/interface/JPTJet.h"


typedef CalibTreeMaker<reco::CaloJet> CalibTreeMakerCalo;
DEFINE_FWK_MODULE(CalibTreeMakerCalo);
typedef CalibTreeMaker<reco::PFJet> CalibTreeMakerPF;
DEFINE_FWK_MODULE(CalibTreeMakerPF);
typedef CalibTreeMaker<reco::TrackJet> CalibTreeMakerTrack;
DEFINE_FWK_MODULE(CalibTreeMakerTrack);
typedef CalibTreeMaker<reco::JPTJet> CalibTreeMakerJPT;
DEFINE_FWK_MODULE(CalibTreeMakerJPT);
