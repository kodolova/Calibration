// -*- C++ -*-
//
// Package:    CalibTreeMaker
// Class:      CalibTreeMaker
// 
/**\class CalibTreeMaker CalibTreeMaker.cc Calibration/CalibTreeMaker/src/CalibTreeMaker.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Christian Autermann
//         Created:  Wed Jul 18 13:54:50 CEST 2007
// $Id: CalibTreeMaker.cc,v 1.10 2008/01/23 15:27:57 auterman Exp $
//
//

#include "Calibration/CalibTreeMaker/interface/CalibTreeMaker.h"

//
// constructors and destructor
//
CalibTreeMaker::CalibTreeMaker(const edm::ParameterSet& iConfig)
{
  _file=0;

  _HistName  = iConfig.getParameter< std::string > ("OutputFile");
  _TTTreeName  = iConfig.getParameter< std::string > ("TrackTowerTreeName");
  _GJTreeName  = iConfig.getParameter< std::string > ("GammaJetTreeName");
  _TCTreeName  = iConfig.getParameter< std::string > ("TrackClusterTreeName");
  _JJTreeName  = iConfig.getParameter< std::string > ("JetJetTreeName");


  met_       = iConfig.getParameter<edm::InputTag> ("met");
  alltowers_ = iConfig.getParameter<edm::InputTag> ("alltowers");

  //TrackTower
  track_     = iConfig.getParameter<edm::InputTag> ("track");
  tower_     = iConfig.getParameter<edm::InputTag> ("tower");
  //TrackCluster
  tc_track_  = iConfig.getParameter<edm::InputTag> ("tc_track");
  tc_tower_  = iConfig.getParameter<edm::InputTag> ("tc_tower");
  //JetJet
  jj_jets_    = iConfig.getParameter<edm::InputTag> ("jj_jets");
  jj_genjets_ = iConfig.getParameter<edm::InputTag> ("jj_genjets");

  //Steering Flags
  writeGammaJet_     = iConfig.getParameter<bool> ("writeGammaJet");
  writeTrackTower_   = iConfig.getParameter<bool> ("writeTrackTower");
  writeTrackCluster_ = iConfig.getParameter<bool> ("writeTrackCluster");
  writeJetJet_       = iConfig.getParameter<bool> ("writeJetJet");

  // open the tree file
  _file=new TFile(_HistName.c_str(),"RECREATE");
  _file->cd();

  // Initialize the tree
  if (writeGammaJet_)
    GammaJetTree   = new TTree(_GJTreeName.c_str(),"");
  if (writeTrackTower_)
    TrackTowerTree = new TTree(_TTTreeName.c_str(),"");
  if (writeTrackCluster_)
    TrackClusterTree = new TTree(_TCTreeName.c_str(),"");
  if (writeJetJet_)
    JetJetTree = new TTree(_JJTreeName.c_str(),"");
  
  //-> pass "CalibTree" and "iConfig" to other helper classes here!
  if (writeGammaJet_)
    gammajet_analysis_.setup(iConfig,   GammaJetTree);
  if (writeTrackTower_)
    tracktower_analysis_.setup(iConfig, TrackTowerTree);
  if (writeTrackCluster_)
    trackcluster_analysis_.setup(iConfig, TrackClusterTree);
  if (writeJetJet_)
    jetjet_analysis_.setup(iConfig, JetJetTree);
}


CalibTreeMaker::~CalibTreeMaker()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void CalibTreeMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::ESHandle<CaloGeometry> geometry;
  //iSetup.get<IdealGeometryRecord>().get(geometry);
  
  if (writeGammaJet_) {
    gammajet_analysis_.analyze(iEvent, iSetup, GammaJetTree);
  }
  if (writeTrackTower_) {
    edm::Handle<CaloTowerCollection> tower;
    edm::Handle<reco::TrackCollection> track;
    iEvent.getByLabel(tower_,tower);
    iEvent.getByLabel(track_,track);
    tracktower_analysis_.analyze(*track, *tower, TrackTowerTree);
  }
  if (writeTrackCluster_) {
    edm::Handle<CaloTowerCollection> tc_tower;
    edm::Handle<reco::TrackCollection> tc_track;
    iEvent.getByLabel(tc_tower_,tc_tower);
    iEvent.getByLabel(tc_track_,tc_track);
    trackcluster_analysis_.analyze(*tc_track, *tc_tower, TrackClusterTree);
  } 
  if (writeJetJet_) {
    edm::Handle<CaloTowerCollection> alltowers;
    edm::Handle<reco::CaloJetCollection> jj_jet;
    edm::Handle<reco::GenJetCollection> jj_genjet;
    edm::Handle<CaloMETCollection> met;
    iEvent.getByLabel(met_,met);
    iEvent.getByLabel(alltowers_,alltowers);
    iEvent.getByLabel(jj_jets_,jj_jet);
    iEvent.getByLabel(jj_genjets_,jj_genjet);
    jetjet_analysis_.analyze(*jj_jet, *jj_genjet, *met, *alltowers, 
                               JetJetTree);
  }
}


// ------------ method called once each job just before starting event loop  ------------
void CalibTreeMaker::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void CalibTreeMaker::endJob() {  
  _file->cd(); 
  if (writeGammaJet_){
    GammaJetTree->Write();
    delete GammaJetTree;
  }
  if (writeTrackTower_){
    TrackTowerTree->Write();
    delete TrackTowerTree;
  }
  if (writeTrackCluster_){
    TrackClusterTree->Write();
    delete TrackClusterTree;
  }
  if (writeJetJet_){
    JetJetTree->Write();
    delete JetJetTree;
  }
 
  if (_file!=0) {   // if there was a tree file...
    _file->Write(); // write out the branches
    _file->Close();
    delete _file;   // close and delete the file
    _file=0;        // set to zero to clean up
  }
}

//define this as a plug-in
DEFINE_FWK_MODULE(CalibTreeMaker);
