{
  TFile *_file0 = TFile::Open("result.root");
  const Int_t NRGBs = 5;    //色の区切りの数（区切り色　= RGB空間に打たれた点のさす色）
  const Int_t NCont = 255;  //グラデーションの段数
  //グラデーションの色を配列で指定
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 }; //stopはカラーバーでの位置（比率）。つまり、区切りの色がくる場所。
  Double_t Red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 }; //RGBは一般的なRGBの値を255で割って規格化（？）している。
  Double_t Green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 }; //縦に見る。カラーバーの位置0.00でのRGB=(0.00,0.00,0.51),
  Double_t Blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 }; //位置0.34でのRGB=(0.00,0.81,1.00),のようになっている。
  TColor::CreateGradientColorTable(NRGBs, stops, Red, Green, Blue, NCont);
  //グラデーションを作る。勝手にgStyle->SetPalette()してくれる
  gStyle->SetNumberContours(NCont);
  
  TCanvas *c1 = new TCanvas("CerenkovDistribution","",600,600);
  nt1->Draw("Pos_y:Pos_x >> h1(40,-500,500,40,-500,500)"," WaveLength <= 650.*10**(-6) && WaveLength >= 300.*10**(-6) && eventID == 52","colz");
  h1->GetXaxis()->SetTitle("x[mm]");
  h1->GetYaxis()->SetTitle("y[mm]");
  h1->GetYaxis()->SetTitleOffset(1.4);//Y軸タイトルの位置調整
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gPad->Update();
  TPaletteAxis *palette = (TPaletteAxis*)h1->GetListOfFunctions()->FindObject("palette");
  palette->SetX1NDC(1);
  palette->SetX2NDC(10); //位置を変えたり、縮めたり  

  // TCanvas *c1 = new TCanvas("CerenkovDistribution","",600,600);
  // nt1->Draw("Photon_y:Photon_x >> h1(40,-500,500,40,-500,500)","ParentID == 1 &&  WaveLength <= 650.*10**(-6) && WaveLength >= 300.*10**(-6) && Photon_z == -100","colz");
  // h1->GetXaxis()->SetTitle("x[mm]");
  // h1->GetYaxis()->SetTitle("y[mm]");
  // h1->GetYaxis()->SetTitleOffset(1.4);//Y軸タイトルの位置調整
  // gStyle->SetOptTitle(0);
  // gStyle->SetOptStat(0);
  // gPad->Update();
  // TPaletteAxis *palette = (TPaletteAxis*)h1->GetListOfFunctions()->FindObject("palette");
  // palette->SetX1NDC(1);
  // palette->SetX2NDC(10); //位置を変えたり、縮めたり  

}
