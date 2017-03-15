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
// $Id: CerenkovActionInitialization.cc 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file CerenkovActionInitialization.cc
/// \brief Implementation of the CerenkovActionInitialization class

#include "CerenkovActionInitialization.hh"
#include "CerenkovPrimaryGeneratorAction.hh"
#include "CerenkovRunAction.hh"
#include "CerenkovEventAction.hh"
#include "CerenkovSteppingAction.hh"
#include "SteppingVerbose.hh"
//#include "T01StackingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CerenkovActionInitialization::CerenkovActionInitialization()
  : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CerenkovActionInitialization::~CerenkovActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CerenkovActionInitialization::BuildForMaster() const
{
  CerenkovEventAction* eventAction = new CerenkovEventAction;
  SetUserAction(new CerenkovRunAction(eventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CerenkovActionInitialization::Build() const
{
  SetUserAction(new CerenkovPrimaryGeneratorAction);
  CerenkovEventAction* eventAction = new CerenkovEventAction;
  SetUserAction(new CerenkovRunAction(eventAction));
  SetUserAction(new CerenkovSteppingAction);  
  //  SetUserAction(new T01StackingAction);
  SetUserAction(eventAction);
}  

G4VSteppingVerbose* CerenkovActionInitialization::InitializeSteppingVerbose() const
{
  return new SteppingVerbose();
}  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
