void draw(Int_t pdg)
{
    TStyle* style = new TStyle("style","my_style");
//    style->SetTitleAlign(33);
    style->SetTitleSize(0.04,"X");
    style->SetTitleSize(0.04,"Y");
    style->SetTitleOffset(1.6,"Y");
    style->SetTitleOffset(1.0,"X");
    style->SetFrameLineWidth(2);
    style->SetFrameFillColor(0);
    style->SetPadColor(0);
    style->SetLabelSize(0.04,"X");
    style->SetLabelSize(0.04,"Y");
    style->SetPadTopMargin(0.1);
    style->SetPadBottomMargin(0.18);
    style->SetPadLeftMargin(0.18);
    style->SetPadTopMargin(0.1);
    style->SetPadRightMargin(0.1);
    style->SetMarkerSize(1.5);
    style->SetErrorX(0);
    style->SetOptStat(0);
    style->SetCanvasColor(0);
    style->SetTitleFillColor(0);
    style->SetTitleBorderSize(0);
    style->SetHistLineWidth(3);
    style->SetLineWidth(2);
    style->SetCanvasBorderMode(0);
    style->SetLegendBorderSize(0);
    style->SetPadBorderMode(0);
    //style->SetTitleAlign(13);
    gROOT->ForceStyle();
    style->cd();

    TLine* line = new TLine(-1,0,1,0);
    TFile* f = new TFile(Form("flow_%i.root",pdg));
    TProfile* profile[12];
    // y vs v1, mc
    for(int i=0;i<3;i++)
    {
       Int_t b = i%3;
       profile[i] = (TProfile*) f->Get( Form("y vs v1, mc, centrality %i",b) );
           profile[0]->SetTitle("mc-tracks");
       profile[i]->GetXaxis()->SetTitle("rapidity, y");
       profile[i]->GetYaxis()->SetTitle("strait flow, v_{1}");
//       profile[i]->GetXaxis()->SetNdivisions(504);
       profile[i]->SetMarkerStyle(21);
       profile[i]->SetMarkerColor(4);
       profile[i]->SetLineColor(4);
    }
    // y vs v1, mpd
    for(int i=3;i<6;i++)
    {
       Int_t b = i%3;
       profile[i] = (TProfile*) f->Get( Form("y vs v1, mpd, centrality %i",b) );
       profile[0]->SetTitle("mpd-tracks");
//       profile[i]->GetXaxis()->SetNdivisions(504);
       profile[i]->SetMarkerStyle(20);
       profile[i]->SetMarkerColor(2);
       profile[i]->SetLineColor(2);
    }
    // pt vs v1, mc
    for(int i=6;i<9;i++)
    {
       Int_t b = i%3;
       profile[i] = (TProfile*) f->Get( Form("pt vs v1, mc, centrality %i",b) );
       profile[i]->GetXaxis()->SetTitle("rapidity, y");
       profile[i]->GetYaxis()->SetTitle("v_{1}");
       profile[i]->GetXaxis()->SetNdivisions(504);
       profile[i]->SetMarkerStyle(21);
       profile[i]->SetMarkerColor(4);
       profile[i]->SetLineColor(4);
    }
   // pt vs v1, mpd
    for(int i=9;i<12;i++)
    {
       Int_t b = i%3;
       profile[i] = (TProfile*) f->Get( Form("pt vs v1, mpd, centrality %i",b) );
       profile[i]->GetXaxis()->SetNdivisions(504);
       profile[i]->SetMarkerStyle(20);
       profile[i]->SetMarkerColor(2);
    }

    TCanvas* canv = new TCanvas("canv","Main canvas",1500,700);
    TPad* pad1 = new TPad("pad1","Pad 1",0.   ,0.1 ,0.33,0.9); //left pad
    TPad* pad2 = new TPad("pad2","Pad 2",0.33 ,0.1 ,0.66,0.9); //middle pad
    TPad* pad3 = new TPad("pad3","Pad 3",0.66 ,0.1 ,1.  ,0.9); //right pad
    TLatex* ltext = new TLatex();
    canv->cd();
    pad1->Draw();
    pad2->Draw();
    pad3->Draw();
    auto legend = new TLegend(0.40,0.05,0.60,0.1);
    legend -> AddEntry(profile[0],"mc-tracks");
    legend -> AddEntry(profile[3],"mpd-tracks");

    pad1->cd();
    profile[0]->SetTitle("0<b<5 [fm]");
    profile[0]->Draw();
    profile[3]->Draw("Same");
    line->Draw("Same");
    pad2->cd();
    profile[1]->SetTitle("5<b<9 [fm]");
    profile[1]->Draw();
    profile[4]->Draw("Same");
    line->Draw("Same");
    pad3->cd();
    profile[2]->SetTitle("9<b<16 [fm]");
    profile[2]->Draw();
    profile[5]->Draw("Same");
    line->Draw("Same");
    canv->cd();
    legend -> Draw();
    ltext->DrawLatex(0.4,0.93,"y vs v_{1} for protons");

}