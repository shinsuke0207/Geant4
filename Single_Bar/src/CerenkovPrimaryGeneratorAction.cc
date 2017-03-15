#include "CerenkovPrimaryGeneratorAction.hh"
#include "DetectorHit.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include "CerenkovAnalysis.hh"
#include "CerenkovRunAction.hh"

#include "G4GeneralParticleSource.hh"

#include "Randomize.hh"
#include "G4PhysicalConstants.hh"
#include "G4RandomDirection.hh"
#include "parameter.hh"

#include <cmath>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//G4double CerenkovPrimaryGeneratorAction::theta;

CerenkovPrimaryGeneratorAction::CerenkovPrimaryGeneratorAction()
  :  G4VUserPrimaryGeneratorAction(),
     fParticleGun(0), 
     fWorldBox(0),
     E(0.),
     eventID(0)
     //  fParticleGPS(0)
{
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);
  //fParticleGPS = new G4GeneralParticleSource();
  
  // default particle kinematic
  
  G4ParticleDefinition* particleDefinition
    = G4ParticleTable::GetParticleTable()->FindParticle("mu-");
    
  fParticleGun->SetParticleDefinition(particleDefinition);

  // fParticleGun->SetParticleMomentumDirection(G4ThreeVector(mu_vector_X,mu_vector_Y,mu_vector_Z));


  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CerenkovPrimaryGeneratorAction::~CerenkovPrimaryGeneratorAction()
{
  delete fParticleGun;
  //delete fParticleGPS;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CerenkovPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // This function is called at the begining of event
    
  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume
  // from G4LogicalVolumeStore.
    
  G4double worldHalfLengthZ = 0;
  G4double worldHalfLengthY = 0;
  G4double worldHalfLengthX = 0;
  G4double AcrylHalfLengthX = 0;
  G4double AcrylHalfLengthY = 0;
  G4double AcrylHalfLengthZ = 0;  
  // G4double x0 = 0;
  G4double y0 = 0;
  // G4double z0 = 0;
  G4double px = 0;
  G4double py = 0;
  G4double pz = 0;  
  G4double Energy = 0;
  eventID = anEvent->GetEventID();
  
  G4LogicalVolume* worldLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
  G4Box* worldBox = NULL;
  if ( worldLV ) worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
  if ( worldBox ){
    worldHalfLengthZ = worldBox->GetZHalfLength()*2;
    worldHalfLengthY = worldBox->GetYHalfLength()*2;
    worldHalfLengthX = worldBox->GetXHalfLength()*2;    
  }
  else  {
    G4cerr << "World volume of box not found." << G4endl;
    G4cerr << "Perhaps you have changed geometry." << G4endl;
    G4cerr << "The gun will be place in the center." << G4endl;
  }

  G4LogicalVolume* fLogicAcrylD
    = G4LogicalVolumeStore::GetInstance()->GetVolume("Acryl");
  G4Box* AcrylBox = NULL;
  if ( fLogicAcrylD ) AcrylBox = dynamic_cast<G4Box*>(fLogicAcrylD->GetSolid());
  if ( AcrylBox ){
    AcrylHalfLengthZ = AcrylBox->GetZHalfLength();
    AcrylHalfLengthY = AcrylBox->GetYHalfLength();
    AcrylHalfLengthX = AcrylBox->GetXHalfLength();
  } 
  else  {
    G4cerr << "Acryl volume of box not found." << G4endl;
    G4cerr << "Perhaps you have changed geometry." << G4endl;
    G4cerr << "The gun will be place in the center." << G4endl;
  }

  //初期粒子の入射位置とベクトル
  // x0 = 2*AcrylHalfLengthX*(G4UniformRand()-0.5);
  // //  y0 = 2*AcrylHalfLengthY*(G4UniformRand()-0.5);
  // y0 = 2*AcrylHalfLengthY*(G4UniformRand()-0.5);  
  // z0 = 0.5*worldHalfLengthZ;

  // x0 = worldHalfLengthZ*(G4UniformRand()-0.5);
  y0 = 0.5*worldHalfLengthY;
  // z0 = 0.5*worldHalfLengthZ;

  G4ThreeVector randomDirection = G4RandomDirection();
  px = randomDirection.x();
  py = randomDirection.y();
  pz = std::abs(randomDirection.z());
  
  const G4double rSphere = y0;

  // G4int sgn = (eventID % 2);
  // G4double theta = (G4UniformRand()-0.5)*3*degree;
  //G4double theta = (G4UniformRand()-0.5)*5*degree + sgn*180*degree ;  
  // G4double theta = (eventID % 2) * 180.*degree;

  G4double theta = 0.*degree;  
  // G4double phi = G4UniformRand()*pi;
  G4double phi = 0;

  G4double sphere_x = rSphere*std::sin(theta)*std::cos(phi);
  G4double sphere_y = rSphere*std::cos(theta);
  G4double sphere_z = rSphere*std::sin(theta)*std::sin(phi);

  G4ThreeVector sphereMomentum(-sphere_x,-sphere_y,-sphere_z);

  fParticleGun->SetParticleMomentumDirection(sphereMomentum);
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,-1.,0.));  
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(px,py,pz));
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,-1));
  fParticleGun->SetParticlePosition(G4ThreeVector(sphere_x,sphere_y,sphere_z));
  //fParticleGun->SetParticlePosition(G4ThreeVector(0,y0,-z0));  
  //fParticleGun->SetParticlePosition(G4ThreeVector(0,0,200*mm));  
  //エネルギー
  // Energy = (G4UniformRand()*(10*GeV-100*MeV))+100*MeV;
  Energy = (G4UniformRand()*(100*GeV-1*GeV))+1*GeV;
  // G4int Eint = int((G4UniformRand()*99 + 1));
  // Energy = 1.*Eint*GeV;
  // Energy = 1.*GeV;

  //fParticleGun->SetParticleEnergy(2*eV);
  fParticleGun->SetParticleEnergy(Energy);  
  fParticleGun->GeneratePrimaryVertex(anEvent);
  // Get analysis manager
  // G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  // analysisManager->FillNtupleIColumn(2, 0, eventID);    
  // analysisManager->FillNtupleDColumn(2, 1, Energy/MeV);
  // analysisManager->FillNtupleDColumn(2, 2, sphereMomentum.x());
  // analysisManager->FillNtupleDColumn(2, 3, sphereMomentum.y());
  // analysisManager->FillNtupleDColumn(2, 4, sphereMomentum.z());
  // analysisManager->FillNtupleDColumn(2, 5, sphere_x/mm);
  // analysisManager->FillNtupleDColumn(2, 6, sphere_y/mm);
  // analysisManager->FillNtupleDColumn(2, 7, sphere_z/mm);
  // //analysisManager->FillNtupleDColumn(2, 8, theta/degree);
  // analysisManager->FillNtupleDColumn(2, 9, phi/degree);

  //analysisManager->AddNtupleRow(2);
  //fParticleGPS->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


// #include "CerenkovPrimaryGeneratorAction.hh"

// #include "G4LogicalVolumeStore.hh"
// #include "G4LogicalVolume.hh"
// #include "G4Box.hh"
// #include "G4Event.hh"
// #include "G4ParticleGun.hh"
// #include "G4ParticleTable.hh"
// #include "G4ParticleDefinition.hh"
// #include "G4SystemOfUnits.hh"

// #include "CerenkovAnalysis.hh"
// #include "CerenkovRunAction.hh"

// #include "G4GeneralParticleSource.hh"

// #include "Randomize.hh"
// #include "G4PhysicalConstants.hh"

// #include "parameter.hh"
// //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// CerenkovPrimaryGeneratorAction::CerenkovPrimaryGeneratorAction()
// : G4VUserPrimaryGeneratorAction(),
//   //  fParticleGun(0), 
//   //  fWorldBox(0),
//   //E(0.),
//    fParticleGPS(0)
// {
//   //  G4int nofParticles = 1;
//   //fParticleGun = new G4ParticleGun(nofParticles);
//   fParticleGPS = new G4GeneralParticleSource();
  
//   // default particle kinematic
  
//   G4ParticleDefinition* particleDefinition
//     = G4ParticleTable::GetParticleTable()->FindParticle("mu-");
    
//   fParticleGPS->SetParticleDefinition(particleDefinition);
//   //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(mu_vector_X,mu_vector_Y,mu_vector_Z));


//   //fParticleGun->SetParticleEnergy(mu_Energy);
  
// }

// //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// CerenkovPrimaryGeneratorAction::~CerenkovPrimaryGeneratorAction()
// {
//   // delete fParticleGun;
//   delete fParticleGPS;
// }

// //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// void CerenkovPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
// {
//     // This function is called at the begining of event
    
//     // In order to avoid dependence of PrimaryGeneratorAction
//     // on DetectorConstruction class we get world volume
//     // from G4LogicalVolumeStore.
    
//   G4double worldHalfLength = 0;
//   // G4double x0 = 0;
//   // G4double y0 = 0;
//   // G4double z0 = 0;

//   G4LogicalVolume* worldLV
//     = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
//   G4Box* worldBox = NULL;
//   if ( worldLV ) worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
//   if ( worldBox ) worldHalfLength = worldBox->GetXHalfLength();
//   else  {
//     G4cerr << "World volume of box not found." << G4endl;
//     G4cerr << "Perhaps you have changed geometry." << G4endl;
//     G4cerr << "The gun will be place in the center." << G4endl;
//   }

//   // 初期粒子の入射位置
//   //x0 = 0.*cm;
//   //  y0 = 0.*cm;
//   //  z0 = worldHalfLength;
  
//   // fParticleGun->SetParticlePosition(G4ThreeVector(mu_position_X,mu_position_Y,mu_position_Z));
//   // fParticleGun->GeneratePrimaryVertex(anEvent);
    

//   fParticleGPS->GeneratePrimaryVertex(anEvent);
// }

// //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

