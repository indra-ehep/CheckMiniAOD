/**********************************************************************
 Created on : 09/11/2025
 Purpose    : Draw and fit the calibration plots
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

Double_t CrystallBall(Double_t *x, Double_t *par)
{
  //parameters
  //par[0] : norm
  //par[1] : mean
  //par[2] : sigma
  //par[3] : alpha
  //par[4] : n
  double N = par[0];
  double x0 = par[1];
  double sigma = par[2];
  double alpha = par[3];
  double n = par[4];
  
  double A = pow(n/fabs(alpha),n) * exp(-(alpha*alpha/2.));
  double B = n/fabs(alpha) - fabs(alpha);
  
  double nSig = (x[0] - x0)/sigma ;
  double ret = 0;
  if(nSig<=alpha)
    ret = N*exp(-0.5*nSig*nSig);
  else
    ret = N*A/pow((B+nSig),n);
  
  return ret;
}

int calcCalib()
{
  std::string infile = "root_files/calib.root";
  TFile *fin = TFile::Open(infile.c_str());

  int gindex = 0;
  const int nEbins = 50;
  const int nJetEtaBins = 6;
  const float maxE = 1000.0;
  double xmin = 0.0, xmax = 5.;
  Float_t jetEtaBin[7] = {1.321, 1.7, 2.0, 2.3, 2.6, 2.9, 3.152} ;
  TH1F *hEtaBin = new TH1F("hEtaBin","hEtaBin", nJetEtaBins, jetEtaBin);
  TH1F *hEBin = new TH1F("hEBin","hEBin", nEbins, 0., maxE);
  Color_t color[nJetEtaBins] = {kBlack, kRed, kBlue, kYellow+2, kGreen+2, kMagenta};

  TH1D *hGenClusEReso[nJetEtaBins][nEbins];
  double prevVal = -1;
  TGraph *grCalib[nJetEtaBins] ; //= new TGraph(nEbins);
  for(int ieta=0;ieta<nJetEtaBins;ieta++) {
    grCalib[ieta] = new TGraph(nEbins);
    for(int ipt=0;ipt<nEbins;ipt++){
      hGenClusEReso[ieta][ipt] = (TH1D *) fin->Get(Form("hGenClusEReso_%d_%d",ieta,ipt));
      float xval = hEBin->GetXaxis()->GetBinCenter(ipt+1);
      float yval = hGenClusEReso[ieta][ipt]->GetBinCenter(hGenClusEReso[ieta][ipt]->GetMaximumBin());
      float ymean = (hGenClusEReso[ieta][ipt]->GetEntries()==0)?prevVal:hGenClusEReso[ieta][ipt]->GetMean();
      double norm = hGenClusEReso[ieta][ipt]->GetBinContent(hGenClusEReso[ieta][ipt]->GetMaximumBin());
      // TF1 *fn = new TF1(Form("funch%d",gindex++),CrystallBall,xmin+1,xmax,5);
      // fn->SetParameters(norm, hGenClusEReso[ieta][ipt]->GetMean(), hGenClusEReso[ieta][ipt]->GetRMS(), 1.0, 5);
      // hGenClusEReso[ieta][ipt]->Fit(fn,"NQLR");
      //grCalib[ieta]->SetPoint(ipt, xval, hGenClusEReso[ieta][ipt]->GetMean());
      //grCalib[ieta]->SetPoint(ipt, xval, yval);
      //grCalib[ieta]->SetPoint(ipt, xval, fn->GetParameter(1));
      TF1 *fn = new TF1(Form("funch%d",gindex++),"gaus",xmin,xmax);
      fn->SetParameters(norm, ymean, hGenClusEReso[ieta][ipt]->GetRMS());
      // TF1 *fn = new TF1(Form("funch%d",gindex++),"landau",xmin,xmax);
      // fn->SetParameters(norm, ymean, hGenClusEReso[ieta][ipt]->GetRMS()/2);
      // TF1 *fn = new TF1(Form("funch%d",gindex++),CrystallBall,xmin+1,xmax,5);
      // fn->SetParameters(norm, ymean, hGenClusEReso[ieta][ipt]->GetRMS(), 1.0, 1);
      hGenClusEReso[ieta][ipt]->Fit(fn,"QLR");
      yval = (hGenClusEReso[ieta][ipt]->GetEntries()<50.0)?ymean:fn->GetParameter(1);
      grCalib[ieta]->SetPoint(ipt, xval, yval);
      prevVal = ymean;
      //delete fn;
    }
    grCalib[ieta]->SetLineColor(color[ieta]);
    grCalib[ieta]->SetLineWidth(2);
    grCalib[ieta]->SetMarkerColor(color[ieta]);
    grCalib[ieta]->SetMarkerStyle(kFullCircle);
    grCalib[ieta]->SetName(Form("graph_%d",ieta));
    grCalib[ieta]->SetTitle(Form("plot for #eta:(%2.1f-%2.1f)",jetEtaBin[ieta],jetEtaBin[ieta+1]));
  }


  TLegend *leg0 = new TLegend(0.19,0.25,0.58,0.42);
  leg0->SetTextSize(0.028);
  leg0->SetShadowColor(kWhite);
  leg0->SetHeader(Form("#eta ranges for energy calibration"));
  for(int ieta=0;ieta<nJetEtaBins;ieta++) leg0->AddEntry(grCalib[ieta],grCalib[ieta]->GetTitle(),"lp");

  grCalib[0]->SetMinimum(0);
  grCalib[0]->SetMaximum(10);
  std::string title0 = grCalib[0]->GetTitle();
  grCalib[0]->SetTitle("");
  TCanvas *c1 = new TCanvas("c1","c1",900,900);
  c1->SetTickx();
  c1->SetTicky();
  grCalib[0]->Draw("alp");
  grCalib[0]->GetXaxis()->SetTitle("E_{TICL} (GeV)");
  grCalib[0]->GetYaxis()->SetTitle("E_{genjet}/E_{TICL}");
  for(int ieta=1;ieta<nJetEtaBins;ieta++) grCalib[ieta]->Draw("lp");
  leg0->Draw();

  TFile *fout = new TFile("root_files/fitresults.root","recreate");
  c1->Write();
  grCalib[0]->SetTitle(title0.c_str());
  for(int ieta=0;ieta<nJetEtaBins;ieta++) grCalib[ieta]->Write();
  for(int ieta=0;ieta<nJetEtaBins;ieta++) for(int ipt=0;ipt<nEbins;ipt++) hGenClusEReso[ieta][ipt]->Write();
  fout->Close();
  delete fout;
  
  return true;
}
