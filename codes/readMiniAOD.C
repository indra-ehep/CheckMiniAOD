#if !defined(__CINT__) && !defined(__MAKECINT__)
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
//Headers for the data items
//...
#endif
void readMiniAOD() {
  TFile file("/eos/user/i/idas/Output/test/DY/DY4JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_120000_0C26B9DF-D3C8-E611-A9D1-0CC47A7452DA.root");
  //DY3JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_120000_C673D5DF-4DC6-E611-BBB0-008CFA1660F8.root
  //DY2JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_120000_1646F6A3-9EBE-E611-9E92-842B2B766242.root
  //DY4JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_120000_0C26B9DF-D3C8-E611-A9D1-0CC47A7452DA.root
  //
  //
  // edmDumpEventContent root://cmsxrootd.fnal.gov//store/mc/RunIISummer16MiniAODv2/DY4JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/80000/3627436D-BEC8-E611-994B-0025905A60E4.root
  //
  //
  fwlite::Event ev(&file);

  int ievent = 0;

  for( ev.toBegin(); ! ev.atEnd() && ievent<10 ; ++ev) {
    fwlite::Handle<std::vector<pat::PackedGenParticle> > objs;
    objs.getByLabel(ev,"packedGenParticles");
    //objs.getByLabel(ev,"externalLHEProducer");
    //now can access data
    std::cout <<" size "<<objs.ptr()->size()<<std::endl;

    for(unsigned int ip = 0 ; ip < objs.ptr()->size() ; ip++){
    //for(unsigned int ip = 0 ; ip < 10 ; ip++){
      std::cout<<" ip :  "<< ip << ", pdg : " << (*objs)[ip].pdgId() <<", status : " << (*objs)[ip].status() << ", mother : " << ((*objs)[ip].mother(0))->pdgId() << endl;
      //std::cout<<" ip :  "<< ip << ", pdg : " << (*objs)[ip].pdgId() << endl;
      //const Candidate * motherInPrunedCollection = (*objs)[ip].mother(0) ;
      // if(motherInPrunedCollection != nullptr && isAncestor( bMeson , motherInPrunedCollection)){
      // 	int j=0;
      // 	std::cout << "     PdgID: " << (*objs)[j].pdgId() << endl;
      // 	j++;
      // }
    }

    ievent++;
  }
}
