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
// $Id: ACerenkovDetectorConstruction.cc 87359 2014-12-01 16:04:27Z gcosmo $
//
/// \file ACerenkovDetectorConstruction.cc
/// \brief Implementation of the ACerenkovDetectorConstruction class

#include "ACerenkovDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4AutoDelete.hh"
#include "G4SubtractionSolid.hh"

#include "G4GlobalMagFieldMessenger.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
#include "G4RotationMatrix.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"

#include "SensitiveVolume.hh"
#include "G4SDManager.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4PVReplica.hh"

#include "parameter.hh"


#include "FresnelLens.hh"


//........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal G4GlobalMagFieldMessenger* ACerenkovDetectorConstruction::fMagFieldMessenger = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ACerenkovDetectorConstruction::ACerenkovDetectorConstruction()
  :G4VUserDetectorConstruction(),
   //アクリル用--------------------------------
   fLogicAcrylD(NULL),
   fLogicAirBox(NULL),
   //------------------------------------------
   fCheckOverlaps(true)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ACerenkovDetectorConstruction::~ACerenkovDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* ACerenkovDetectorConstruction::Construct()
{  
  // Define materials
  DefineMaterials();

  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ACerenkovDetectorConstruction::DefineMaterials()
{
  // Material definition
  lambda_min = 200*nm ; 
  lambda_max = 700*nm ;   
  G4NistManager* nistManager = G4NistManager::Instance();

  //空気と真空の検索
  nistManager->FindOrBuildMaterial("G4_AIR");
  nistManager->FindOrBuildMaterial("G4_Galactic");
  nistManager->FindOrBuildMaterial("G4_WATER");
  //元素の検索
  G4Element* C = nistManager->FindOrBuildElement("C");
  G4Element* O = nistManager->FindOrBuildElement("O");
  G4Element* H = nistManager->FindOrBuildElement("H");

  G4double  density;
  G4String name;
  G4int nel;

  // Aluminum 
  // ---------
  density = 2.7*g/cm3;
  new G4Material(name="Aluminum", 13., 26.98*g/mole, density); 

  density = 1.19*g/cm3;
  
  G4Material* Acrylic = new G4Material(name="Acrylic", density, nel=3);
  Acrylic->AddElement(C, 5);
  Acrylic->AddElement(H, 8);
  Acrylic->AddElement(O, 2);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  G4Material* air = G4Material::GetMaterial("G4_AIR");
  //
  // ------------ アクリルの屈折率と吸収長 ------------
  //
  //翔太の遺産
  // const G4int nEntries = 13;

  // G4double  PhotonEnergy[nEntries] =
  //   { 1.7712*eV,  1.93725*eV, 2.13765*eV, 2.38431*eV,
  //     2.53029*eV, 2.6953*eV,  2.88335*eV, 3.0996*eV,
  //     3.35092*eV, 3.64659*eV, 3.99948*eV, 4.428*eV,   4.95936*eV };
  //
  // acrylic
  //
  // G4double RefractiveIndex2[nEntries] =
  //   { 1.4864,  1.4887, 1.4918,  1.4954, 1.4971, 1.4991,
  //     1.50281, 1.50786, 1.51534, 1.52616,
  //     1.54141, 1.56227, 1.59012};

  // G4double Absorption2[nEntries] =
  //   { 4.4422*m,  4.69363*m, 4.79893*m, 4.79571*m,
  //     4.78895*m, 4.76494*m, 4.71112*m, 4.63091*m,
  //     4.38308*m, 3.89792*m, 2.64466*m, 0.494499*m,
  //     0.0556574*m
  //   };

  //G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
  // myMPT2->AddProperty("RINDEX",       PhotonEnergy, RefractiveIndex2,nEntries)->SetSpline(true);
  // myMPT2->AddProperty("ABSLENGTH",    PhotonEnergy, Absorption2,     nEntries);

  //翔太の遺産
  //air_showerから追加 acrylのパラメータ

   // G4Material* absorberMaterial = G4Material::GetMaterial("G4_WATER");
   //G4Material* absorberMaterial2 = G4Material::GetMaterial("G4_AIR");
   //
   // ------------ Generate & Add Material Properties Table ------------
   //
   const G4int nEntries = 13;
   
   G4double PhotonEnergy[nEntries] =
     { 1.7712*eV,  1.93725*eV, 2.13765*eV, 2.38431*eV,
       2.53029*eV, 2.6953*eV,  2.88335*eV, 3.0996*eV,
       3.35092*eV, 3.64659*eV, 3.99948*eV, 4.428*eV,   4.95936*eV };
   //
   // Water
   //
   // G4double RefractiveIndex1[nEntries] =
   //   { 1.33079, 1.33219, 1.33394, 1.33621,
   //     1.33758, 1.33922, 1.34133, 1.34393,
   //     1.34723, 1.35145, 1.35721, 1.36542,
   //     1.37805};
   
   // G4double Absorption1[nEntries] =
   //   {1.53846*m, 3.03951*m, 9.25926*m, 20.9644*m,
   //    51.0204*m, 64.1026*m, 69.4444*m, 58.4795*m,
   //    33.3333*m, 17.8253*m, 9.52381*m, 3.47222*m,
   //    1.78891*m};

   // G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();
   // myMPT1->AddProperty("RINDEX",       PhotonEnergy, RefractiveIndex1,nEntries)->SetSpline(true);
   // myMPT1->AddProperty("ABSLENGTH",    PhotonEnergy, Absorption1,     nEntries)->SetSpline(true);
   
   // absorberMaterial->SetMaterialPropertiesTable(myMPT1);
   
//////////////////////////////////////////////////////////////////
//               ACRYLIC Optical properties
//////////////////////////////////////////////////////////////////

// Refractive index 

//   const G4int NENTRIES = 11 ;
//   G4double LAMBDA_ACRYLIC[NENTRIES] ;


//   G4double RINDEX_ACRYLIC[NENTRIES] ;
//   G4double ENERGY_ACRYLIC[NENTRIES] ;

// // Parameterization for refractive index of High Grade PMMA 

//   G4double bParam[4] = {1760.7010,-1.3687,2.4388e-3,-1.5178e-6} ; 
  
//   for(G4int i=0;i<NENTRIES; i++){
 
//     LAMBDA_ACRYLIC[i] = lambda_min + i*(lambda_max-lambda_min)/float(NENTRIES-1) ;
//     RINDEX_ACRYLIC[i] = 0.0 ;

//     for (G4int jj=0 ; jj<4 ; jj++)
//     {
//       RINDEX_ACRYLIC[i] +=  (bParam[jj]/1000.0)*std::pow(LAMBDA_ACRYLIC[i]/nm,jj) ; 
//     }

//     ENERGY_ACRYLIC[i] =   h_Planck*c_light/LAMBDA_ACRYLIC[i] ;  // Convert from wavelength to energy ;
// //  G4cout << ENERGY_ACRYLIC[i]/eV << " " << LAMBDA_ACRYLIC[i]/nm << " " << RINDEX_ACRYLIC[i] << G4endl ;

//   }

  // G4MaterialPropertiesTable *myMPT2 = new G4MaterialPropertiesTable();
  // myMPT2->AddProperty("RINDEX", ENERGY_ACRYLIC, RINDEX_ACRYLIC, NENTRIES);
  // const G4int NENT = 25 ;
  // G4double LAMBDAABS[NENT] = 
  // {
  //   100.0,
  //   246.528671, 260.605103, 263.853516, 266.019104, 268.726105,    
  //   271.433136, 273.598724, 276.305725, 279.554138, 300.127380,    
  //   320.159241, 340.191101, 360.764343, 381.337585, 399.745239,    
  //   421.401276, 440.891724, 460.382172, 480.414001, 500.987274,    
  //   520.477722, 540.509583, 559.458618,
  //   700.0    
  // } ;

  // G4double ABS[NENT] =   // Transmission (in %) of  3mm thick PMMA 
  // { 
  //   0.0000000,
  //   0.0000000,  5.295952,  9.657321, 19.937695, 29.283491, 
  //   39.252335, 48.598133, 58.255451, 65.109039, 79.439247,
  //   85.669785, 89.719627, 91.277260, 91.588783, 91.900307,
  //   91.588783, 91.277260, 91.277260, 91.588783, 91.588783,
  //   91.900307, 91.900307, 91.588783,
  //   91.5
  // } ;

  G4double RefractiveIndex2[nEntries] =
    { 1.4864,  1.4887, 1.4918,  1.4954, 1.4971, 1.4991,
      1.50281, 1.50786, 1.51534, 1.52616,
      1.54141, 1.56227, 1.59012};
  
  G4double Absorption2[nEntries] =
    { 4.4422*m,  4.69363*m, 4.79893*m, 4.79571*m,
      4.78895*m, 4.76494*m, 4.71112*m, 4.63091*m,
      4.38308*m, 3.89792*m, 2.64466*m, 0.494499*m,
      0.0556574*m
    };


  // myMPT2->AddProperty("ABSLENGTH", new G4MaterialPropertyVector()) ;
  // G4cout << "Energy" << "    " << "ABSLENGTH" << G4endl;  
  // for(G4int i=0;i<NENT; i++){
  //   G4double energy    = h_Planck*c_light/(LAMBDAABS[i]*nm) ;
  //   G4double abslength ;

  //   if (ABS[i] <= 0.0) {
  //     abslength = 1.0/kInfinity ;
  //   }
  //   else {
  //     abslength = -3.0*mm/(std::log(ABS[i]/100.0)) ;
  //     G4cout << energy/eV << "   " <<abslength/m << G4endl;
  //   }

  //   myMPT2->AddEntry("ABSLENGTH", energy, abslength);
  // }
  //
  G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
  myMPT2->AddProperty("RINDEX",       PhotonEnergy, RefractiveIndex2,nEntries)->SetSpline(true);
  myMPT2->AddProperty("ABSLENGTH",    PhotonEnergy, Absorption2,     nEntries);
  //air_showerから追加
  
  Acrylic->SetMaterialPropertiesTable(myMPT2);


/////////////////////////////////////////////
// Construct Material Properties Tables
/////////////////////////////////////////////

  const G4int NUMENTRIES = 2;

  // Energy bins
  G4double X_RINDEX[NUMENTRIES] = {h_Planck*c_light/lambda_max, h_Planck*c_light/lambda_min} ; 


  // Air
  G4double RINDEX_AIR[NUMENTRIES] = {1.000292, 1.000292} ; 

// Air refractive index at 20 oC and 1 atm (from PDG) 
  for(G4int j=0 ; j<NUMENTRIES ; j++){
    RINDEX_AIR[j] = RINDEX_AIR[j] + 2.73*std::pow(10.0,-4) ; 
    }

  G4MaterialPropertiesTable *MPT_Air = new G4MaterialPropertiesTable();
  MPT_Air->AddProperty("RINDEX", X_RINDEX, RINDEX_AIR, NUMENTRIES);
  air->SetMaterialPropertiesTable(MPT_Air);

  //空気
  //
  /*
    G4double RefractiveIndex4[nEntries] =
    { 1.0002758, 1.0002765, 1.0002773, 1.0002785,
    1.0002793, 1.0002801, 1.0002814, 1.0002827,
    1.0002846, 1.000287,  1.0002903, 1.0002947,
    1.0003018  };

    G4MaterialPropertiesTable* myMPT4 = new G4MaterialPropertiesTable();
    myMPT4->AddProperty("RINDEX",       PhotonEnergy, RefractiveIndex4,nEntries)->SetSpline(true);

    absorberMaterial2->SetMaterialPropertiesTable(myMPT4);
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* ACerenkovDetectorConstruction::DefineVolumes()
{
  G4Material* Vacuum = G4Material::GetMaterial("G4_Galactic");
  G4Material* air  = G4Material::GetMaterial("G4_AIR");
  G4Material* Acrylic = G4Material::GetMaterial("Acrylic");
  //  G4Material* MyVacuum = G4Material::GetMaterial("MyVacuum");
  // Sizes of the principal geometrical components (solids)


  //G4Material* water  = G4Material::GetMaterial("G4_WATER");

  // Definitions of Solids, Logical Volumes, Physical Volumes

  // World

  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(WorldLength_Y);

  G4cout << "Computed tolerance = "
	 << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
	 << " mm" << G4endl;

  G4Box* worldS
    = new G4Box("world",                                    //its name
		WorldLength_X/2,WorldLength_Y/2,WorldLength_Z/2); //its size
  G4LogicalVolume* worldLV
    = new G4LogicalVolume(
			  worldS,   //its solid
			  air,      //its material
			  "World"); //its name

  //  Must place the World Physical volume unrotated at (0,0,0).

  G4VPhysicalVolume* worldPV
    = new G4PVPlacement(
			0,               // no rotation
			G4ThreeVector(), // at (0,0,0)
			worldLV,         // its logical volume
			"World",         // its name
			0,               // its mother  volume
			false,           // no boolean operations
			0,               // copy number
			fCheckOverlaps); // checking overlaps
  
    //アクリル用----------------------------------------
  //

  G4Box* AcrylS 
    = new G4Box("Acryl",           // its name
		AcrylLength_X/2,AcrylLength_Y/2,AcrylLength_Z/2); // its size
                         
  fLogicAcrylD
    = new G4LogicalVolume(
                 AcrylS,           // its solid
                 Acrylic,  // its material
                 "Acryl");         // its name

 new G4PVPlacement(
			0,               // no rotation
			G4ThreeVector(), // at (0,0,0)
			fLogicAcrylD,         // its logical volume
			"Acryl",         // its name
			worldLV,               // its mother  volume
			false,           // no boolean operations
			0,               // copy number
			fCheckOverlaps); // checking overlaps


   G4Box* absBox 
    = new G4Box("absBox",           // its name
		absBox_X/2,absBox_Y/2,absBox_Z/2); // its size
                         
  fLogicabsBox
    = new G4LogicalVolume(
                 absBox,           // its solid
                 Vacuum,  // its material
                 "absBox");         // its name

 new G4PVPlacement(
			0,               // no rotation
			G4ThreeVector(0,-AcrylLength_Y/2 -absBox_Y/2,0), // at (0,0,0)
			fLogicabsBox,         // its logical volume
			"absBox",         // its name
			worldLV,               // its mother  volume
			false,           // no boolean operations
			0,               // copy number
			fCheckOverlaps); // checking overlaps

   fLogicabsBox_top
    = new G4LogicalVolume(
                 absBox,           // its solid
                 Vacuum,  // its material
                 "absBox");         // its name
   
                         
 new G4PVPlacement(
			0,               // no rotation
			G4ThreeVector(0,AcrylLength_Y/2 + absBox_Y/2,0), // at (0,0,0)
			fLogicabsBox_top,         // its logical volume
			"absBox",         // its name
			worldLV,               // its mother  volume
			false,           // no boolean operations
			0,               // copy number
			fCheckOverlaps); // checking overlaps


  
  // Visualization attributes

  G4VisAttributes* boxVisAtt_white= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  //G4VisAttributes* boxVisAtt_red= new G4VisAttributes(G4Colour(1.0,0.,0.));
  // G4VisAttributes* boxVisAtt_purple= new G4VisAttributes(G4Colour(1.,0.,1.));    
  G4VisAttributes* boxVisAtt_blue= new G4VisAttributes(G4Colour(0.,0.,1.));   
  worldLV      ->SetVisAttributes(boxVisAtt_white);
  fLogicAcrylD->SetVisAttributes(boxVisAtt_blue);



  //-------------------------------------------

  // Always return the physical world

  return worldPV;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....



//........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ACerenkovDetectorConstruction::ConstructSDandField()
{

  //有感検出器
  G4String SDname;
  auto aSV = new SensitiveVolume(SDname="/AorWdetector");
  //SetSensitiveDetector(fLogicAcrylD,aSV);
  fLogicAcrylD->SetSensitiveDetector(aSV);
  auto SDman = G4SDManager::GetSDMpointer();
  SDman->AddNewDetector(aSV);

  
  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue = G4ThreeVector();
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);

  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
}
