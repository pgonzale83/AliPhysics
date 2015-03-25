/**
 * \file AliEMCalTriggerEventCounterAnalysisComponent.h
 * \brief
 *
 * \author Markus Fasel <markus.fasel@cern.ch>, Lawrence Berkeley National Laboratory
 * \date Dec. 12, 2014
 */
#ifndef ALIEMCALTRIGGEREVENTCOUNTERANALYSISCOMPONENT_H
#define ALIEMCALTRIGGEREVENTCOUNTERANALYSISCOMPONENT_H
/* Copyright(c) 1998-2014, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */


#include "AliEMCalTriggerTracksAnalysisComponent.h"
#include "AliEMCalTriggerAnaTriggerDecision.h"

/**
 * \namespace EMCalTriggerPtAnalysis
 * \brief Analysis of high-p_{t} tracks in triggered events
 *
 * This namespace contains classes for the analysis of high-p_{t} tracks in
 * triggered events.
 */
namespace EMCalTriggerPtAnalysis {

class AliEMCalTriggerEventData;

/**
 * \class AliEMCalTriggerEventCounterAnalysisComponent
 * \brief Event counter analysis component for the trigger analysis
 *
 * Analysis component counting events for different trigger classes. Task needs
 * to be grouped with a global event selection.
 */
class AliEMCalTriggerEventCounterAnalysisComponent: public AliEMCalTriggerTracksAnalysisComponent {
public:
  AliEMCalTriggerEventCounterAnalysisComponent();
  AliEMCalTriggerEventCounterAnalysisComponent(const char *name);
  virtual ~AliEMCalTriggerEventCounterAnalysisComponent() {}

  virtual void CreateHistos();
  virtual void Process(const AliEMCalTriggerEventData * const data);

  /**
   * Specify method to select triggered events
   * \param method Method applied
   */
  void SetTriggerMethod(ETriggerMethod_t method) { fTriggerMethod = method; }

protected:
  void DefineAxis(TAxis& axis, const char* name,
      const char* title, int nbins, double min, double max,
      const char** labels) const;

  ETriggerMethod_t                          fTriggerMethod;     ///< Use patches for trigger decision

  /// \cond CLASSIMP
  ClassDef(AliEMCalTriggerEventCounterAnalysisComponent, 1);    // Analysis component for event counting
  /// \endcond
};

} /* namespace EMCalTriggerPtAnalysis */

#endif /* ALIEMCALTRIGGEREVENTCOUNTERANALYSISCOMPONENT_H */
