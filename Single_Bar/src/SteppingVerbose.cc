//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file radioactivedecay/rdecay01/src/SteppingVerbose.cc
/// \brief Implementation of the SteppingVerbose class
//
//
// $Id: SteppingVerbose.cc 71654 2013-06-19 18:20:04Z maire $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingVerbose.hh"

#include "G4SteppingManager.hh"
#include "G4ParticleTypes.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "CerenkovAnalysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingVerbose::SteppingVerbose()
  : G4SteppingVerbose()
{
  G4VSteppingVerbose::SetSilent(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingVerbose::~SteppingVerbose()
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingVerbose::TrackingStarted()
{  
  CopyState();
  
  //G4int prec = G4cout.precision(3);
  
  //Step zero
  //
  // G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  // if(fTrack->GetDefinition()->GetParticleName() == "opticalphoton" && fTrack->GetVolume()->GetName() == "Acryl"){
  //   analysisManager->FillNtupleDColumn(3, 7, fTrack->GetPosition().x());
  //   analysisManager->FillNtupleDColumn(3, 8, -fTrack->GetPosition().y() + 250*mm);
  //   analysisManager->FillNtupleDColumn(3, 9, fTrack->GetPosition().z());
    // analysisManager->FillNtupleIColumn(3, 10, fTrack->GetTrackID());    
  //}
  // else{
  //   analysisManager->FillNtupleDColumn(3, 7, -1.);
  //   analysisManager->FillNtupleDColumn(3, 8, -1.);
  //   analysisManager->FillNtupleDColumn(3, 9, -1.);
  //   // analysisManager->FillNtupleIColumn(3, 10, fTrack->GetTrackID());        
  // }
  // if( verboseLevel > 0 ){
  //   G4cout << std::setw( 5) << "Step#"      << " "
  //          << std::setw( 6) << "X"          << "    "
  //          << std::setw( 6) << "Y"          << "    "  
  //          << std::setw( 6) << "Z"          << "    "
  //          << std::setw( 9) << "KineE"      << " "
  //          << std::setw( 9) << "dEStep"     << " "  
  //          << std::setw(10) << "StepLeng"  
  //          << std::setw(10) << "TrakLeng"
  //          << std::setw(10) << "Volume"     << "  "
  //          << std::setw(10) << "Process"    << G4endl;             

  //   G4cout << std::setw(5) << fTrack->GetCurrentStepNumber() << " "
  //       << std::setw(6) << G4BestUnit(fTrack->GetPosition().x(),"Length")
  //       << std::setw(6) << G4BestUnit(fTrack->GetPosition().y(),"Length")
  //       << std::setw(6) << G4BestUnit(fTrack->GetPosition().z(),"Length")
  //       << std::setw(6) << G4BestUnit(fTrack->GetKineticEnergy(),"Energy")
  //       << std::setw(6) << G4BestUnit(fStep->GetTotalEnergyDeposit(),"Energy")
  //       << std::setw(6) << G4BestUnit(fStep->GetStepLength(),"Length")
  //       << std::setw(6) << G4BestUnit(fTrack->GetTrackLength(),"Length")
  //       << std::setw(10) << fTrack->GetVolume()->GetName()
  //       << "   initStep" << G4endl;        
  // }
  // G4cout.precision(prec);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingVerbose::StepInfo()
{
  CopyState();
  
  //G4int prec = G4cout.precision(3);
  
  // if( verboseLevel >= 1 ){
  //   if( verboseLevel >= 4 ) VerboseTrack();
  //   if( verboseLevel >= 3 ){
  //     G4cout << G4endl;    
  //     G4cout << std::setw( 5) << "#Step#"     << " "
  //            << std::setw( 6) << "X"          << "    "
  //            << std::setw( 6) << "Y"          << "    "  
  //            << std::setw( 6) << "Z"          << "    "
  //            << std::setw( 9) << "KineE"      << " "
  //            << std::setw( 9) << "dEStep"     << " "  
  //            << std::setw(10) << "StepLeng"     
  //            << std::setw(10) << "TrakLeng" 
  //            << std::setw(10) << "Volume"    << "  "
  //            << std::setw(10) << "Process"   << G4endl;                  
  //   }

  //   G4cout << std::setw( 5) << fTrack->GetCurrentStepNumber() << " "
  //       << std::setw(6) << G4BestUnit(fTrack->GetPosition().x(),"Length")
  //       << std::setw(6) << G4BestUnit(fTrack->GetPosition().y(),"Length")
  //       << std::setw(6) << G4BestUnit(fTrack->GetPosition().z(),"Length")
  //       << std::setw(6) << G4BestUnit(fTrack->GetKineticEnergy(),"Energy")
  //       << std::setw(6) << G4BestUnit(fStep->GetTotalEnergyDeposit(),"Energy")
  //       << std::setw(6) << G4BestUnit(fStep->GetStepLength(),"Length")
  //       << std::setw(6) << G4BestUnit(fTrack->GetTrackLength(),"Length")
  //       << std::setw(10) << fTrack->GetVolume()->GetName();

  // const G4VProcess* process 
  //                     = fStep->GetPostStepPoint()->GetProcessDefinedStep();
  // G4String procName = " UserLimit";
  // if (process) procName = process->GetProcessName();
  // G4cout << procName << G4endl;
  // if(procName != "initStep" && fTrack->GetDefinition()->GetParticleName() == "opticalphoton"){
  //   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  //   analysisManager->FillNtupleDColumn(3, 7, -2);
  //   analysisManager->FillNtupleDColumn(3, 8, -2);
  //   analysisManager->FillNtupleDColumn(3, 9, -2);
  // }
  //   if (fStepStatus == fWorldBoundary) procName = "OutOfWorld";
  //   G4cout << "   " << std::setw(10) << procName;
  //   G4cout << G4endl;

  //   if (verboseLevel == 2) {
  //     const std::vector<const G4Track*>* secondary 
  //                                   = fStep->GetSecondaryInCurrentStep();
  //     size_t nbtrk = (*secondary).size();
  //     if (nbtrk) {
  //       G4cout << "\n    :----- List of secondaries ----------------" << G4endl;
  //       G4cout.precision(4);
  //       for (size_t lp=0; lp<(*secondary).size(); lp++) {
  //         G4cout << "   "
  //                << std::setw(13)                 
  //                << (*secondary)[lp]->GetDefinition()->GetParticleName()
  //                << ":  energy ="
  //                << std::setw(6)
  //                << G4BestUnit((*secondary)[lp]->GetKineticEnergy(),"Energy")
  //                << "  time ="
  //                << std::setw(6)
  //                << G4BestUnit((*secondary)[lp]->GetGlobalTime(),"Time");
  //         G4cout << G4endl;
  //       }
              
  //       G4cout << "    :------------------------------------------\n" << G4endl;
  //     }
  //   }
    
  //}
  //G4cout.precision(prec);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
