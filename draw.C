void draw(Int_t pdg)
{
    TStyle* style = new TStyle("style","my_style");
    style->SetTitleSize(0.08,"X");
    style->SetTitleSize(0.08,"Y");
    style->SetTitleOffset(1.1,"Y");
    style->SetTitleOffset(0.9,"X");
    style->SetFrameLineWidth(2);
    style->SetFrameFillColor(0);
    style->SetPadColor(0);
    style->SetLabelSize(0.06,"X");
    style->SetLabelSize(0.06,"Y");
    style->SetPadTopMargin(0.1);
    style->SetPadBottomMargin(0.18);
    style->SetPadLeftMargin(0.18);
    style->SetPadTopMargin(0.1);
    style->SetPadRightMargin(0.1);
    //style->SetMarkerSize(2.);
    style->SetErrorX(0);
    style->SetOptStat(0);
    style->SetCanvasColor(0);
    style->SetTitleFillColor(0);
    style->SetTitleBorderSize(0);
    //style->SetMarkerStyle(20);
    style->SetHistLineWidth(3);
    style->SetLineWidth(2);
    style->SetCanvasBorderMode(0);
    style->SetLegendBorderSize(0);
    style->SetPadBorderMode(0);
    //style->SetTitleAlign(13);
    gROOT->ForceStyle();
    style->cd();

    TFile* f = new TFile(Form("flow_%i.root",pdg));
    TProfile* profile[12];
    // y vs v1, mc
    for(int i=0;i<3;i++)
    {
       Int_t b = i%3;
       profile[i] = (TProfile*) f->Get( Form("y vs v1, mc, centrality %i",b) );
       profile[i]->GetXaxis()->SetNdivisions(504);
       profile[i]->SetMarkerStyle(21);
       profile[i]->SetMarkerColor(3);
    }
    // y vs v1, mpd
    for(int i=3;i<6;i++)
    {
       Int_t b = i%3;
       profile[i] = (TProfile*) f->Get( Form("y vs v1, mpd, centrality %i",b) );
       profile[i]->GetXaxis()->SetNdivisions(504);
       profile[i]->SetMarkerStyle(22);
       profile[i]->SetMarkerColor(4);
    }
    // pt vs v1, mc
    for(int i=6;i<9;i++)
    {
       Int_t b = i%3;
       profile[i] = (TProfile*) f->Get( Form("pt vs v1, mc, centrality %i",b) );
       profile[i]->GetXaxis()->SetNdivisions(504);
       profile[i]->SetMarkerStyle(21);
       profile[i]->SetMarkerColor(3);
    }
   // pt vs v1, mpd
    for(int i=9;i<12;i++)
    {
       Int_t b = i%3;
       profile[i] = (TProfile*) f->Get( Form("pt vs v1, mpd, centrality %i",b) );
       profile[i]->GetXaxis()->SetNdivisions(504);
       profile[i]->SetMarkerStyle(22);
       profile[i]->SetMarkerColor(4);
    }

    TCanvas* canv = new TCanvas("canv","Main canvas",1500,500);
    TPad* pad1 = new TPad("pad1","Pad 1",0.   ,0. ,0.33,0.9); //left pad
    TPad* pad2 = new TPad("pad2","Pad 2",0.33 ,0. ,0.66,0.9); //middle pad
    TPad* pad3 = new TPad("pad3","Pad 3",0.66 ,0. ,1.  ,0.9); //right pad
    canv->cd();
    pad1->Draw();
    pad2->Draw();
    pad3->Draw();

    pad1->cd();
    profile[0]->Draw();
    profile[3]->Draw("Same");
    pad2->cd();
    profile[1]->Draw();
    profile[4]->Draw("Same");
    pad3->cd();
    profile[2]->Draw();
    profile[5]->Draw("Same");

}