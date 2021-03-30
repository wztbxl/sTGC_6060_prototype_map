const Int_t mMax = 100000;
struct StsTGCData
{
	// event information
	Int_t    mEventId;
	Int_t    nFireChannel;
	Int_t    Sec;
	Int_t    RDO;

	//channel information
	Int_t    Channel_Number[mMax];
	Int_t    Strip_Number[mMax];
	Int_t    FEB[mMax];
	Int_t    VMM[mMax];
	Int_t    Channel[mMax];
	Int_t    BCID[mMax];
	Int_t    ADC[mMax];

};
