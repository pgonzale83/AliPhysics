AliAnalysisTaskSE *AddTaskSigma0Run2(bool isMC = false, bool isHeavyIon = false,
                                     TString trigger = "kINT7",
                                     const char *cutVariation = "0") {
  TString suffix;
  suffix.Form("%s", cutVariation);

  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    Error("AddTaskSigma0Run2()", "No analysis manager found.");
    return 0x0;
  }

  // ================== GetInputEventHandler =============================
  AliVEventHandler *inputHandler = mgr->GetInputEventHandler();

  AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer();

  //=========  Set Cutnumber for V0Reader ================================
  TString cutnumberPhoton;
  cutnumberPhoton = "00200008400000002280920000";
  if (suffix == "7") {
    cutnumberPhoton = "10200008400000002280920000";
  }
  TString cutnumberEvent = "00000000";
  TString periodNameV0Reader = "";
  Bool_t enableV0findingEffi = kFALSE;
  Bool_t runLightOutput = kFALSE;
  TString cutnumberAODBranch = "00000003_06000008400100001000000000";

  //========= Add V0 Reader to  ANALYSIS manager if not yet existent =====
  TString V0ReaderName =
      Form("V0ReaderV1_%s_%s", cutnumberEvent.Data(), cutnumberPhoton.Data());
  AliConvEventCuts *fEventCuts = NULL;

  if (!(AliV0ReaderV1 *)mgr->GetTask(V0ReaderName.Data())) {
    AliV0ReaderV1 *fV0ReaderV1 = new AliV0ReaderV1(V0ReaderName.Data());
    if (periodNameV0Reader.CompareTo("") != 0)
      fV0ReaderV1->SetPeriodName(periodNameV0Reader);
    fV0ReaderV1->SetUseOwnXYZCalculation(kTRUE);
    fV0ReaderV1->SetCreateAODs(kFALSE);  // AOD Output
    fV0ReaderV1->SetUseAODConversionPhoton(kTRUE);
    fV0ReaderV1->SetProduceV0FindingEfficiency(enableV0findingEffi);

    if (!mgr) {
      Error("AddTask_V0ReaderV1", "No analysis manager found.");
      return nullptr;
    }

    if (cutnumberEvent != "") {
      fEventCuts =
          new AliConvEventCuts(cutnumberEvent.Data(), cutnumberEvent.Data());
      fEventCuts->SetPreSelectionCutFlag(kTRUE);
      fEventCuts->SetV0ReaderName(V0ReaderName);
      if (periodNameV0Reader.CompareTo("") != 0)
        fEventCuts->SetPeriodEnum(periodNameV0Reader);
      fV0ReaderV1->SetEventCuts(fEventCuts);
    }

    // Set AnalysisCut Number
    AliConversionPhotonCuts *fCuts = NULL;
    if (cutnumberPhoton != "") {
      fCuts = new AliConversionPhotonCuts(cutnumberPhoton.Data(),
                                          cutnumberPhoton.Data());
      fCuts->SetPreSelectionCutFlag(kTRUE);
      fCuts->SetIsHeavyIon(isHeavyIon);
      fCuts->SetV0ReaderName(V0ReaderName);
      fCuts->SetLightOutput(runLightOutput);
      if (fCuts->InitializeCutsFromCutString(cutnumberPhoton.Data())) {
        fV0ReaderV1->SetConversionCuts(fCuts);
        fCuts->SetFillCutHistograms("", kTRUE);
      }
    }

    fV0ReaderV1->Init();

    AliLog::SetGlobalLogLevel(AliLog::kFatal);

    // connect input V0Reader
    mgr->AddTask(fV0ReaderV1);
    mgr->ConnectInput(fV0ReaderV1, 0, cinput);
  }

  //========= Init subtasks and start analyis ============================
  AliSigma0V0Cuts *v0Cuts = AliSigma0V0Cuts::LambdaCuts();
  v0Cuts->SetIsMC(isMC);
  v0Cuts->SetPID(3122);
  v0Cuts->SetPosPID(AliPID::kProton, 2212);
  v0Cuts->SetNegPID(AliPID::kPion, -211);
  AliSigma0V0Cuts *antiv0Cuts = AliSigma0V0Cuts::LambdaCuts();
  antiv0Cuts->SetIsMC(isMC);
  antiv0Cuts->SetPID(-3122);
  antiv0Cuts->SetPosPID(AliPID::kPion, 211);
  antiv0Cuts->SetNegPID(AliPID::kProton, -2212);

  if (suffix != "0") {
    v0Cuts->SetLightweight(true);
    antiv0Cuts->SetLightweight(true);
  }
  if (suffix == "1") {
    v0Cuts->SetV0OnFlyStatus(true);
    antiv0Cuts->SetV0OnFlyStatus(true);
  }
  if (suffix == "2") {
    v0Cuts->SetK0Rejection(0., 0.);
    antiv0Cuts->SetK0Rejection(0., 0.);
    v0Cuts->SetLambdaSelection(1.115683 - 0.008, 1.115683 + 0.008);
    antiv0Cuts->SetLambdaSelection(1.115683 - 0.008, 1.115683 + 0.008);
    v0Cuts->SetPileUpRejectionMode(AliSigma0V0Cuts::OneDaughterCombined);
    antiv0Cuts->SetPileUpRejectionMode(AliSigma0V0Cuts::OneDaughterCombined);
    v0Cuts->SetArmenterosCut(0.01, 0.12, 0.3, 0.95);
    antiv0Cuts->SetArmenterosCut(0.01, 0.12, 0.3, 0.95);
  }
  if (suffix == "3") {
    v0Cuts->SetK0Rejection(0., 0.);
    antiv0Cuts->SetK0Rejection(0., 0.);
    v0Cuts->SetLambdaSelection(1.115683 - 0.008, 1.115683 + 0.008);
    antiv0Cuts->SetLambdaSelection(1.115683 - 0.008, 1.115683 + 0.008);
    v0Cuts->SetArmenterosCut(0.01, 0.12, 0.3, 0.95);
    antiv0Cuts->SetArmenterosCut(0.01, 0.12, 0.3, 0.95);
  }

  if (suffix == "999") {
    v0Cuts->SetCheckCutsMC(true);
    antiv0Cuts->SetCheckCutsMC(true);
    v0Cuts->SetLightweight(false);
    antiv0Cuts->SetLightweight(false);
  }

  AliSigma0V0Cuts *photonV0Cuts = AliSigma0V0Cuts::PhotonCuts();
  photonV0Cuts->SetIsMC(isMC);
  photonV0Cuts->SetPID(22);
  photonV0Cuts->SetPosPID(AliPID::kElectron, 11);
  photonV0Cuts->SetNegPID(AliPID::kElectron, -11);
  if (suffix != "0") photonV0Cuts->SetLightweight(true);

  if (suffix == "999") {
    photonV0Cuts->SetCheckCutsMC(true);
    photonV0Cuts->SetLightweight(false);
  }

  AliSigma0PhotonMotherCuts *sigmaCuts =
      AliSigma0PhotonMotherCuts::DefaultCuts();
  sigmaCuts->SetIsMC(isMC);
  sigmaCuts->SetPDG(3212, 3122, 22);
  sigmaCuts->SetLambdaCuts(v0Cuts);
  sigmaCuts->SetV0ReaderName(V0ReaderName.Data());
  if (suffix != "0" && suffix != "999") {
    sigmaCuts->SetLightweight(true);
  }
  if (suffix == "4") {
    sigmaCuts->SetPhotonMaxPt(3);
  }
  if (suffix == "5") {
    sigmaCuts->SetPhotonMaxPt(5);
  }
  if (suffix == "6") {
    sigmaCuts->SetPhotonMaxPt(10);
  }

  AliSigma0PhotonMotherCuts *antiSigmaCuts =
      AliSigma0PhotonMotherCuts::DefaultCuts();
  antiSigmaCuts->SetIsMC(isMC);
  antiSigmaCuts->SetPDG(3212, 3122, 22);
  antiSigmaCuts->SetLambdaCuts(antiv0Cuts);
  antiSigmaCuts->SetV0ReaderName(V0ReaderName.Data());
  if (suffix != "0" && suffix != "999") {
    antiSigmaCuts->SetLightweight(true);
  }
  if (suffix == "4") {
    antiSigmaCuts->SetPhotonMaxPt(3);
  }
  if (suffix == "5") {
    antiSigmaCuts->SetPhotonMaxPt(5);
  }
  if (suffix == "6") {
    antiSigmaCuts->SetPhotonMaxPt(10);
  }

  AliSigma0PhotonMotherCuts *sigmaPhotonCuts =
      AliSigma0PhotonMotherCuts::DefaultCuts();
  sigmaPhotonCuts->SetIsMC(isMC);
  sigmaPhotonCuts->SetPDG(3212, 3122, 22);
  sigmaPhotonCuts->SetPhotonCuts(photonV0Cuts);
  sigmaPhotonCuts->SetLambdaCuts(v0Cuts);
  if (suffix != "0" && suffix != "999") {
    sigmaPhotonCuts->SetLightweight(true);
  }
  if (suffix == "4") {
    sigmaPhotonCuts->SetPhotonMaxPt(3);
  }
  if (suffix == "5") {
    sigmaPhotonCuts->SetPhotonMaxPt(5);
  }
  if (suffix == "6") {
    sigmaPhotonCuts->SetPhotonMaxPt(10);
  }

  AliSigma0PhotonMotherCuts *antiSigmaPhotonCuts =
      AliSigma0PhotonMotherCuts::DefaultCuts();
  antiSigmaPhotonCuts->SetIsMC(isMC);
  antiSigmaPhotonCuts->SetPDG(-3212, -3122, 22);
  antiSigmaPhotonCuts->SetPhotonCuts(photonV0Cuts);
  antiSigmaPhotonCuts->SetLambdaCuts(antiv0Cuts);
  if (suffix != "0" && suffix != "999") {
    antiSigmaPhotonCuts->SetLightweight(true);
  }
  if (suffix == "4") {
    antiSigmaPhotonCuts->SetPhotonMaxPt(3);
  }
  if (suffix == "5") {
    antiSigmaPhotonCuts->SetPhotonMaxPt(5);
  }
  if (suffix == "6") {
    antiSigmaPhotonCuts->SetPhotonMaxPt(10);
  }

  AliAnalysisTaskSigma0Run2 *task =
      new AliAnalysisTaskSigma0Run2("AnalysisTaskSigma0");
  if (trigger == "kINT7") {
    task->SetTrigger(AliVEvent::kINT7);
    task->SelectCollisionCandidates(AliVEvent::kINT7);
  } else if (trigger == "kHighMultV0") {
    task->SetTrigger(AliVEvent::kHighMultV0);
    task->SelectCollisionCandidates(AliVEvent::kHighMultV0);
  }
  task->SetV0ReaderName(V0ReaderName.Data());
  task->SetIsHeavyIon(isHeavyIon);
  task->SetIsMC(isMC);
  task->SetV0Cuts(v0Cuts);
  task->SetAntiV0Cuts(antiv0Cuts);
  task->SetPhotonV0Cuts(photonV0Cuts);
  task->SetSigmaCuts(sigmaCuts);
  task->SetAntiSigmaCuts(antiSigmaCuts);
  task->SetSigmaPhotonCuts(sigmaPhotonCuts);
  task->SetAntiSigmaPhotonCuts(antiSigmaPhotonCuts);

  if (suffix != "0" && suffix != "999") task->SetLightweight(true);

  mgr->AddTask(task);

  TString containerName = mgr->GetCommonFileName();
  containerName += ":AnalysisTaskSigma0_";
  if (trigger == "kHighMultV0") containerName += "HighMultV0_";
  containerName += suffix;

  TString name = "histo_";
  if (trigger == "kHighMultV0") name += "HighMultV0_";
  name += suffix;
  AliAnalysisDataContainer *cOutputList = mgr->CreateContainer(
      name, TList::Class(), AliAnalysisManager::kOutputContainer,
      containerName.Data());

  mgr->ConnectInput(task, 0, cinput);
  mgr->ConnectOutput(task, 1, cOutputList);

  return task;
}
