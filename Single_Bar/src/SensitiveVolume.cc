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
{
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
}

//-----------------------------------------------------------------------

G4bool SensitiveVolume::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  DetectorHit* newhit = new DetectorHit();
  G4Track* aTrack = aStep->GetTrack();
  G4ParticleDefinition* pType = aTrack->GetDefinition();
  G4String pName = pType->GetParticleName();
  G4StepPoint* point2 = aStep->GetPostStepPoint();
  const G4VProcess* aProcess = point2->GetProcessDefinedStep();
  G4String ProcessName = aProcess->GetProcessName();
  Position = point2->GetPosition();
  StLength = aStep->GetStepLength();


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
  
  

  TrackID = aTrack->GetTrackID();
  ParentID = aTrack->GetParentID();

  G4double h = 4.13566*pow(10,-21)*MeV*s;//プランク定数
  G4double c = 2.99792458*pow(10,17)*nm/s;//光速
  G4double Energy = point2->GetKineticEnergy();//エネルギー
  WL = (h*c/Energy)/nm;//波長
  
  if ((pName == "opticalphoton" && (WL >= 300. && WL <= 650.)) || pName == "mu-" || pName == "e-" || pName == "e+"){//光子とミューオンと電子のみの情報を取得
    newhit->SetParentID(ParentID);
    newhit->SetTrackID(TrackID);
    newhit->SetStLength(StLength);
    newhit->SetpName(pName);
    newhit->SetPosition(Position);

    if(-Position.y()/mm + 250*mm == 500*mm && point2->GetStepStatus() == fGeomBoundary) ProcessName = "bottom";//アクリルの底面に光子が到達したら、ProcessNameはbottomとする

    newhit->SetProcessName(ProcessName);
    if (pName == "opticalphoton" && point2->GetStepStatus() == fGeomBoundary && -Position.y()/mm + 250*mm == 0*mm){//アクリルの上面に光子が到達したら、上面での光学過程を記録
      boundaryStatus = boundary->GetStatus();
      newhit->SetBoundaryStatus(boundaryStatus);
    }
    else newhit->SetBoundaryStatus(-1);
  }

  fHitsCollection->insert(newhit);

  return true;
}

//-----------------------------------------------------------------------
void SensitiveVolume::EndOfEvent(G4HCofThisEvent*)
{
  if ( verboseLevel>1 ) {
    G4int nofHits = fHitsCollection->entries();
    G4cout << G4endl
	   << "-------->Hits Collection: in this event they are " << nofHits
	   << " hits in the Detector: " << G4endl;
    for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
  }
}


//-----------------------------------------------------------------------
