/**********************************************************************
 Created on : 09/11/2025
 Purpose    : Energy calibration of the energy measured by TICL with respect to the genjet energy
 Author     : Indranil Das, Research Associate, Imperial
 Email      : indranil.das@cern.ch | indra.ehep@gmail.com
**********************************************************************/

#if !defined(__CINT__) && !defined(__MAKECINT__)
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"

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

void energyCalib(int index = 0, const char *infile = "/home/indra/temp/a532f04b-e2a8-4e53-b079-6e06945eb6b0.root") {
  
  cout << "Reading file1 " << infile << endl ;
  TFile *file = TFile::Open(Form("%s",infile));
  cout << "Reading file " << file->GetName() << endl ;
  fwlite::Event ev(file);
  
  // ifstream fin("flist.txt");
  // string s;
  // vector<string> fileNames;
  // while(getline(fin,s)){
  //   //cout << s << endl;
  //   std::string sname = "root://eoscms.cern.ch/" + s ;
  //   fileNames.push_back(sname);
  // }
  // fin.close();
  // fwlite::ChainEvent ev(fileNames);  
  
  TH1F *hGenJetE = new TH1F("hGenJetE","hGenJetE", 11, -50.0, 1050.0);
  TH1F *hPtGenJet = new TH1F("hPtGenJet","hPtGenJet", 1000, 0., 1000.);
  TH1F *hEtaGenJet = new TH1F("hEtaGenJet","hEtaGenJet", 100, -5., 5.);
  TH1F *hPhiGenJet = new TH1F("hPhiGenJet","hPhiGenJet", 100, -5., 5.);

  TH1F *hPtRecoJet = new TH1F("hPtRecoJet","hPtRecoJet", 1000, 0., 1000.);
  TH1F *hEtaRecoJet = new TH1F("hEtaRecoJet","hEtaRecoJet", 100, -5., 5.);
  TH1F *hPhiRecoJet = new TH1F("hPhiRecoJet","hPhiRecoJet", 100, -5., 5.);

  TH1F *hPtOutLierJet = new TH1F("hPtOutLierJet","hPtOutLierJet", 1000, 0., 1000.);
  TH1F *hEtaOutLierJet = new TH1F("hEtaOutLierJet","hEtaOutLierJet", 100, -5., 5.);
  TH1F *hPhiOutLierJet = new TH1F("hPhiOutLierJet","hPhiOutLierJet", 100, -5., 5.);

  TH1F *hPtExptdJet = new TH1F("hPtExptdJet","hPtExptdJet", 1000, 0., 1000.);

  TH1D *hERecoByGen = new TH1D("hERecoByGen","hERecoByGen", 100, 0.0, 4.0);
  TH1D *hERecoGenDiff = new TH1D("hERecoGenDiff","hERecoGenDiff", 100, -100.0, 500.0);

  TH1D *hPtRecoByGen = new TH1D("hPtRecoByGen","hPtRecoByGen", 100, 0.0, 4.0);
  TH1D *hPtRecoGenDiff = new TH1D("hPtRecoGenDiff","hPtRecoGenDiff", 100, -100.0, 500.0);

  TH1D *hETICLByGen = new TH1D("hETICLByGen","hETICLByGen", 100, -0.5, 1.5);
  TH1D *hETICLGenDiff = new TH1D("hETICLGenDiff","hETICLGenDiff", 1000, -500.0, 500.0);
  
  const int nEbins = 50;
  const float maxE = 1000.0;
  float etaMinCore = 1.7, etaMaxCore = 2.8;
  TH1F *hEBin = new TH1F("hEBin","hEBin", nEbins, 0., maxE);
  TH1D *hEResoP[nEbins], *hEDiff[nEbins] ;
  for(int ibin=0;ibin<nEbins;ibin++){
    hEResoP[ibin] = new TH1D(Form("hEResoP_%d",ibin), Form("hEResoP_%d (%4.1f GeV)",ibin,hEBin->GetXaxis()->GetBinCenter(ibin+1)), 100, 0.0, 100.0);
    hEDiff[ibin] = new TH1D(Form("hEDiff_%d",ibin), Form("hEDiff_%d (%4.1f GeV)",ibin,hEBin->GetXaxis()->GetBinCenter(ibin+1)), 200, -10.0, 190.0);
  }

  const int nJetEtaBins = 6;
  Float_t jetEtaBin[7] = {1.321, 1.7, 2.0, 2.3, 2.6, 2.9, 3.152} ;
  TH1F *hEtaBin = new TH1F("hEtaBin","hEtaBin", nJetEtaBins, jetEtaBin);
  int nPtResoBins = 400;
  //////////////// 2D pt,eta histos ////////////////////////////
  TH1F ***hGenClusEReso = 0;
  hGenClusEReso = new TH1F**[nJetEtaBins];
  for(int ieta=0;ieta<nJetEtaBins;ieta++){
    hGenClusEReso[ieta] = new TH1F*[nEbins];
    for(int ipt=0;ipt<nEbins;ipt++){
      hGenClusEReso[ieta][ipt] = new TH1F(Form("hGenClusEReso_%d_%d",ieta,ipt),
					   Form("hGenClusEReso #eta:(%4.3f-%4.3f) p_{T}:(%2.0f) GeV",jetEtaBin[ieta],jetEtaBin[ieta+1],hEBin->GetXaxis()->GetBinCenter(ipt+1)),
					   nPtResoBins,0.,40.);
    }// jet et
  }//jet eta
  //////////////////////////////////////////////////////////

  TString trigName;
  unsigned int ievent = 0;
  unsigned int noftrigger = 0;
  bool isTriggered = false;
  //for( ev.toBegin(); ! ev.atEnd() && ievent<15000 ; ++ev) {
  for( ev.toBegin(); ! ev.atEnd() ; ++ev) {

    //==================== Genjet ==============================
    fwlite::Handle<std::vector<reco::GenJet> > genjets;
    genjets.getByLabel(ev,"slimmedGenJets");
    for(unsigned int ip = 0 ; ip < genjets.ptr()->size() ; ip++){
      hGenJetE->Fill((*genjets)[ip].energy());
      hPtGenJet->Fill((*genjets)[ip].pt());
      hEtaGenJet->Fill((*genjets)[ip].eta());
      hPhiGenJet->Fill((*genjets)[ip].phi());
    }
    std::cout <<"\n1 : "<<ievent <<", gen jet size "<<genjets.ptr()->size() <<std::endl;      
    for(unsigned int ip = 0 ; ip < genjets.ptr()->size() ; ip++)
      std::cout << "igen: " << ip << ", (pt,eta,phi,e) : (" << (*genjets)[ip].pt() << ", " << (*genjets)[ip].eta() << ", " << (*genjets)[ip].phi()<< ", " << (*genjets)[ip].energy() << ")" << std::endl;

    //====================== Reco ===============================
    fwlite::Handle<std::vector<pat::Jet> > recojets;
    recojets.getByLabel(ev,"slimmedJets");
    
    // if(ievent%1000==0){
    //   std::cout <<"2 : "<<ievent <<", jet size " << recojets.ptr()->size() <<std::endl;      
    // }
    
    std::cout <<"2 : "<<ievent <<", reco jet size "<<recojets.ptr()->size() <<std::endl;      
    for(unsigned int ip = 0 ; ip < recojets.ptr()->size() ; ip++)
      std::cout << "ireco: " << ip << ", (pt,eta,phi,e) : (" << (*recojets)[ip].pt() << ", " << (*recojets)[ip].eta() << ", " << (*recojets)[ip].phi() << ", " << (*recojets)[ip].energy() << ")" << std::endl;

    for(unsigned int ip = 0 ; ip < recojets.ptr()->size() ; ip++){
      hPtRecoJet->Fill((*recojets)[ip].pt());
      hEtaRecoJet->Fill((*recojets)[ip].eta());
      hPhiRecoJet->Fill((*recojets)[ip].phi());
    }
    // std::cout <<"3 : "<<ievent <<", jet size "<<recojets.ptr()->size() <<std::endl;      
    //(recojets.ptr())->clear();

    //====================== TICL ================================
    fwlite::Handle<vector<ticl::Trackster> > ticljets;
    ticljets.getByLabel(ev,"ticlTrackstersMerge");
    std::cout <<"3 : "<<ievent <<", ticl jet size "<<ticljets.ptr()->size() <<std::endl;
    
    for(unsigned int ip = 0 ; ip < ticljets.ptr()->size() ; ip++)
      std::cout << "iticl: " << ip << ", (raw_e,raw_em_e,phi,eta,phi,regressed_e,seedindex) : (" << (*ticljets)[ip].raw_energy()
		<< ", " << (*ticljets)[ip].raw_em_energy()
		<< ", " << (*ticljets)[ip].barycenter().eta()
		<< ", " << (*ticljets)[ip].barycenter().phi()
		<< ", " << (*ticljets)[ip].regressed_energy()
		<< ", " << (*ticljets)[ip].seedIndex()
	//<< ", " << (*ticljets)[ip].phi()
		<< ")"
		<< std::endl;
    
    for(unsigned int ipg = 0 ; ipg < genjets.ptr()->size() ; ipg++){
      double totE = 0;
      double ticlEta = -1; double prevMax = -1;
       for(unsigned int ipr = 0 ; ipr < ticljets.ptr()->size() ; ipr++){
	double etadiff = TMath::Abs( (*genjets)[ipg].eta() - (*ticljets)[ipr].barycenter().eta() );
	double phidiff = TMath::Abs( (*genjets)[ipg].phi() - (*ticljets)[ipr].barycenter().phi() );
	double delEtaDelPhi = TMath::Sqrt(etadiff*etadiff + phidiff*phidiff);
	if(delEtaDelPhi<0.1) {
	  totE += (*ticljets)[ipr].regressed_energy();
	  if((*ticljets)[ipr].regressed_energy() > prevMax){
	    ticlEta = (*ticljets)[ipr].barycenter().eta() ;
	    prevMax = (*ticljets)[ipr].regressed_energy();
	  }//pick eta of largest cluster
	}//deltaEtaDeltaPhi condn
       }//ticl array loop
       
       //std::cout << "igen: " << ipg << ", (e,totE) : (" << (*genjets)[ipg].energy()<< ", " << totE << ")" << std::endl;
       double eratio = (totE/(*genjets)[ipg].energy()) ;
       double ediff = ((*genjets)[ipg].energy() - totE) ;
       hETICLByGen->Fill( eratio );
       ///// Fill Calibration histos //////////////
       if( totE<maxE ){
	 int iptbin = hEBin->GetXaxis()->FindBin(totE) - 1;
	 int ietabin = -1;
	 if(ticlEta <= hEtaBin->GetXaxis()->GetBinCenter(1))
	   ietabin = 1;
	 else if(ticlEta >= hEtaBin->GetXaxis()->GetBinCenter(hEtaBin->GetNbinsX()))
	   ietabin = hEtaBin->GetNbinsX();
	 else
	   ietabin = hEtaBin->GetXaxis()->FindBin(TMath::Abs(ticlEta));
	 ietabin -= 1;
	 cout << "totE: " << totE << ", ticlEta: " << ticlEta << std::endl;
	 cout << "ietabin: " << ietabin << ", iptbin: " << iptbin << std::endl;
	 hGenClusEReso[ietabin][iptbin]->Fill(1./eratio);
       }
       ////////////////////////////////////////////
       ////////////////
       if( (*genjets)[ipg].energy()<maxE and (TMath::Abs((*genjets)[ipg].eta())>etaMinCore and TMath::Abs((*genjets)[ipg].eta())<etaMaxCore) ){
       	 hETICLGenDiff->Fill( ediff  );
       	 int ibin = hEBin->GetXaxis()->FindBin((*genjets)[ipg].energy()) - 1;
	 if(eratio>0.9){
	   hEResoP[ibin]->Fill( ediff*100./(*genjets)[ipg].energy() );
	   hEDiff[ibin]->Fill( ediff );
	 }
       }
       ///////////////
    }
    // for(unsigned int ipg = 0 ; ipg < genjets.ptr()->size() ; ipg++){
    //   for(unsigned int ipr = 0 ; ipr < recojets.ptr()->size() ; ipr++){
    // 	double etadiff = TMath::Abs( (*genjets)[ipg].eta() - (*recojets)[ipr].eta() );
    // 	double phidiff = TMath::Abs( (*genjets)[ipg].phi() - (*recojets)[ipr].phi() );
    // 	double delEtaDelPhi = TMath::Sqrt(etadiff*etadiff + phidiff*phidiff);
    // 	if(
    // 	   // etadiff<0.05
    // 	   // and
    // 	   // phidiff<0.05
    // 	   // and
    // 	   delEtaDelPhi<0.05
    // 	   and
    // 	   ((*genjets)[ipg].energy()>=75.0 and (*genjets)[ipg].energy()<=125.0)
    // 	   and
    // 	   (TMath::Abs((*genjets)[ipg].eta())>1.6 and TMath::Abs((*genjets)[ipg].eta())<1.8)
    // 	   // and
    // 	   // (*genjets)[ipg].pt()>=20.0
    // 	   ){
    // 	  double eratio = ( (*recojets)[ipr].energy() / (*genjets)[ipg].energy() );
    // 	  double ptratio = ( (*recojets)[ipr].pt() / (*genjets)[ipg].pt() );
	  
    // 	  hERecoGenDiff->Fill( ( (*recojets)[ipr].energy() - (*genjets)[ipg].energy() )  );
    // 	  hERecoByGen->Fill( eratio  );
	  
    // 	  hPtRecoGenDiff->Fill( ( (*recojets)[ipr].energy() - (*genjets)[ipg].energy() )  );
    // 	  hPtRecoByGen->Fill( eratio  );

    // 	  if(eratio>1.5){
    // 	    hPtOutLierJet->Fill( (*genjets)[ipg].pt());
    // 	    hEtaOutLierJet->Fill( (*genjets)[ipg].eta());
    // 	    hPhiOutLierJet->Fill( (*genjets)[ipg].phi());
    // 	  }else{
    // 	    hPtExptdJet->Fill( (*genjets)[ipg].pt());
    // 	  }
	  
    // 	}
    //   }
    // }
    
    ievent++;
  }
  cout <<"Total number of events processsed : " << ievent << endl;
  
  //fileNames.clear();

  TFile *fout = TFile::Open(Form("outfile_%d.root",index),"recreate");
  hGenJetE->Write();
  hPtGenJet->Write();
  hEtaGenJet->Write();
  hPhiGenJet->Write();
  hPtRecoJet->Write();
  hEtaRecoJet->Write();
  hPhiRecoJet->Write();
  hERecoGenDiff->Write();
  hERecoByGen->Write();
  hPtOutLierJet->Write();
  hEtaOutLierJet->Write();
  hPhiOutLierJet->Write();
  hPtExptdJet->Write();
  hPtRecoGenDiff->Write();
  hPtRecoByGen->Write();
  hETICLGenDiff->Write();
  hETICLByGen->Write();
  for(int ibin=0;ibin<nEbins;ibin++) hEResoP[ibin]->Write();
  for(int ibin=0;ibin<nEbins;ibin++) hEDiff[ibin]->Write();
  for(int ieta=0;ieta<nJetEtaBins;ieta++) for(int ipt=0;ipt<nEbins;ipt++) hGenClusEReso[ieta][ipt]->Write();
  fout->Close();
  delete fout;
  
  return;
}
