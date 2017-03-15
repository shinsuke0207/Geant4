//没となったマクロです

#include "ana.hh"
int return_Parent(int track);

int ana()
{
  TFile *_file0 = TFile::Open("result.root");
  const double  nm = TMath::Power(10, -6);
  // std::vector<int> PDGCodes;
  // std::vector<int> TrackIDs;
  // std::vector<int> ParentIDs;    
  // std::vector<double> Positions_x;
  // std::vector<double> Positions_y;
  // std::vector<double> Positions_z;
  // std::vector<double> WaveLengths;;

  
  PDGCode = 0;
  TrackID = 0;
  ParentID = 0;
  Pos_x = 0.;
  Pos_y = 0.;
  Pos_z = 0.;
  WaveLength = 0.;

  nt1->SetBranchAddress("PDGCode",&PDGCode);
  nt1->SetBranchAddress("TrackID",&TrackID);
  nt1->SetBranchAddress("ParentID",&ParentID);  
  nt1->SetBranchAddress("Pos_x",&Pos_x);
  nt1->SetBranchAddress("Pos_y",&Pos_y);
  nt1->SetBranchAddress("Pos_z",&Pos_z);  
  nt1->SetBranchAddress("WL",&WaveLength);
  
  int nEntry = nt1->GetEntries(); //treeのEntry数を取得して代入する

  for(int iEntry=0;iEntry<nEntry;++iEntry){ //すべてのEntryでループをまわす
    nt1->GetEntry(iEntry);
    PDGCodes.push_back(PDGCode);
    TrackIDs.push_back(TrackID);
    ParentIDs.push_back(ParentID);
    Positions_x.push_back(Pos_x);
    Positions_y.push_back(Pos_y);
    Positions_z.push_back(Pos_z);
    WaveLengths.push_back(WaveLength);
  }

   for(int iEntry=0;iEntry<nEntry;++iEntry){
     cout << iEntry << ", " << PDGCodes[iEntry] << ", " << TrackIDs[iEntry] << ", " << ParentIDs[iEntry] << ", " << Positions_x[iEntry] << ", " << Positions_y[iEntry] << ", " << Positions_z[iEntry] << ", " << WaveLengths[iEntry]  <<  endl;
     //   cout << return_Parent(TrackIDs[iEntry]) << endl;
     }

  // e-,mu-由来の光子の軌跡出力（壁面のみ）
  for(int i=0;i<nEntry;i++){
    if(PDGCodes[i] != 0){
      continue;
    }
    int pdg = return_Parent(TrackIDs[i]);
    if(pdg == 11 && Positions_z[i] == -100 && WaveLengths[i] <= 650*nm && WaveLengths[i] >= 300*nm){
      pos_ele_x.push_back(Positions_x[i]);
      pos_ele_y.push_back(Positions_y[i]);      
    }
    if(pdg == 13 && Positions_z[i] == -100 && WaveLengths[i] <= 650*nm && WaveLengths[i] >= 300*nm){
      pos_mu_x.push_back(Positions_x[i]);
      pos_mu_y.push_back(Positions_y[i]);
    }
    cout << i << " " << return_Parent(TrackIDs[i]) << endl;
  }

  TGraph *g1 = new TGraph(pos_ele_y.sizeof(), pos_ele_x, pos_ele_y);
  g1->Draw("P");
}

int return_Parent(int track)
{
  int nEntry = nt1->GetEntries(); //treeのEntry数を取得して代入する
  int p;
  int PDG;
  for(int i=0; i<nEntry;i++){
    if(track == TrackIDs[i]) p = ParentIDs[i];
  }
  for(int i=0; i<nEntry;i++){
    if(p == TrackIDs[i]) PDG = PDGCodes[i];
  }
  return PDG;
}
