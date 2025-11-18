#if !defined(__CINT__) && !defined(__MAKECINT__)
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"

// #include "FWCore/Framework/interface/Frameworkfwd.h"
// #include "FWCore/Framework/interface/EDAnalyzer.h"
// #include "FWCore/Framework/interface/Event.h"
// #include "FWCore/Framework/interface/MakerMacros.h"
// #include "FWCore/ParameterSet/interface/ParameterSet.h"

// #include "DataFormats/Math/interface/deltaR.h"
// #include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
//#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"

//Headers for the data items
//...
#include <fstream>
#include <iostream>
#include <TH1F.h>
#include <TFile.h>

using namespace std;

#endif

void readJets(int index = 0, const char *infile = "WZ_TuneCUETP8M1_13TeV-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_14ABCCC3-72D7-E611-B6C2-002590D9D9DA.root") {
  // const char *infile1 = "WZ_TuneCUETP8M1_13TeV-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_14ABCCC3-72D7-E611-B6C2-002590D9D9DA.root";
  // const char *infile2 = "WZ_TuneCUETP8M1_13TeV-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_14ABCCC3-72D7-E611-B6C2-002590D9D9DA.root"
    ;
  
  //TFile file(Form("/eos/user/i/idas/Output/test/WZ/%s",infile));
  //TFile file(Form("/run/media/indra/DHEP_Storage_1/Data/MiniAOD/AODFiles/MiniAOD/WZ/%s",infile));
  //
  //
  // edmDumpEventContent root://cmsxrootd.fnal.gov//store/mc/RunIISummer16MiniAODv2/DY4JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/80000/3627436D-BEC8-E611-994B-0025905A60E4.root
  //
  //
  //
  cout << "Reading file1 " << infile << endl ;
  TFile *file = TFile::Open(Form("%s",infile));
  cout << "Reading file " << file->GetName() << endl ;
  fwlite::Event ev(file);
  
  // ifstream fin("files1.txt");
  // string s;
  // vector<string> fileNames;
  // while(getline(fin,s)){
  //   //cout << s << endl;
  //   fileNames.push_back(s);
  // }
  // fin.close();
  // fwlite::ChainEvent ev(fileNames);  
  
  TH1F *hCutFlowUS = new TH1F("hCutFlowUS","hCutFlowUS", 10, 0., 10.);
  TH1F *hPtJetRaw = new TH1F("hPtJetRaw","hPtJetRaw", 1000, 0., 1000.);
  TH1F *hEtaJetRaw = new TH1F("hEtaJetRaw","hEtaJetRaw", 100, -3., 3.);
  TH1F *hPhiJetRaw = new TH1F("hPhiJetRaw","hPhiJetRaw", 100, -5., 5.);

  TString trigName;
  unsigned int ievent = 0;
  unsigned int noftrigger = 0;
  bool isTriggered = false;
  //for( ev.toBegin(); ! ev.atEnd() && ievent<15000 ; ++ev) {
  for( ev.toBegin(); ! ev.atEnd() ; ++ev) {
    
    fwlite::Handle<edm::TriggerResults> triggerBits;
    triggerBits.getByLabel(ev,"TriggerResults","","HLT");
    // fwlite::Handle<pat::PackedTriggerPrescales> triggerPrescales;
    // triggerPrescales.getByLabel(ev, "patTrigger");
    // fwlite::Handle<std::vector<pat::TriggerObjectStandAlone> > triggerObjects;
    // triggerObjects.getByLabel(ev, "selectedPatTrigger");
    
    if(ievent%10000==0){
      std::cout <<"1 : "<<ievent <<", trigger size " << triggerBits->size() <<std::endl;      
    }
    
    
    isTriggered = false;
    //const edm::TriggerNames &names = ev.triggerNames(*triggerBits);
    for (unsigned int i = 0, n = triggerBits->size(); i < n; ++i) {
      trigName = ev.triggerNames(*triggerBits).triggerName(i);
      if((trigName.Contains("HLT_IsoMu24") || trigName.Contains("HLT_IsoTkMu24")) && triggerBits->accept(i)){
    	// std::cout << "\n == TRIGGER PATHS= " << std::endl;
    	// std::cout << "Event : " << ievent << ", Trigger " << names.triggerName(i) <<
    	//   // ", prescale " << triggerPrescales->getPrescaleForIndex(i) <<
    	//   ": " << (triggerBits->accept(i) ? "PASS" : "fail (or not run)")
    	// 	  << std::endl;
    	noftrigger++;
    	isTriggered = true;
    	break;
      }
    }
    //triggerObjects->clear()
    
    if(isTriggered)
      hCutFlowUS->Fill(1);

    fwlite::Handle<std::vector<pat::Jet> > objs;
    objs.getByLabel(ev,"slimmedJets");
    
    if(ievent%1000==0){
      std::cout <<"2 : "<<ievent <<", jet size " << objs.ptr()->size() <<std::endl;      
    }
    
    // std::cout <<"2 : "<<ievent <<", jet size "<<objs.ptr()->size() <<std::endl;      
    
    if(isTriggered){
      for(unsigned int ip = 0 ; ip < objs.ptr()->size() ; ip++){
	//if(((*objs)[ip])==0x0) continue;
	if((*objs)[ip].pt() > 17.0 and abs((*objs)[ip].eta()) < 4.0){
	  hPtJetRaw->Fill((*objs)[ip].pt());
	  hEtaJetRaw->Fill((*objs)[ip].eta());
	  hPhiJetRaw->Fill((*objs)[ip].phi());
	}
      }
    }
    // std::cout <<"3 : "<<ievent <<", jet size "<<objs.ptr()->size() <<std::endl;      
    
    //(objs.ptr())->clear();
    
    ievent++;
  }
  cout <<"Total number of events processsed : " << ievent << endl;
  cout <<"Total number of events triggered : " << noftrigger << endl;
  
  //fileNames.clear();

  TFile *fout = TFile::Open(Form("outfile_%d.root",index),"recreate");
  hCutFlowUS->Write();
  hPtJetRaw->Write();
  hEtaJetRaw->Write();
  hPhiJetRaw->Write();
  fout->Close();
  delete fout;
  
  return;
}
