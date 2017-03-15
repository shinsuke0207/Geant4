#include "ACerenkovDetectorConstruction.hh"
#include "CerenkovActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "G4OpticalPhysics.hh"
#include "G4PhysListFactory.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
#ifdef G4UI_USE
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    // ui = new G4UIExecutive(argc, argv);
    ui = new G4UIExecutive(argc, argv,"tcsh");    
  }
#endif

  // Construct the default run manager
  //
#ifdef G4MULTITHREADED
  auto runManager = new G4MTRunManager;
  runManager->SetNumberOfThreads(4); //マルチプロセス数
#else
  auto runManager = new G4RunManager;
#endif

  // Mandatory user initialization classes
  runManager->SetUserInitialization(new ACerenkovDetectorConstruction);

  auto physicsList = new FTFP_BERT;
  physicsList->RegisterPhysics(new G4OpticalPhysics());

  runManager->SetUserInitialization(physicsList);

  // User action initialization
  runManager->SetUserInitialization(new CerenkovActionInitialization());

#ifdef G4VIS_USE
  // Visualization manager construction
  G4VisManager* visManager = new G4VisExecutive; //Visualizationしないとき用
  //G4VisManager* visManager = new G4VisExecutive("Quiet"); //Visualizationするとき用
  visManager->Initialize();
#endif
    
  // Get the pointer to the User Interface manager
  auto UImanager = G4UImanager::GetUIpointer();

  if ( argc != 1 ) {
    // execute an argument macro file if exist
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
#ifdef G4UI_USE
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac"); 
#else
    UImanager->ApplyCommand("/control/execute init_vis.mac"); 
#endif
    if (ui->IsGUI()) {
      UImanager->ApplyCommand("/control/execute gui.mac");
    }     
    // start interactive session
    ui->SessionStart();
    delete ui;
#endif
  }

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
