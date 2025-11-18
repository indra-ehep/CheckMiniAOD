{
   gSystem->Load("libFWCoreFWLite.so"); 
   FWLiteEnabler::enable();
   gSystem->Load("libDataFormatsFWLite.so");
   gSystem->Load("libDataFormatsPatCandidates.so");
}
