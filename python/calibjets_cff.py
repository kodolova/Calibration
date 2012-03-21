from RecoJets.Configuration.RecoJets_cff import *
from RecoJets.Configuration.RecoPFJets_cff import *
from CommonTools.ParticleFlow.pfNoPileUp_cff import *
from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector


kt6CaloJets.doRhoFastjet = True
kt6CaloJets.doAreaFastjet = True
#kt6CaloJets.voronoiRfact = 0.9
ak5CaloJets.doAreaFastjet = True
ak7CaloJets.doAreaFastjet = True

kt6PFJets.doRhoFastjet = True
kt6PFJets.doAreaFastjet = True
#kt6PFJets.voronoiRfact = 0.9
ak5PFJets.doAreaFastjet = True
ak7PFJets.doAreaFastjet = True

#CHS
goodOfflinePrimaryVertices = cms.EDFilter(
    "PrimaryVertexObjectFilter",
    filterParams = pvSelector.clone( minNdof = cms.double(4.0), maxZ = cms.double(24.0) ),
    src=cms.InputTag('offlinePrimaryVertices')
)

pfPileUp.Vertices = 'goodOfflinePrimaryVertices'
pfPileUp.checkClosestZVertex = cms.bool(False)

ak5PFCHSJets = ak5PFJets.clone(
    src = 'pfNoPileUp'
)
kt6PFCHSJets = kt6PFJets.clone(
    src = 'pfNoPileUp'
)


calibjets = cms.Sequence(recoJets * recoPFJets * goodOfflinePrimaryVertices 
                         * pfNoPileUpSequence * ak5PFCHSJets)
 