#include <TFile.h>   // ROOTファイルを使用
#include <TH1D.h>
#include <TH1.h>     // ヒストグラムを使用
#include <TRandom.h> // 乱数を使用
#include <TCanvas.h> // キャンバス
#include <TApplication.h> // アプリケーションオブジェクト
#include <TTree.h>
#include <numeric>

#include <iostream>

using namespace std;

int count_unique(const vector<int> &vec);
  
int main(int argc, char* argv [])
{
  TFile *f = TFile::Open("result.root");
  vector<double> mu_length;
  double length = 0;
  vector<double> lengths;

  vector<int> eventIDs;
  vector<int> TrackIDs;
  vector<int> ParentIDs;
  vector<int> ParentCodes;
  vector<int> PDGCodes;
  vector<double> Pos_xs;
  vector<double> Pos_ys;
  vector<double> Pos_zs;      
  vector<double> WaveLengths;
  vector<double> Energys;
  vector<double> StLengths;

  vector<int> eleTrackIDs;
  vector<int> numele;
  
  Int_t eventID=0;    //ntupleからデータをとってくる変数を定義    
  Int_t TrackID=0;
  Int_t ParentID=0;
  Int_t ParentCode=0;
  Int_t PDGCode=0;
  Double_t Pos_x=0;    //ntupleからデータをとってくる変数を定義
  Double_t Pos_y=0;    //ntupleからデータをとってくる変数を定義
  Double_t Pos_z=0;    //ntupleからデータをとってくる変数を定義
  Double_t WaveLength=0;
  Double_t Energy=0;
  Double_t StLength=0;
  TTree *tr = (TTree*)f->Get("nt1");
  tr->SetBranchAddress("eventID",&eventID);
  tr->SetBranchAddress("TrackID",&TrackID);
  tr->SetBranchAddress("ParentID",&ParentID);
  tr->SetBranchAddress("ParentCode",&ParentCode);
  tr->SetBranchAddress("PDGCode",&PDGCode);
  tr->SetBranchAddress("Pos_x",&Pos_x);
  tr->SetBranchAddress("Pos_y",&Pos_y);
  tr->SetBranchAddress("Pos_z",&Pos_z);
  tr->SetBranchAddress("WaveLength",&WaveLength);
  tr->SetBranchAddress("Energy",&Energy);
  tr->SetBranchAddress("StLength",&StLength);
  
  for(Int_t i = 0;i < tr->GetEntries();i++){
    tr->GetEntry(i);
    eventIDs.push_back(eventID);
    TrackIDs.push_back(TrackID);
    ParentIDs.push_back(ParentID);
    ParentCodes.push_back(ParentCode);
    PDGCodes.push_back(PDGCode);
    Pos_xs.push_back(Pos_x);
    Pos_ys.push_back(Pos_y);
    Pos_zs.push_back(Pos_z);
    WaveLengths.push_back(WaveLength);
    Energys.push_back(Energy);
    StLengths.push_back(StLength);
    cout << 100.*i/tr->GetEntries() << "[%] " << "input done" <<  endl;
  }
  return 0;
};

int count_unique(const vector<int> &vec){
    vector<int> v;
    v = vec;
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    return v.size();
  }

