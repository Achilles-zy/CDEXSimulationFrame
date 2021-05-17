void scan()
{
    TFile Distribution_File("Distribution_Results_NTuple.root");
    TTree* Handle;
    Distribution_File.GetObject("Distribution_Results",Handle);
    Handle->Scan("SiPM_0:SiPM_1:SiPM_2:SiPM_3:SiPM_4:SiPM_5:SiPM_6:SiPM_7:SiPM_8:SiPM_9:SiPM_10:SiPM_11:SiPM_12:SiPM_13:SiPM_14:SiPM_15:Total");

}