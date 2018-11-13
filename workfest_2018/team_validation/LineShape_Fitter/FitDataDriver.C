{

  gROOT->ProcessLine(".L LineShape_Library.C++g");
  //(TString FitTag, TString dataFilePath, TString mcDataFilePath, TString accDenomFilePath,TString addCuts)
  
  //Nums.push_back("03")
  //flux_30732.root
  std::vector<TString> Nums;
    Nums.push_back("blah");
  /*//Nums.push_back("30734");
  //Nums.push_back("30735");
  //Nums.push_back("30736");
  //Nums.push_back("30737");
  //Nums.push_back("30738");
  //Nums.push_back("30739");
  //Nums.push_back("30740");
  //Nums.push_back("30741");
  Nums.push_back("30742");
  Nums.push_back("30743");
  Nums.push_back("30744");
  Nums.push_back("30745");
  Nums.push_back("30749");
  Nums.push_back("30754");
  Nums.push_back("30769");
  Nums.push_back("30770");
  Nums.push_back("30778");
  Nums.push_back("30779");
  Nums.push_back("30780");
  Nums.push_back("30783");
  Nums.push_back("30784");
  Nums.push_back("30785");
  Nums.push_back("30787");
  Nums.push_back("30788");*/

  for(int i=0;i<Nums.size();i++)
  {
  TString UnpaddedNum=Nums[i];
  TString RunNum="0"+UnpaddedNum;
  cout<<".x FitData.C(\"RunWKFEST\",\"/volatile/halld/home/tbritton/RECONv3_wkfest/RooDataSet/\",\"/volatile/halld/home/tbritton/RECONv3_wkfest_MC/RooDataSet/\",\"/w/halld-scifs17exp/halld2/home/tbritton/ANA/PipPim/LineShape_WKFESTreconv3/tree_thrown_WKFEST_v3.root\",\"./flux_WKFEST.root\",\"\")"<<endl;
  gROOT->ProcessLine(".x FitData.C(\"RunWKFEST\",\"/volatile/halld/home/tbritton/RECONv3_wkfest/RooDataSet/\",\"/volatile/halld/home/tbritton/RECONv3_wkfest_MC/RooDataSet/\",\"/w/halld-scifs17exp/halld2/home/tbritton/ANA/PipPim/LineShape_WKFESTreconv3/tree_thrown_WKFEST_v3.root\",\"./flux_WKFEST.root\",\"\")");
  
  //gROOT->ProcessLine(".x FitData.C(\"Run"+RunNum+"\",\"/volatile/halld/home/tbritton/RECONv3_wkfest/RooDataSet/RooDataSet_"+RunNum+".root\",\"/volatile/halld/home/tbritton/RECONv3_wkfest_MC/RooDataSet/RooDataSet_"+RunNum+".root\",\"/cache/halld/gluex_simulations/recon-2017_01-ver03/rho/trees/tree_thrown/merged/tree_thrown_"+RunNum+".root\",\"./flux_"+UnpaddedNum+".root\",\"\")");
  }

}
