/// \file AddTaskNucleiYield.C
/// \brief Simple macro to add the task to a grid job
///
/// The task is here added several times to analyse different particle species and to investigate
/// different set of cuts in only one run.
///
/// \author Maximiliano Puccio <maximiliano.puccio@cern.ch>, University and INFN Torino
/// \date Feb 19th, 2015

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Rtypes.h>
#include <TString.h>
#include "AliAnalysisTaskNucleiYield.h"
#include "AliAnalysisManager.h"
#include "AliAnalysisDataContainer.h"
#include "AliPID.h"
#endif

AliAnalysisTaskNucleiYieldESD* AddTaskNucleiYieldESD(Bool_t isMC = kFALSE,
    AliPID::EParticleType part = AliPID::kDeuteron,
    TString tskname = "",
    TString suffix = "") {

  // Get the current analysis manager
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    Error("AddTaskNucleiYieldESD", "No analysis manager found.");
    return 0x0;
  }

  // Check the analysis type using the event handlers connected to the analysis manager.
  if (!mgr->GetInputEventHandler()) {
    ::Error("AddTaskNucleiYieldESD", "This task requires an input event handler");
    return 0x0;
  }

  // Common variables

  tskname.Append(Form("%s",suffix.Data()));
  AliAnalysisTaskNucleiYieldESD *deu = new AliAnalysisTaskNucleiYieldESD(tskname);
  deu->SetParticleType(part);
  deu->SetIsMC(isMC);

  float centBins[11] = {0.f,5.f,10.f,20.f,30.f,40.f,50.f,60.f,70.f,80.f,90.f};
  deu->SetCentBins(10, centBins);
 
  float pt[36] = {0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1, 1.1, 1.2, 1.3, 
    1.4, 1.50, 1.6, 1.70, 1.8, 1.90, 2, 2.2, 2.4, 2.6, 
    2.8, 3.00, 3.2, 3.40, 3.6, 3.80, 4, 4.5, 5.0, 5.5, 
    6.0, 6.50, 7.0, 8.00, 9.0, 10.0
  };
  deu->SetPtBins(35,pt);
  
  float dcabins[32] = {
    -0.50,-0.40,-0.30,-0.20,-0.15,-0.12,-0.10,-0.09,-0.08,-0.07,
    -0.06,-0.05,-0.04,-0.03,-0.02,-0.01, 0.00, 0.01, 0.02, 0.03,
    0.04, 0.05, 0.06, 0.07, 0.09, 0.10, 0.12, 0.15, 0.20, 0.30,
    0.40, 0.50
  };
  deu->SetDCABins(31,dcabins);
  
  mgr->AddTask(deu);
  TString output = "AnalysisResults.root";
  AliAnalysisDataContainer *deuCont = mgr->CreateContainer(Form("mpuccio_%s",tskname.Data()),
      TList::Class(),
      AliAnalysisManager::kOutputContainer,
      output.Data());
  mgr->ConnectInput  (deu,  0, mgr->GetCommonInputContainer());
  mgr->ConnectOutput (deu,  1, deuCont);
  return deu;
}

