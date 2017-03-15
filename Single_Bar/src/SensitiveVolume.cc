#include "SensitiveVolume.hh"
#include "CerenkovAnalysis.hh"
#include "CerenkovRunAction.hh"
#include "CerenkovEventAction.hh"
#include "DetectorHit.hh"
#include "CerenkovPrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"

#include "G4SDManager.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4HCofThisEvent.hh"
#include "G4VProcess.hh"
#include "G4StepPoint.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include <vector>
#include <algorithm>
#include <iostream>


//-----------------------------------------------------------------------
SensitiveVolume::SensitiveVolume(G4String name)
  : G4VSensitiveDetector(name),fHitsCollection(nullptr), fHCID(-1),
    Position(0)
    //ParentID(0),
    //PDGCode(0),
{
  //  G4String HCname = "detectorColl";
  collectionName.insert( "detectorColl");
}

//-----------------------------------------------------------------------
SensitiveVolume::~SensitiveVolume()
{}

//-----------------------------------------------------------------------
void SensitiveVolume::Initialize(G4HCofThisEvent* hce)
{

  // Create hits collection
  fHitsCollection
    = new DetectorHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce
  if (fHCID<0)
    { fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); }
  hce->AddHitsCollection( fHCID, fHitsCollection );

  // Positions_mu.clear();
  // Positions.clear();  
  // TrackIDs.clear();
  // // ParentIDs.clear();
  // // PDGCodes.clear();
  // // endOfPos.clear();
  // Boundarys.clear();
  // std::vector<G4ThreeVector>().swap(Positions_mu);
  // std::vector<G4ThreeVector>().swap(Positions);
  // std::vector<G4int>().swap(TrackIDs);
  // std::vector<G4int>().swap(Boundarys);

  // TrackID = 0;
  // WL = 0;
  // mu_length = 0;
  // nbOfbottom = 0;
  // nOfPhoton = 0;

}

//-----------------------------------------------------------------------

G4bool SensitiveVolume::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  DetectorHit* newhit = new DetectorHit();
  G4Track* aTrack = aStep->GetTrack();
  //if ( aTrack->GetCurrentStepNumber() == 1 ) return false;  
  G4ParticleDefinition* pType = aTrack->GetDefinition();
  G4String pName = pType->GetParticleName();
  //G4StepPoint* point1 = aStep->GetPreStepPoint();
  G4StepPoint* point2 = aStep->GetPostStepPoint();
  const G4VProcess* aProcess = point2->GetProcessDefinedStep();
  G4String ProcessName = aProcess->GetProcessName();
  Position = point2->GetPosition();
  StLength = aStep->GetStepLength();


  ///////////////
  
  G4OpBoundaryProcessStatus boundaryStatus=Undefined;
  static G4ThreadLocal G4OpBoundaryProcess* boundary=NULL;

  // find the boundary process only once
  if(!boundary){
    G4ProcessManager* pm
      = aStep->GetTrack()->GetDefinition()->GetProcessManager();
    G4int nprocesses = pm->GetProcessListLength();
    G4ProcessVector* pv = pm->GetProcessList();
    G4int i;
    for( i=0;i<nprocesses;i++){
      if((*pv)[i]->GetProcessName()=="OpBoundary"){
        boundary = (G4OpBoundaryProcess*)(*pv)[i];
        break;
      }
    }
  }
  
  ///////////////
  
  //if (point2->GetStepStatus() != fGeomBoundary) return false;
  TrackID = aTrack->GetTrackID();
  ParentID = aTrack->GetParentID();
  //if(TrackID != 1) return false;
  // if (pName == "mu-"){
  //   Positions_mu.push_back(Position);
  //   mu_length += StLength;
  // }
  //プランク定数
  G4double h = 4.13566*pow(10,-21)*MeV*s;
  // //光速
  G4double c = 2.99792458*pow(10,17)*nm/s;
  G4double Energy = point2->GetKineticEnergy();
  WL = (h*c/Energy)/nm;
  if ((pName == "opticalphoton" && (WL >= 300. && WL <= 650.)) || pName == "mu-" || pName == "e-" || pName == "e+"){
    newhit->SetParentID(ParentID);
    newhit->SetTrackID(TrackID);
    newhit->SetStLength(StLength);
    newhit->SetpName(pName);
    newhit->SetPosition(Position);

    if(-Position.y()/mm + 250*mm == 500*mm && point2->GetStepStatus() == fGeomBoundary) ProcessName = "bottom";

    newhit->SetProcessName(ProcessName);
    if (pName == "opticalphoton" && point2->GetStepStatus() == fGeomBoundary && -Position.y()/mm + 250*mm == 0*mm){
      boundaryStatus = boundary->GetStatus();
      newhit->SetBoundaryStatus(boundaryStatus);
    }
    else newhit->SetBoundaryStatus(-1);
  }

  //--------------------------------------------------------------------------//
  
  //G4cout << "Process:" << ProcessName << G4endl;
  //G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  //if(pName != "opticalphoton" || boundaryStatus == FresnelReflection || boundaryStatus == TotalInternalReflection) return false;

  //if(ProcessName == "OpAbsorption"){
  //if(-Position.y() + 250*mm == 500*mm) ProcessName = "Bottom";
  // analysisManager->FillNtupleIColumn(3, 0, CerenkovEventAction::eventID);
  // analysisManager->FillNtupleIColumn(3, 1, boundaryStatus);
  // analysisManager->FillNtupleDColumn(3, 2, Position.x());
  // analysisManager->FillNtupleDColumn(3, 3, -Position.y() + 250*mm);
  // analysisManager->FillNtupleDColumn(3, 4, Position.z());
  // analysisManager->FillNtupleIColumn(3, 5, TrackID);
  // analysisManager->FillNtupleSColumn(3, 6, ProcessName);
  // analysisManager->AddNtupleRow(3);   
  //}
  //if (point2->GetStepStatus() == fGeomBoundary){
  //if(-Position.y()/mm + 250*mm == 500*mm){
  //ProcessName = "bottom";
  //nbOfbottom++;
  //}
  //newhit->SetBounday(1);
  // Boundarys.push_back(1);
  // //if(boundaryStatus != TotalInternalReflection && boundaryStatus != FresnelReflection){
  // analysisManager->FillNtupleIColumn(3, 0, CerenkovEventAction::eventID);
  // analysisManager->FillNtupleIColumn(3, 1, boundaryStatus);
  // analysisManager->FillNtupleDColumn(3, 2, Position.x()/mm);
  // analysisManager->FillNtupleDColumn(3, 3, -Position.y()/mm + 250*mm);
  // analysisManager->FillNtupleDColumn(3, 4, Position.z()/mm);
  // analysisManager->FillNtupleIColumn(3, 5, TrackID);
  // analysisManager->FillNtupleSColumn(3, 6, ProcessName);    
  // analysisManager->AddNtupleRow(3);
  //   }
  // }
  //}
  
  // switch(boundaryStatus){
  // case FresnelReflection:
  //   G4cout << "FresnelReflection" << G4endl;
  //   break;    
  // case TotalInternalReflection:
  //   G4cout << "TotalInternalReflection" << G4endl;
  //   break;    
  // default:
  //   break;
  //}

  // //ParentID = aTrack->GetParentID();
  // PDGCode = pType->GetPDGEncoding();

  

  // //G4double time = aTrack->GetGlobalTime();

  // // G4String creatorProcName =  aTrack->GetCreatorProcess()->GetProcessName();
  // // G4cout << creatorProcName << G4endl;
  
  
  // // DetectorHit* newhit = new DetectorHit();
  // // newhit->SetTrackID(TrackID);
  // // newhit->SetParentID(ParentID);
  // // newhit->SetPos(Position);
  // // newhit->SetWL(WL);
  // // newhit->SetPDGCode(PDGCode);

  // // fHitsCollection->insert(newhit);
  // // G4cout << PDGCode << " " << TrackID << G4endl;

  // //ParentIDs.push_back(ParentID);
  // //PDGCodes.push_back(PDGCode);
  // //  times.push_back(time);
  // // pNames.push_back(pName);
  // Positions.push_back(Position);

  //if (pName == "e-") Positions_ele.push_back(Position);
  //if (pName == "opticalphoton") Positions_photon.push_back(Position);    
  // wavelengths.push_back(WL);



  //newhit->SetTrackID(TrackID);
  //newhit->SetPos(Position);
  //newhit->SetParentID(ParentID);
  //newhit->SetParentPDG(return_parentPDGCode(TrackID));
  //newhit->SetWL(WL/nm);
  //newhit->SetPDGCode(PDGCode);
  //newhit->SetProcess(ProcessName);  
  //newhit->SetEnergy(Energy/MeV);
  //newhit->SetStLength(StLength/mm);
  //newhit->SetTime(time/ns);
  //fHitsCollection->insert(newhit);
   
  // if(TrackIDs.size() == 1) return false;

  // if(TrackIDs[TrackIDs.size()-1] !=   TrackIDs[TrackIDs.size()-2]){
  //   //G4cout << TrackIDs[TrackIDs.size()-1] << "," << TrackIDs[TrackIDs.size()-2] << G4endl;
  //   analysisManager->FillNtupleIColumn(3, 0, EventID);
  //   analysisManager->FillNtupleDColumn(3, 1, Positions[TrackIDs.size()-2].x());
  //   analysisManager->FillNtupleDColumn(3, 2, -Positions[TrackIDs.size()-2].y() + 250*mm);
  //   analysisManager->FillNtupleDColumn(3, 3, Positions[TrackIDs.size()-2].z());
  //   analysisManager->FillNtupleIColumn(3, 4, TrackIDs[TrackIDs.size()-2]);
  //   analysisManager->FillNtupleIColumn(3, 5, Boundarys[TrackIDs.size()-2]);        
  //   analysisManager->AddNtupleRow(3);
  // }
  
  // coordinates world and local
  // G4cout << "  Volume Name: Mother Volume Name: " << volumeName << "  :  " << motherVolumeName << G4endl;
  // G4cout  << "(CopyNo, motherCopyNo) : " << copyNo << " , "<< motherCopyNo << G4endl;
  
  // G4cout << pName << "," << PDGCode << "," << TrackID << "," << ParentID << G4endl;

  fHitsCollection->insert(newhit);

  return true;
}

//-----------------------------------------------------------------------
void SensitiveVolume::EndOfEvent(G4HCofThisEvent*)
{
  //   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();  
  //   if(Positions_mu.size() > 0){
  //     auto Pos_esc_mu = Positions_mu.back();
  //     auto Pos_esc_mu_x = Pos_esc_mu.x();
  //     auto Pos_esc_mu_y = Pos_esc_mu.y();
  //     auto Pos_esc_mu_z = Pos_esc_mu.z();
  //     analysisManager->FillNtupleDColumn(2, 11, Pos_esc_mu_x/mm);
  //     analysisManager->FillNtupleDColumn(2, 12, -Pos_esc_mu_y/mm + 250*mm);
  //     analysisManager->FillNtupleDColumn(2, 13, Pos_esc_mu_z/mm);
  //   }
  //   else{
  //     analysisManager->FillNtupleDColumn(2, 11, -1000);
  //     analysisManager->FillNtupleDColumn(2, 12, -1000);
  //     analysisManager->FillNtupleDColumn(2, 13, -1000);  
  //   }

  //   //analysisManager->FillNtupleIColumn(2, 14, nbOfbottom);
  //   analysisManager->FillNtupleDColumn(2, 15, mu_length/mm);
  //   analysisManager->FillNtupleIColumn(2, 16, CerenkovEventAction::eventID);
      
  //   // auto Pos_esc_photon = Positions_photon.back();
  //   // auto Pos_esc_photon_x = Pos_esc_photon.x();
  //   // auto Pos_esc_photon_y = Pos_esc_photon.y();
  //   // auto Pos_esc_ptoton_z = Pos_esc_photon.z();
  
  //   // // for(G4int i=0; i < (G4int)TrackIDs.size();i++){
  //   // //   if(Boundarys[i] != 1 || PDGCodes[i] != 0){
  //   // //     continue;
  //   // //   }
  //   // //if(TrackIDs[i+1] != TrackIDs[i]){
  //   // analysisManager->FillNtupleIColumn(3, 0, EventID);
  //   // analysisManager->FillNtupleDColumn(3, 1, Positions.back().x());
  //   // analysisManager->FillNtupleDColumn(3, 2, -Positions.back().y() + 250*mm);
  //   // analysisManager->FillNtupleDColumn(3, 3, Positions.back().z());
  //   // analysisManager->AddNtupleRow(3);
  //   //}
  //   // }
  

  //   //TrackIDs.erase(std::unique(TrackIDs.begin(), TrackIDs.end()), TrackIDs.end());

  //   //G4cout << TrackIDs.size() << G4endl;
  //   if(TrackIDs.size() > 0){
  //     std::sort(TrackIDs.begin(), TrackIDs.end());
  //     nOfPhoton = unique_count(TrackIDs.begin(), TrackIDs.end());
  //     // analysisManager->FillNtupleIColumn(2, 10, nOfPhoton);
  //   }
  //   else{
  //     // analysisManager->FillNtupleIColumn(2, 10, 0);
  //   }
  
  //   analysisManager->AddNtupleRow(2);

  //G4cout << Positions.size() << "," << Positions.capacity() << G4endl;

  if ( verboseLevel>1 ) {
    G4int nofHits = fHitsCollection->entries();
    G4cout << G4endl
	   << "-------->Hits Collection: in this event they are " << nofHits
	   << " hits in the Detector: " << G4endl;
    for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
  }
}


//-----------------------------------------------------------------------
