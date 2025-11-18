/**********************************************************************
 Created on : 08/11/2025
 Purpose    : plot jet resolution
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

int plotReso()
{
  //std::string infile = "root_files/test15_ticlresult_resop_eratio-gt-0p7_maxE1TeV_fullstat_deltaEtaDeltaPhi-0p1_hgcal-core-eta-1p7-2p8_calib.root";
  //std::string infile = "root_files/test15_ticlresult_resop_eratio-gt-0p7_maxE1TeV_deltaEtaDeltaPhi-0p1_hgcal-core-eta-1p7-2p9_calib.root";
  //std::string infile = "root_files/test15_ticlresult_resop_eratio-gt-0p9_maxE1TeV_deltaEtaDeltaPhi-0p1_hgcal-core-eta-1p7-2p9_newbin.root";
  //std::string infile = "root_files/test15_ticlresult_resop_eratio-gt-0p7_ticl-maxE500GeV_deltaEtaDeltaPhi-0p1_newbin.root";
  //std::string infile = "root_files/test15_ticlresult_resop_eratio-gt-0p9_ticl-maxE500GeV_deltaEtaDeltaPhi-0p1_newbin_hgcal-core-eta-2p0-2p3.root";
  //std::string infile = "root_files/test15_ticlresult_resop_eratio-gt-0p9_ticl-maxE500GeV_deltaEtaDeltaPhi-0p1_hgcal-core-eta-1p7-2p9_varEbin.root";
  //std::string infile = "root_files/test16_ticlresult_resop_eratio-gt-0p6_maxE1TeV_deltaEtaDeltaPhi-0p1_hgcal-core-eta-1p7-2p9_calib.root";
  //std::string infile = "root_files/test16_ticlresult_resop_eratio-gt-0p6_maxE1TeV_deltaEtaDeltaPhi-0p1_hgcal-core-eta-1p7-2p9_calib_fullstat.root";
  //std::string infile = "root_files/test16_ticlresult_resop_eratio-gt-0p9_maxE1TeV_deltaEtaDeltaPhi-0p1_hgcal-core-eta-1p7-2p9_calib_10bins.root";
  std::string infile = "root_files/test16_ticlresult_resop_eratio-gt-0p95_maxE1TeV_deltaEtaDeltaPhi-0p1_hgcal-core-eta-1p7-2p9_calib_10bins_fullstat.root";
    
  TFile *fin = TFile::Open(infile.c_str());

  //const int nEbins = 50;
  //const int nEbins = 20;
  const int nEbins = 10;
  const float maxE = 1000.0;
  TH1F *hEBin = new TH1F("hEBin","hEBin", nEbins, 0., maxE);
  // const float maxE = 330.0;
  // Float_t jetEBin[11] = {15.0, 25.0, 35.0, 65.0, 90.0, 110.0, 130.0, 170.0, 230.0, 270.0, 330.0} ;
  // TH1F *hEBin = new TH1F("hEBin","hEBin", nEbins, jetEBin);

  TH1D *hEResoP[nEbins], *hEDiff[nEbins] ;
  // TGraphErrors *grReso = new TGraphErrors(nEbins-1);
  // TGraphErrors *grResoL = new TGraphErrors(nEbins-1);
  TGraphErrors *grReso = new TGraphErrors(nEbins);
  TGraphErrors *grResoL = new TGraphErrors(nEbins);
  for(int ibin=0;ibin<nEbins;ibin++){
    hEResoP[ibin] = (TH1D *) fin->Get(Form("hEResoP_%d",ibin)); //new TH1D(, Form("hEResoP_%d (%4.1f GeV)",ibin,hEBin->GetXaxis()->GetBinCenter(ibin+1)), 100, 0.0, 100.0);
    hEDiff[ibin] = (TH1D *) fin->Get(Form("hEDiff_%d",ibin));   // new TH1D(Form("hEDiff_%d",ibin), Form("hEDiff_%d (%4.1f GeV)",ibin,hEBin->GetXaxis()->GetBinCenter(ibin+1)), 200, -10.0, 190.0);
    float xval = hEBin->GetXaxis()->GetBinCenter(ibin+1);
    
    // grReso->SetPoint(ibin-1, xval, hEResoP[ibin]->GetRMS());
    // grReso->SetPointError(ibin-1, 0, hEResoP[ibin]->GetRMSError());    
    // grResoL->SetPoint(ibin-1, 1./TMath::Sqrt(xval), hEResoP[ibin]->GetRMS());
    // grResoL->SetPointError(ibin-1, 0, hEResoP[ibin]->GetRMSError());
    
    // grReso->SetPoint(ibin, xval, hEResoP[ibin]->GetRMS());
    // grReso->SetPointError(ibin, 0, hEResoP[ibin]->GetRMSError());    
    // grResoL->SetPoint(ibin, 1./TMath::Sqrt(xval), hEResoP[ibin]->GetRMS());
    // grResoL->SetPointError(ibin, 0, hEResoP[ibin]->GetRMSError());

    float yval = 100.*hEDiff[ibin]->GetRMS()/xval;
    float yvalerror = 100.*hEDiff[ibin]->GetRMSError()/xval;
    grReso->SetPoint(ibin, xval, yval);
    grReso->SetPointError(ibin, 0, yvalerror);    
    grResoL->SetPoint(ibin, 1./TMath::Sqrt(xval), yval);
    grResoL->SetPointError(ibin, 0, yvalerror);

    std::cout << "Eval: " << xval << ", 1/sqrt(E) : " << 1./TMath::Sqrt(xval) << std::endl;
  }

  float reco2018x[10] = {20., 30., 50., 80., 100., 120., 150., 200., 245., 300.}; 
  float reco2018y[10] = {4.85, 3.98, 3.15, 2.55, 2.3, 2.05, 1.9, 1.6, 1.5, 1.4};
  TGraph *grReso2018 = new TGraph(10,reco2018x,reco2018y);
  TGraph *grReso2018L = new TGraph(10);
  for(int ibin=0;ibin<10;ibin++){
    grReso2018L->SetPoint(ibin, 1./TMath::Sqrt(reco2018x[ibin]), reco2018y[ibin]);
  }

  grReso2018L->SetMarkerStyle(kFullCircle);
  TCanvas *c4 = new TCanvas("c4","c4");
  c4->SetTickx();
  c4->SetTicky();
  grReso2018L->Draw("AP");

  grReso2018->SetMarkerStyle(kFullCircle);
  TCanvas *c3 = new TCanvas("c3","c3");
  c3->SetTickx();
  c3->SetTicky();
  grReso2018->Draw("AP");

  grResoL->SetMarkerStyle(kFullCircle);
  TCanvas *c2 = new TCanvas("c2","c2");
  c2->SetTickx();
  c2->SetTicky();
  grResoL->Draw("AP");
  
  gStyle->SetOptStat(kFALSE);
  gStyle->SetPalette(1);
  gROOT->SetStyle("Pub");
  
  TF1 *fnReso =  new TF1("fnReso","[0] + [1]/sqrt(x)",0,1040);
  fnReso->SetLineColor(kRed);
  fnReso->SetLineWidth(3);
  fnReso->SetParameters(0.6,100.0);
  grReso->SetMinimum(0);
  grReso->SetMaximum(7);
  grReso->SetTitle("");
  grReso->SetMarkerStyle(kOpenCircle);
  grReso->SetMarkerSize(2);
  TCanvas *c1 = new TCanvas("c1","c1",900,900);
  c1->SetTickx();
  c1->SetTicky();
  grReso->Draw("AP");
  grReso->GetYaxis()->SetTitle("Energy resolution (\%)");
  grReso->GetXaxis()->SetTitle("Electron energy (GeV)");
  grReso->GetYaxis()->SetTitleSize(0.03);
  grReso->GetXaxis()->SetTitleSize(0.03);
  grReso->GetYaxis()->SetLabelSize(0.03);
  grReso->GetXaxis()->SetLabelSize(0.03);
  //fnReso->Draw("sames");
  grReso->Fit(fnReso,"LR");
  
  TLatex *texl = new TLatex(0.5,0.5,"CMS");
  texl->SetTextSize(0.035);
  TLatex *texp = new TLatex(0.5,0.5,"Simulation");
  texp->SetTextSize(0.025);
  texp->SetTextFont(52);
  TLatex *texq = new TLatex(0.5,0.5,"(private work)");
  texq->SetTextSize(0.025);
  TLatex *texr = new TLatex(0.5,0.5,"#it{e}^{-} particle gun");
  texr->SetTextSize(0.025);

  TLegend *leg0 = new TLegend(0.4621381,0.6289954,0.8652561,0.8573059);
  leg0->SetBorderSize(0);
  leg0->SetTextSize(0.028);
  leg0->SetShadowColor(kWhite);
  //leg0->SetHeader(Form("Energy resolution"));
  leg0->AddEntry(grReso,"MC","lp");
  leg0->AddEntry(fnReso," #frac{#sigma_{E}}{#LTE#GT} = #frac{S}{#sqrt{E}} #oplus C","lp");
  leg0->AddEntry((TObject *)0x0,Form("S = (%2.1f #pm %2.1f)\% [#sqrt\{GeV\}]",fnReso->GetParameter(1),fnReso->GetParError(1)),"");
  leg0->AddEntry((TObject *)NULL,Form("C = (%2.1f #pm %2.1f)\%",fnReso->GetParameter(0),fnReso->GetParError(0)),"");
  leg0->Draw();
  texl->Draw("same");
  texp->Draw("same");
  texq->Draw("same");
  texr->Draw("same");

  return true;
}
