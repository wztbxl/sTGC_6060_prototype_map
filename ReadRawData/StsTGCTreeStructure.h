const Int_t mMax = 1000;
struct StsTGCData
{
	// event information
	Int_t    mEventId;
	Int_t    nFireChannel;

	//track information
	Int_t    Channel_Number[mMax];
	Int_t    Strip_Number[mMax];
	Int_t    nFireTimeBin[mMax];
	Int_t    TimeBin[mMax];
	Int_t    ADC[mMax];

};
