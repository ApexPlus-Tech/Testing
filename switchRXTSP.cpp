#include <iostream>
#include <cstdlib>
#include <wiringPi.h>
#include "lime/LimeSuite.h"
#include "lime/LMS7002M.h"
#include "lime/Logger.h"
#include "lime/lms7_device.h"
using namespace std;
using namespace lime;
#define NANO_LMS_RESET 2
#define RFD_EXT_CLK 40e6

enum ARGS{
  MODE=1,
  CHAN=2,
  DCI=3,
  DCQ=4
};

enum ERRORS{
  NO_DEV=-1,
  NOT_OPEN=-2
};

lms_device_t* device = NULL;
int main(int argc,char** args){
  int n;
  lms_info_str_t list[8];
  //get device list
  //select channel
   if(argc<3){
	  cout<<"USAGE:sudo ./switchRXTSP 'normal' <CHANNEL 0,1 or 2)> OR \n sudo ./switchRXTSP 'tsg' <CHANNEL(0,1 or 2)> <DCI(0000 - ffff)> <DCQ(0000 - ffff)>\n";
          return -1;
  }
  if((n = LMS_GetDeviceList(list)) < 0){
        cout<<"Could not get devices\n";
        return NO_DEV;
  }
  if(LMS_Open(&device,list[0],NULL)){
        cout<<"Could not open device\n";
        return NOT_OPEN;
  }
  int channel = atoi(args[CHAN]);
  LMS_WriteParam(device,LMS7param(MAC),channel+1);

  string mode = args[MODE];
  if (mode == "normal"){
	cout<<"Normal Mode\n";
	//change input source of RXTSP to ADC output
	LMS_WriteParam(device,LMS7param(INSEL_RXTSP),0);
  }
  else if(mode == "tsg"){
	cout<<"TSG Mode\n";
	//set input source of RXTSP to test signal generator 
	 LMS_WriteParam(device,LMS7param(INSEL_RXTSP),1);
	//set tsg mode to dc source
	 LMS_WriteParam(device,LMS7param(TSGMODE_RXTSP),1);
	//set dci value
         long dci = strtol(args[DCI],NULL,16);
	 LMS_WriteParam(device,LMS7param(DC_REG_RXTSP),dci);
         LMS_WriteParam(device,LMS7param(TSGDCLDI_RXTSP),0);
         LMS_WriteParam(device,LMS7param(TSGDCLDI_RXTSP),1); 
	 LMS_WriteParam(device,LMS7param(TSGDCLDI_RXTSP),0);
         //set dcq value
	 long dcq = strtol(args[DCQ],NULL,16);	
	 LMS_WriteParam(device,LMS7param(DC_REG_RXTSP),dcq);
         LMS_WriteParam(device,LMS7param(TSGDCLDQ_RXTSP),0);
         LMS_WriteParam(device,LMS7param(TSGDCLDQ_RXTSP),1);
         LMS_WriteParam(device,LMS7param(TSGDCLDQ_RXTSP),0);
  }
  return 0;
}
