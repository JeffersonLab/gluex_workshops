std::vector<string> list_files(const char *dirname="C:/root/folder/", const char *ext=".root");

void MergeRooDataSets(){
  
RooDataSet* FullSet;
  std::vector<string> ourFiles=list_files("./2017/",".root");

  cout<<ourFiles.size()<<endl;

  for(int i=0;i<ourFiles.size();i++)
  {
      TFile f(TString("./2017/"+TString(ourFiles[i])));
      f.cd();
      RooDataSet* fileSet=(RooDataSet*) gDirectory->Get("MassData");

      if(i==0)
      {
          FullSet=fileSet;
      }
      else
      {
          if(fileSet)
          {
            FullSet->append(*fileSet);
          }
      }

  }

    TFile ff("FullSet.root","RECREATE") ;
      FullSet->Write();
      ff.Close();

}
std::vector<string> list_files(const char *dirname, const char *ext=".root") 
{ 
    std::vector<string> Files;
    TSystemDirectory dir(dirname, dirname);
     TList *files = dir.GetListOfFiles(); 
     if (files) 
     { 
         TSystemFile *file; 
         TString fname;
          TIter next(files);
           while ((file=(TSystemFile*)next())) 
           { 
               fname = file->GetName(); 
               if (!file->IsDirectory() && fname.EndsWith(ext))
                { 
                    cout << fname.Data() << endl; 
                    Files.push_back(fname.Data());
                } 
            } 
        } 
        return Files;
}