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
  if ( fDHC1ID==-1 ) {
    auto SDman = G4SDManager::GetSDMpointer();
    fDHC1ID = SDman->GetCollectionID("AorWdetector/detectorColl");
  }
  //ベクトルをスワップ法で初期化
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
      if(pName == "e-" || pName == "e+"){//電子についての情報取得
	electronTrackID.push_back(hit->GetTrackID());
      }
      if(pName == "opticalphoton"){//光子についての情報取得
	TrackID.push_back(hit->GetTrackID());
	//ParentID > 1 なら電子由来の光子、ParentID == 1ならミューオン由来の光子
	if(hit->GetParentID() > 1) eleTrackID.push_back(hit->GetTrackID());
	if(hit->GetParentID() == 1) muTrackID.push_back(hit->GetTrackID());	
	if(hit->GetProcessName() == "bottom" ){//もし光子がアクリルの底面にいたら
	  nOfbottom++;//number Of bottom を+1
	  if(hit->GetParentID() > 1) elenOfbottom++; //電子由来の光子
	  if(hit->GetParentID() == 1) munOfbottom++; //ミューオン由来の光子
	}
      }
      if(pName == "mu-"){//ミューオンについての情報取得
	StLength += hit->GetStLength();
	Pos_esc_mu = hit->GetPosition();
      }

      G4int Status = hit->GetBoundaryStatus();
      if(Status == 3 || Status == 4){//アクリル上面についての情報取得
	nOftopRefle++;
	if(hit->GetParentID() > 1) elenOftopRefle++;
	if(hit->GetParentID() == 1) munOftopRefle++;
      }
    }

  if(Pos_esc_mu.mag() > 0){//ミューオンがアクリルから出た座標
    Pos_esc_mu_x = Pos_esc_mu.x();
    Pos_esc_mu_y = -Pos_esc_mu.y() + 250*mm;
    Pos_esc_mu_z = Pos_esc_mu.z();
  }

  if(electronTrackID.size() > 0){//電子のTrackIDのユニーク数
    std::sort(electronTrackID.begin(), electronTrackID.end());
    nOfelectron = unique_count(electronTrackID.begin(), electronTrackID.end());
  }
  if(TrackID.size() > 0){//光子のTrackIDのユニーク数
    std::sort(TrackID.begin(), TrackID.end());
    nOfPhoton = unique_count(TrackID.begin(), TrackID.end());
  }
  if(eleTrackID.size() > 0){//電子由来の光子のTrackIDのユニーク数
    std::sort(eleTrackID.begin(), eleTrackID.end());
    elenOfPhoton = unique_count(eleTrackID.begin(), eleTrackID.end());
  }
  if(muTrackID.size() > 0){//ミューオン由来の光子のTrackIDのユニーク数
    std::sort(muTrackID.begin(), muTrackID.end());
    munOfPhoton = unique_count(muTrackID.begin(), muTrackID.end());
  }

  //値をrootファイルに詰める
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
  

  //処理が10%終わるごとにLINEで通知
  //ただし、10000イベント未満のシミュレーションの場合、通知はしない  
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

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
