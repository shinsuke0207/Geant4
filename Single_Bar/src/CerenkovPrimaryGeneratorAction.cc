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

  G4ParticleDefinition* particleDefinition
    = G4ParticleTable::GetParticleTable()->FindParticle("mu-");
    
  fParticleGun->SetParticleDefinition(particleDefinition);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CerenkovPrimaryGeneratorAction::~CerenkovPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CerenkovPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{    
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

  // x0 = worldHalfLengthZ*(G4UniformRand()-0.5);
  y0 = 0.5*worldHalfLengthY;
  // z0 = 0.5*worldHalfLengthZ;

  G4ThreeVector randomDirection = G4RandomDirection();
  px = randomDirection.x();
  py = randomDirection.y();
  pz = std::abs(randomDirection.z());
  

  //初期粒子発射座標の球座標を定義

  //初期粒子発射座標のrを定義
  const G4double rSphere = y0;

  // G4int sgn = (eventID % 2);
  // G4double theta = (G4UniformRand()-0.5)*3*degree;
  //G4double theta = (G4UniformRand()-0.5)*5*degree + sgn*180*degree ;  
  // G4double theta = (eventID % 2) * 180.*degree;

  //初期粒子発射座標のTheta,Phiの定義
  G4double theta = 0.*degree;
  // G4double phi = G4UniformRand()*pi;
  G4double phi = 0;

  //Theta,Phiからデカルト座標に直す
  G4double sphere_x = rSphere*std::sin(theta)*std::cos(phi);
  G4double sphere_y = rSphere*std::cos(theta);
  G4double sphere_z = rSphere*std::sin(theta)*std::sin(phi);

  G4ThreeVector sphereMomentum(-sphere_x,-sphere_y,-sphere_z);

  fParticleGun->SetParticleMomentumDirection(sphereMomentum);
  fParticleGun->SetParticlePosition(G4ThreeVector(sphere_x,sphere_y,sphere_z));

  //粒子のエネルギーを定義（連続的）
  Energy = (G4UniformRand()*(100*GeV-1*GeV))+1*GeV;

  //粒子のエネルギーを定義（離散的）
  // G4int Eint = int((G4UniformRand()*99 + 1));
  // Energy = 1.*Eint*GeV;

  fParticleGun->SetParticleEnergy(Energy);  
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
