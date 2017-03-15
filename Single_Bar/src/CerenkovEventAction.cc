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
// $Id: CerenkovEventAction.cc 75214 2013-10-29 16:04:42Z gcosmo $
//
/// \file CerenkovEventAction.cc
/// \brief Implementation of the CerenkovEventAction class

#include "CerenkovEventAction.hh"
#include "DetectorHit.hh"
#include "CerenkovRunAction.hh"
#include "CerenkovAnalysis.hh"
#include "SensitiveVolume.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

#include "G4PhysicalConstants.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CerenkovEventAction::CerenkovEventAction()
  : G4UserEventAction(),
    fDHC1ID(-1)
{
  G4RunManager::GetRunManager()->SetPrintProgress(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CerenkovEventAction::~CerenkovEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CerenkovEventAction::BeginOfEventAction(const G4Event*)
{
  //アクリル用------------------------------------------
  if ( fDHC1ID==-1 ) {
    auto SDman = G4SDManager::GetSDMpointer();
    fDHC1ID = SDman->GetCollectionID("AorWdetector/detectorColl");
  }
  std::vector<G4int>().swap(electronTrackID);
  std::vector<G4int>().swap(TrackID);
  std::vector<G4int>().swap(muTrackID);
  std::vector<G4int>().swap(eleTrackID);  
  std::vector<G4ThreeVector>().swap(Pos_mu);
  nOfelectron = 0;
  nOfbottom = 0;
  elenOfbottom = 0;
  munOfbottom = 0;
  nOfPhoton = 0;
  elenOfPhoton = 0;
  munOfPhoton = 0;
  nOftopRefle = 0;
  elenOftopRefle = 0;
  munOftopRefle = 0;
  StLength = 0;

  
  //-------------------------------------------------------------
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CerenkovEventAction::EndOfEventAction(const G4Event* event)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4PrimaryVertex* primaryVertex = event->GetPrimaryVertex();
  G4PrimaryParticle* primaryParticle = primaryVertex->GetPrimary();
  G4double px = primaryParticle->GetPx();
  G4double py = primaryParticle->GetPy();
  G4double pz = primaryParticle->GetPz();
  G4double x0 = primaryVertex->GetX0();
  G4double y0 = primaryVertex->GetY0();
  G4double z0 = primaryVertex->GetZ0();
  G4double ke = primaryParticle->GetKineticEnergy();

  G4int sgnz = (z0 > 0) - (z0 < 0);
  G4int sgnx = (x0 > 0) - (x0 < 0);  

  G4double Phi;
  if(x0 == 0 && z0 == 0) Phi = 0;
  else Phi = sgnz*std::acos(x0/sqrt(x0*x0 + z0*z0));
  
  G4double rSphere = std::sqrt(x0*x0 + y0*y0 + z0*z0);
  G4double Theta = sgnx*std::acos(y0/rSphere);
  
  eventID = event->GetEventID();

  G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << eventID << G4endl;
  }

  G4HCofThisEvent* hce = event->GetHCofThisEvent();
  if (!hce) 
    {
      G4ExceptionDescription msg;
      msg << "No hits collection of this event found." << G4endl; 
      G4Exception("CerenkovEventAction::EndOfEventAction()",
  		  "takaCode001", JustWarning, msg);
      return;
    }   

  // Get hits collections
  DetectorHitsCollection* dHC1
    = static_cast<DetectorHitsCollection*>(hce->GetHC(fDHC1ID));

  if ( !dHC1 ) {
    G4ExceptionDescription msg;
    msg << "Some of hits collections of this event not found." << G4endl; 
    G4Exception("CerenkovEventAction::EndOfEventAction()",
  		"takaCode001", JustWarning, msg);
    return;
  }

  
  for (G4int i=0;i<dHC1->entries();i++)
    {
      DetectorHit* hit = (*dHC1)[i];
      G4String pName = hit->GetpName();
      if(pName == "e-" || pName == "e+"){
	electronTrackID.push_back(hit->GetTrackID());
      }
      if(pName == "opticalphoton"){
	TrackID.push_back(hit->GetTrackID());
	if(hit->GetParentID() > 1) eleTrackID.push_back(hit->GetTrackID());
	if(hit->GetParentID() == 1) muTrackID.push_back(hit->GetTrackID());	
	if(hit->GetProcessName() == "bottom" ){
	  nOfbottom++;
	  if(hit->GetParentID() > 1) elenOfbottom++;
	  if(hit->GetParentID() == 1) munOfbottom++;
	}
      }
      if(pName == "mu-"){
	StLength += hit->GetStLength();
	Pos_esc_mu = hit->GetPosition();
      }
      // analysisManager->FillNtupleIColumn(3, 0, hit->GetBoundaryStatus());
      // analysisManager->AddNtupleRow(3);
      G4int Status = hit->GetBoundaryStatus();
      if(Status == 3 || Status == 4){
	nOftopRefle++;
	if(hit->GetParentID() > 1) elenOftopRefle++;
	if(hit->GetParentID() == 1) munOftopRefle++;
      }
    }

  if(Pos_esc_mu.mag() > 0){
    Pos_esc_mu_x = Pos_esc_mu.x();
    Pos_esc_mu_y = -Pos_esc_mu.y() + 250*mm;
    Pos_esc_mu_z = Pos_esc_mu.z();
  }
  else{
    Pos_esc_mu_x = -1000*km;
    Pos_esc_mu_y = -1000*km;
    Pos_esc_mu_z = -1000*km;
  }

  if(electronTrackID.size() > 0){
    std::sort(electronTrackID.begin(), electronTrackID.end());
    nOfelectron = unique_count(electronTrackID.begin(), electronTrackID.end());
  }
  if(TrackID.size() > 0){
    std::sort(TrackID.begin(), TrackID.end());
    nOfPhoton = unique_count(TrackID.begin(), TrackID.end());
  }
  if(eleTrackID.size() > 0){
    std::sort(eleTrackID.begin(), eleTrackID.end());
    elenOfPhoton = unique_count(eleTrackID.begin(), eleTrackID.end());
  }
  if(muTrackID.size() > 0){
    std::sort(muTrackID.begin(), muTrackID.end());
    munOfPhoton = unique_count(muTrackID.begin(), muTrackID.end());
  }
  
  analysisManager->FillNtupleIColumn(2, 0, eventID);    
  analysisManager->FillNtupleDColumn(2, 1, ke/GeV);
  analysisManager->FillNtupleDColumn(2, 2, px);
  analysisManager->FillNtupleDColumn(2, 3, py);
  analysisManager->FillNtupleDColumn(2, 4, pz);
  analysisManager->FillNtupleDColumn(2, 5, x0/mm);
  analysisManager->FillNtupleDColumn(2, 6, y0/mm);
  analysisManager->FillNtupleDColumn(2, 7, z0/mm);
  analysisManager->FillNtupleDColumn(2, 8, Theta/degree);
  analysisManager->FillNtupleDColumn(2, 9, Phi/degree);
  analysisManager->FillNtupleIColumn(2, 10, nOfPhoton);
  analysisManager->FillNtupleDColumn(2, 11, Pos_esc_mu_x/mm);
  analysisManager->FillNtupleDColumn(2, 12, Pos_esc_mu_y/mm);
  analysisManager->FillNtupleDColumn(2, 13, Pos_esc_mu_z/mm);
  analysisManager->FillNtupleIColumn(2, 14, nOfbottom);
  analysisManager->FillNtupleDColumn(2, 15, StLength/mm);
  analysisManager->FillNtupleIColumn(2, 16, nOftopRefle);
  analysisManager->FillNtupleIColumn(2, 17, munOfPhoton);
  analysisManager->FillNtupleIColumn(2, 18, munOfbottom);
  analysisManager->FillNtupleIColumn(2, 19, munOftopRefle);
  analysisManager->FillNtupleIColumn(2, 20, elenOfPhoton);
  analysisManager->FillNtupleIColumn(2, 21, elenOfbottom);
  analysisManager->FillNtupleIColumn(2, 22, elenOftopRefle);
  analysisManager->FillNtupleIColumn(2, 23, nOfelectron);  
  

  analysisManager->AddNtupleRow(2);
  if(CerenkovRunAction::nbEventInRun >= 10000){
    for(G4int i = 0;i < 10;i++){
      if(1.*eventID/CerenkovRunAction::nbEventInRun == i/10.){
	char buff[255];
	sprintf(buff,"zsh inform.sh '\n%d%s：%d/%dイベント'",
		i*10,"%完了",eventID,CerenkovRunAction::nbEventInRun);
	system(buff);
      }
    }
  }
  //
  // Fill histograms & ntuple
  // 
    
  // Get analysis manager
  //G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  //
  // Fill histograms & ntuple
  //
  // 位置とTrackID

  
  
  //nt1への書き込み
  
  // for (G4int i=0;i<dHC1->entries();i++)
  //   {      
  //     DetectorHit* hit = (*dHC1)[i];
  //     analysisManager->FillNtupleIColumn(1, 0, i);
  //     analysisManager->FillNtupleIColumn(1, 1, eventID);
  //     analysisManager->FillNtupleIColumn(1, 2, hit->GetTrackID());
  //     analysisManager->FillNtupleDColumn(1, 3, hit->GetPos().x());
  //     analysisManager->FillNtupleDColumn(1, 4, hit->GetPos().y());
  //     analysisManager->FillNtupleDColumn(1, 5, hit->GetPos().z());
  //     analysisManager->FillNtupleIColumn(1, 6, hit->GetBoundary());
  //     analysisManager->FillNtupleIColumn(1, 7, hit->GetPDGCode());            
  //     analysisManager->AddNtupleRow(1);
  //   }

}

//------------------------------------------------------------------



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
