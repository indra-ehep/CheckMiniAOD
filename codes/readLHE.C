#if !defined(__CINT__) && !defined(__MAKECINT__)
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
//Headers for the data items
//...
#endif
void readLHE() {

  //
  //
  // edmDumpEventContent root://cmsxrootd.fnal.gov//store/mc/RunIISummer16MiniAODv2/DY4JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/80000/3627436D-BEC8-E611-994B-0025905A60E4.root
  //
  //
  //

  const char *infile2 = "DY2JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_120000_1646F6A3-9EBE-E611-9E92-842B2B766242.root" ;
  const char *infile3 = "DY3JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_120000_C673D5DF-4DC6-E611-BBB0-008CFA1660F8.root";
  const char *infile4 = "DY4JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_120000_0C26B9DF-D3C8-E611-A9D1-0CC47A7452DA.root";

  TFile file(Form("/eos/user/i/idas/Output/test/DY/%s",infile4));

  fwlite::Event ev(&file);

  int ievent = 0;

  for( ev.toBegin(); ! ev.atEnd() && ievent<10 ; ++ev) {
    
    cout<<"Procesing Event : " << ievent << endl;
    
    fwlite::Handle<LHEEventProduct> objs;
    objs.getByLabel(ev,"externalLHEProducer");
    
    const LHEEventProduct* LHE = 0;

    if(objs.isValid()){
      
      LHE = objs.product();
      lhef::HEPEUP hepeup(LHE->hepeup());
      int nLHEPart = hepeup.NUP;
      cout <<" nLHEPart "<< nLHEPart << ", process id "<<hepeup.IDPRUP<<endl;
      for(size_t i=0; i< hepeup.IDUP.size(); i++){
	//See file SimDataFormats/GeneratorProducts/interface/LesHouches.h for details
	printf("\t ipart : %02zu, pdg : %d, status : %d, mother (%d,%d)\n",i,  hepeup.IDUP[i], hepeup.ISTUP[i], hepeup.MOTHUP[i].first,hepeup.MOTHUP[i].second );
      }
    }
    ievent++;
  }
  
  return true;
}
