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
// $Id: CerenkovRunAction.cc 75214 2013-10-29 16:04:42Z gcosmo $
//
/// \file CerenkovRunAction.cc
/// \brief Implementation of the CerenkovRunAction class

#include "CerenkovRunAction.hh"
#include "CerenkovAnalysis.hh"
#include "CerenkovEventAction.hh"
#include "DetectorHit.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "parameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CerenkovRunAction::CerenkovRunAction(CerenkovEventAction* eventAction)
  : G4UserRunAction(),
     fEventAction(eventAction)
{ 
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);

  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in CerenkovAnalysis.hh
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() <<G4endl;

  // Create directories
  //analysisManager->SetNtupleMerging(true);
  analysisManager->SetVerboseLevel(1);


  if ( fEventAction ) {
  // Creating ntuple
  //
  analysisManager->SetFirstNtupleId(2);

  // Crare 2nd ntuple (初期情報)
  analysisManager->CreateNtuple("nt2","event info");
  analysisManager->CreateNtupleIColumn("eventID");
  analysisManager->CreateNtupleDColumn("PrimaryEnergy");
  analysisManager->CreateNtupleDColumn("Mom_x");
  analysisManager->CreateNtupleDColumn("Mom_y");
  analysisManager->CreateNtupleDColumn("Mom_z");
  analysisManager->CreateNtupleDColumn("InitPos_x");
  analysisManager->CreateNtupleDColumn("InitPos_y");
  analysisManager->CreateNtupleDColumn("InitPos_z");
  analysisManager->CreateNtupleDColumn("Theta");
  analysisManager->CreateNtupleDColumn("Phi");  
  analysisManager->CreateNtupleIColumn("nOfPhoton");
  analysisManager->CreateNtupleDColumn("esc_x");
  analysisManager->CreateNtupleDColumn("esc_y");
  analysisManager->CreateNtupleDColumn("esc_z");
  analysisManager->CreateNtupleIColumn("nOfbottom");
  analysisManager->CreateNtupleDColumn("muLength");
  analysisManager->CreateNtupleIColumn("nOftopRefle");
  analysisManager->CreateNtupleIColumn("munOfPhoton");
  analysisManager->CreateNtupleIColumn("munOfbottom");
  analysisManager->CreateNtupleIColumn("munOftopRefle");
  analysisManager->CreateNtupleIColumn("elenOfPhoton");
  analysisManager->CreateNtupleIColumn("elenOfbottom");
  analysisManager->CreateNtupleIColumn("elenOftopRefle");
  analysisManager->CreateNtupleIColumn("nOfele");      
  analysisManager->FinishNtuple(2);  

  analysisManager->CreateNtuple("nt4","Photon data");
  // analysisManager->CreateNtupleIColumn("eventID");
  analysisManager->CreateNtupleIColumn("Status");  
  // analysisManager->CreateNtupleDColumn("esc_x");
  // analysisManager->CreateNtupleDColumn("esc_y");
  // analysisManager->CreateNtupleDColumn("esc_z");
  // analysisManager->CreateNtupleIColumn("TrackID");
  // analysisManager->CreateNtupleSColumn("Process");
  // analysisManager->CreateNtupleDColumn("init_x");
  // analysisManager->CreateNtupleDColumn("init_y");
  // analysisManager->CreateNtupleDColumn("init_z");
  // analysisManager->CreateNtupleIColumn("Track");  
  // analysisManager->FinishNtuple(3);

  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int CerenkovRunAction::nbEventInRun;

CerenkovRunAction::~CerenkovRunAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CerenkovRunAction::BeginOfRunAction(const G4Run* run)
{ 
  // inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open ana output file
  //

  nbEventInRun = run->GetNumberOfEventToBeProcessed();
  analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CerenkovRunAction::EndOfRunAction(const G4Run* /*run*/)
{  
  // print histogram statistics
  //
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
  
    
  if(G4Threading::G4GetThreadId() == -1 && nbEventInRun >= 10000){
    char buff[255];
    sprintf(buff,"zsh inform.sh '\n%dイベントシミュレーションが完了しました．'",nbEventInRun);
    system(buff);
    system("rm output.root;hadd output.root *.root;zsh inform.sh '\nRootファイルの結合が完了しました．'");
  }
  else if(G4Threading::G4GetThreadId() == -1) system("rm output.root;hadd output.root *.root");


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
