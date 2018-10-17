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
       profile[0]->SetTitle("");
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
    TPad* pad1 = new TPad("pad1","Pad 1",0.   ,0. ,0.33,0.92); //left pad
    TPad* pad2 = new TPad("pad2","Pad 2",0.33 ,0. ,0.66,0.92); //middle pad
    TPad* pad3 = new TPad("pad3","Pad 3",0.66 ,0. ,1.  ,0.92); //right pad
    TLatex* ltext = new TLatex();
    auto cent0 = new TLatex();
    auto cent1 = new TLatex();
    auto cent2 = new TLatex();
    canv->cd();
    pad1->Draw();
    pad2->Draw();
    pad3->Draw();
    auto legend = new TLegend(0.2,0.8,0.5,0.89);
    legend -> AddEntry(profile[0],"mc-tracks");
    legend -> AddEntry(profile[3],"mpd-tracks");

    pad1->cd();
    profile[0]->SetTitle("");
    profile[0]->Draw();
    profile[3]->Draw("Same");
    line->Draw("Same");
    legend -> Draw();
    cent0->DrawLatexNDC(0.42,0.86,"0 #leq b #leq 5 [fm]");
    pad2->cd();
    profile[1]->SetTitle("");
    profile[1]->Draw();
    profile[4]->Draw("Same");
    line->Draw("Same");
    legend -> Draw();
    cent1->DrawLatexNDC(0.42,0.86,"6 #leq b #leq 9 [fm]");
    pad3->cd();
    profile[2]->SetTitle("");
    profile[2]->Draw();
    profile[5]->Draw("Same");
    line->Draw("Same");
    legend -> Draw();
    cent2->DrawLatexNDC(0.4,0.86,"10 #leq b #leq 16 [fm]");
    canv->cd();
    switch (pdg)
    {
        case 2212:
            ltext->DrawLatex(0.4,0.93,"v_{1} vs y for p^{+}");
            break;
        case -2212:
            ltext->DrawLatex(0.4,0.93,"v_{1} vs y for p^{-}");
            break;
        case 211:
            ltext->DrawLatex(0.4,0.93,"v_{1} vs y for #pi^{+}");
            break;
        case -211:
            ltext->DrawLatex(0.4,0.93,"v_{1} vs y for #pi^{-}");
            break;
        case 321:
            ltext->DrawLatex(0.4,0.93,"v_{1} vs y for K^{+}");
            break;
        case -321:
            ltext->DrawLatex(0.4,0.93,"v_{1} vs y for protons");
            break;
        default:
            ltext->DrawLatex(0.4,0.93,Form("v_{1} vs y for pdg=%i",pdg));
    }
    canv->SaveAs(Form("stat/v1_vs_y_%i.png",pdg));

}