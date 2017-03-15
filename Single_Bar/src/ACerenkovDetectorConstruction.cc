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


//........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal G4GlobalMagFieldMessenger* ACerenkovDetectorConstruction::fMagFieldMessenger = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ACerenkovDetectorConstruction::ACerenkovDetectorConstruction()
  :G4VUserDetectorConstruction(),
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
  // 物質の定義
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

  //光子の波長に対する物質の屈折率、吸収長を入力
  const G4int nEntries = 13;
   
  G4double PhotonEnergy[nEntries] =
    { 1.7712*eV,  1.93725*eV, 2.13765*eV, 2.38431*eV,
      2.53029*eV, 2.6953*eV,  2.88335*eV, 3.0996*eV,
      3.35092*eV, 3.64659*eV, 3.99948*eV, 4.428*eV,   4.95936*eV };

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

  G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
  myMPT2->AddProperty("RINDEX",       PhotonEnergy, RefractiveIndex2,nEntries)->SetSpline(true);
  myMPT2->AddProperty("ABSLENGTH",    PhotonEnergy, Absorption2,     nEntries);
  
  Acrylic->SetMaterialPropertiesTable(myMPT2);

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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* ACerenkovDetectorConstruction::DefineVolumes()
{
  //定義した物質の呼び出し
  G4Material* Vacuum = G4Material::GetMaterial("G4_Galactic");
  G4Material* air  = G4Material::GetMaterial("G4_AIR");
  G4Material* Acrylic = G4Material::GetMaterial("Acrylic");

  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(WorldLength_Y);

  G4cout << "Computed tolerance = "
	 << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
	 << " mm" << G4endl;

  //Worldの定義
  G4Box* worldS
    = new G4Box("world",
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

  //アクリルのジオメトリの定義
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


  //光の吸収材を、アクリルの底面に設置
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

  //アクリルの上面に、光学過程を記録する仮想ジオメトリを設置
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


  //ジオメトリの配色を指定
  G4VisAttributes* boxVisAtt_white= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  //G4VisAttributes* boxVisAtt_red= new G4VisAttributes(G4Colour(1.0,0.,0.));
  // G4VisAttributes* boxVisAtt_purple= new G4VisAttributes(G4Colour(1.,0.,1.));    
  G4VisAttributes* boxVisAtt_blue= new G4VisAttributes(G4Colour(0.,0.,1.));   
  worldLV      ->SetVisAttributes(boxVisAtt_white);
  fLogicAcrylD->SetVisAttributes(boxVisAtt_blue);

  // Always return the physical world

  return worldPV;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....



//........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ACerenkovDetectorConstruction::ConstructSDandField()
{

  //有感検出器の宣言
  G4String SDname;
  auto aSV = new SensitiveVolume(SDname="/AorWdetector");
  //SetSensitiveDetector(fLogicAcrylD,aSV);
  fLogicAcrylD->SetSensitiveDetector(aSV);
  auto SDman = G4SDManager::GetSDMpointer();
  SDman->AddNewDetector(aSV);

  G4ThreeVector fieldValue = G4ThreeVector();
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);

  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
}
