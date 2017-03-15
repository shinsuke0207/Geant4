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
// $Id: CerenkovSteppingAction.cc 74483 2013-10-09 13:37:06Z gcosmo $
//
/// \file CerenkovSteppingAction.cc
/// \brief Implementation of the CerenkovSteppingAction class

#include "CerenkovSteppingAction.hh"
#include "CerenkovEventAction.hh"
#include "ACerenkovDetectorConstruction.hh"
#include "CerenkovAnalysis.hh"
#include "CerenkovRunAction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

#include "G4ParticleTypes.hh"
#include "G4StepPoint.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

#include "G4OpBoundaryProcess.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CerenkovSteppingAction::CerenkovSteppingAction(/*CerenkovEventAction* eventAction*/)
  : G4UserSteppingAction()
    // fEventAction(eventAction)
{
  fOpProcess = NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CerenkovSteppingAction::~CerenkovSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CerenkovSteppingAction::UserSteppingAction(const G4Step* aStep)
{
  G4Track* track = aStep->GetTrack();
  G4StepPoint* point2 = aStep->GetPostStepPoint();
  G4ThreeVector Position = point2->GetPosition();

  if (track->GetDefinition() != G4MuonPlus::MuonPlusDefinition() &&
      track->GetDefinition() != G4MuonMinus::MuonMinusDefinition() &&
      track->GetDefinition() != G4Electron::ElectronDefinition() &&
      track->GetDefinition() != G4Positron::PositronDefinition() &&
      track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()
      ) track->SetTrackStatus(fKillTrackAndSecondaries);
  
  // G4ParticleDefinition* pType = track->GetDefinition();
  // G4String pName = pType->GetParticleName();
  // G4String StLength = aStep->GetStepLength();
  // G4int TrackID = track->GetTrackID();

  // if (pName == "opticalphoton"){
  //   G4StepPoint* point1 = aStep->GetPreStepPoint();
  //   G4StepPoint* point2 = aStep->GetPostStepPoint();
  //   G4TouchableHandle touch1= point1->GetTouchableHandle();

  //   G4VPhysicalVolume* volume1 = touch1->GetVolume();

  //   G4String name1 = volume1->GetName();

  //   if (point2->GetStepStatus() == fGeomBoundary){
  //     G4TouchableHandle touch2 = point2->GetTouchableHandle();
  //     G4VPhysicalVolume* volume2 = touch2->GetVolume();
  //     G4String name2 = volume2->GetName();
  // 	G4cout << "Track ID = " << TrackID << G4endl;
  // 	G4cout << name1 << "->" << name2  << G4endl;
  // 	G4cout << "Step Length:" << StLength << G4endl;
  // 	G4cout << "------------------------------------------------" << G4endl;
  // }
  // }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
