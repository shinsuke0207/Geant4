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

  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() <<G4endl;

  // Create directories
  //analysisManager->SetNtupleMerging(true);
  analysisManager->SetVerboseLevel(1);


  if ( fEventAction ) {
  // Creating ntuple
  //
  analysisManager->SetFirstNtupleId(2);

  // イベントごとの情報のntupleを作成
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
  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open ana output file
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
  
  //処理が全て終了したら、rootファイルの統合を行い、LINEで通知
  //ただし、10000イベント未満のシミュレーションの場合、通知はしない
  if(G4Threading::G4GetThreadId() == -1 && nbEventInRun >= 10000){
    char buff[255];
    sprintf(buff,"zsh inform.sh '\n%dイベントシミュレーションが完了しました．'",nbEventInRun);
    system(buff);
    system("rm output.root;hadd output.root *.root;zsh inform.sh '\nRootファイルの結合が完了しました．'");
  }
  else if(G4Threading::G4GetThreadId() == -1) system("rm output.root;hadd output.root *.root");


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
