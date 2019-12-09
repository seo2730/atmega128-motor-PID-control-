/*
 * GccApplication1.c
 *
 * Created: 2019-08-28 오후 12:13:16
 * Author : CDSL
 */ 

#include "mcu_init.h"
#include "dataType.h"

#define dt 0.0005 //제어주기 시간
#define Kt 0.0683 // [N*m/A]

#define PI 3.14159265
#define GEAR_RATIO 81.0
#define CPT 1024.0

#define DEG2RAD PI/180
#define RAD2DEG 180/PI

#define P_deg	 2.15   //2   
#define D_deg	 0.085 //0.1

#define P_vel	 7.5 //10//7.5//2.5 // 
#define I_vel	 0.05 //150 //  
#define Ka_vel   1/P_vel

#define P_cur	 0.1316//1.5
#define I_cur	 352 //0.01	  
#define Ka_cur   1/P_cur

#define Test_cur 1.0
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum{
   CURRENT_CONTROL = 0x01,
   VELOCITY_CONTROL = 0x02,
   POSITION_CONTROL = 0x04
};

volatile int32_t g_Cnt, g_preCnt;

volatile int g_SendFlag = 0;

volatile double g_Pdes=0;
volatile double g_Vlimit = 0.;
volatile double g_Climit = 0.;

volatile Packet_t g_PacketBuffer;
volatile unsigned char g_PacketMode;
volatile unsigned char g_ID = 1;
volatile unsigned char checkSize;

volatile unsigned char g_buf[256], g_BufWriteCnt, g_BufReadCnt;

double Vlimit = 24;

double cur_cur = 0,tar_cur = 0,pre_cur = 0 ,error_cur_sum=0;
double error_cur = 0, anti_cur = 0;
volatile double current = 0;
int cur_Hz = 0;

double cur_vel = 0,tar_vel = 0,pre_vel = 0,error_vel_sum=0;
double error_vel = 0, anti_vel = 0;
volatile double speed =0;
int spe_Hz = 0;

double cur_degree = 0,tar_degree = 0,pre_degree = 0;
double error_deg = 0;
double pre_error_deg = 0;
volatile double position =0;
int pos_Hz = 0;

void SetDutyCW(double v){
   
   while(TCNT1  == 0);

   int ocr = v * (400. / 24.) + 400;
   
   if(ocr > OCR_MAX)   ocr = OCR_MAX;
   else if(ocr < OCR_MIN)   ocr = OCR_MIN;
   OCR1A = OCR1B = ocr;
//*/
/*
   int ocr = v * (800. / 24.);
   
   if(ocr > OCR_MAX)   ocr = OCR_MAX;
   else if(ocr < -OCR_MAX)   ocr = -OCR_MAX;
   
   if(ocr < 0){
      OCR1A = 0;      
      OCR1B = -ocr;
   }
   else{
      OCR1B = 0;
      OCR1A = ocr;
   }
//*/

}


void InitLS7366(){
   
   PORTB = 0x00;
   SPI_MasterSend(SELECT_MDR0 | WR_REG);
   SPI_MasterSend(X4_QUAD | FREE_RUN | DISABLE_INDEX | SYNCHRONOUS_INDEX |FILTER_CDF_1);
   PORTB = 0x01;
   
   PORTB = 0x00;
   SPI_MasterSend(SELECT_MDR1 | WR_REG);
   SPI_MasterSend(FOUR_BYTE_COUNT_MODE | ENABLE_COUNTING);
   PORTB = 0x01;
   
   PORTB = 0x00;
   SPI_MasterSend(SELECT_CNTR | CLR_REG);
   PORTB = 0x01;
}



ISR(USART0_RX_vect){
   g_buf[g_BufWriteCnt++] = UDR0;
}

ISR(TIMER3_OVF_vect){
         
   TCNT3 = 65536 - 125;

   //Read LS7366
   int32_t cnt;

   PORTB = 0x00;
   SPI_MasterSend(SELECT_OTR | LOAD_REG);
   PORTB = 0x01;
         
   PORTB = 0x00;
   SPI_MasterSend(SELECT_OTR | RD_REG);
   cnt = SPI_MasterRecv();      cnt = cnt<< 8;
   cnt |= SPI_MasterRecv();   cnt = cnt<< 8;
   cnt |= SPI_MasterRecv();   cnt = cnt<< 8;
   cnt |= SPI_MasterRecv();
   PORTB = 0x01;
   g_Cnt = -cnt;

   //spe_Hz = cur_Hz % 100;
   //pos_Hz = cur_Hz % 1000;
 
   cur_degree = g_Cnt*2*PI/(GEAR_RATIO*CPT*4);//g_Cnt*2*PI/(GEAR_RATIO*CPT*4);
   cur_vel = (cur_degree - pre_degree)*2000;
   cur_cur = 10.0*(GetADC(0)*5.0/1024.0-2.5);

   error_deg = g_Pdes - cur_degree;
   
   if(pos_Hz == 1000)// 0.005 0.05 0.3
   {
		pos_Hz = 0;
		position = P_deg*error_deg + D_deg*(error_deg - pre_error_deg)*2;
   }

   if(spe_Hz == 100)
   {
		spe_Hz = 0;
		Speed_Control(position,g_Vlimit);
		//Speed_Control(g_Vlimit,g_Vlimit);
   }

   if(cur_Hz == 10)
   {
		cur_Hz = 0;
		Current_Control(speed,g_Climit);
		//Current_Control(g_Climit,g_Climit);
   }

   if(g_Pdes == 0 && g_Vlimit == 0 && g_Climit == 0)
		SetDutyCW(0);
   else{
		SetDutyCW(current);
   }
   //////////////////////////////////////////
   pre_error_deg = error_deg;

   g_SendFlag++;
   pre_degree = cur_degree;

   cur_Hz++;
   spe_Hz++;
   pos_Hz++;
}

int main(void){

   /* Replace with your application code*/
   Packet_t packet;
   packet.data.header[0] = packet.data.header[1] = packet.data.header[2] = packet.data.header[3] = 0xFF; //0xFE;
   
   InitIO();
   
   //Uart
   InitUart0();
   
   //SPI
   InitSPI();
   
   //Timer
   InitTimer1();
   InitTimer3();

   
   OCR1A = 0;      //1 L
   OCR1B = 0;      //2 L
   SetDutyCW(0);
   TCNT1 = 0;
   
   //ADC
   InitADC();
   
   //LS7366
   InitLS7366();
   
   TCNT3 = 65536 - 125;
   sei();

   unsigned char check = 0;
   
    while (1) {
      for(;g_BufReadCnt != g_BufWriteCnt ;g_BufReadCnt++){

         switch (g_PacketMode)
         {
            case 0:
               if(g_buf[g_BufReadCnt] == 0xFF)
               {
                  checkSize++;
                  if (checkSize == 4)
                  {
                     g_PacketMode = 1;
                  }
               }

               else
               {
                  checkSize = 0;
               }
               break;

            case 1:

               g_PacketBuffer.buffer[checkSize++] = g_buf[g_BufReadCnt];

               if(checkSize == 8)
               {
                  if(g_PacketBuffer.data.id == g_ID)
                  {
                     g_PacketMode = 2;
                  }

                  else
                  {
                     g_PacketMode = 0;
                     checkSize = 0;
                  }
               }

               break;

            case 2:
               
               g_PacketBuffer.buffer[checkSize++] = g_buf[g_BufReadCnt];
               check += g_buf[g_BufReadCnt];

               if(checkSize == g_PacketBuffer.data.size)
               {
                  if(check == g_PacketBuffer.data.check)
                  {
                     switch(g_PacketBuffer.data.mode)
                     {
                        case 2:
                        g_Pdes = (double)g_PacketBuffer.data.pos / 1000.0;
                        g_Vlimit = (double)g_PacketBuffer.data.velo / 1000.0;
                        g_Climit = (double)g_PacketBuffer.data.cur / 1000.0;
                        break;
                     }
                  }
                  check = 0;
                  g_PacketMode = 0;
                  checkSize = 0;
               }

               else if(checkSize > g_PacketBuffer.data.size || checkSize > sizeof(Packet_t))
               {
                check = 0;
                g_PacketMode = 0;
                checkSize = 0;
               }
               
         }
      }

      if(g_SendFlag > 19){
         g_SendFlag = 0;   
         
         packet.data.id = g_ID;
         packet.data.size = sizeof(Packet_data_t);
         packet.data.mode = 3;
         packet.data.check = 0;
         
         packet.data.pos = cur_degree * 1000.0;
         packet.data.velo = cur_vel * 1000.0;
         packet.data.cur = cur_cur * 1000.0; //current
         
         for(int i = 8;i < sizeof(Packet_t);i++)
			packet.data.check += packet.buffer[i];
         
         for(int i=0;i<packet.data.size; i++){
            TransUart0(packet.buffer[i]);
         }
	
      }
      
   }
      
}

void Speed_Control(double tar_vel, volatile double v_limit)
{	
	if(tar_vel> v_limit)
		tar_vel = v_limit;
	else if(tar_vel< -v_limit)
		tar_vel = -v_limit;

	error_vel =  tar_vel - cur_vel;
	error_vel_sum += error_vel - Ka_vel * anti_vel;

	if(error_vel_sum > 642)
		error_vel_sum = 642;
	else if(error_vel_sum < -642)
		error_vel_sum = -642;
	
	speed = P_vel * error_vel + I_vel * error_vel_sum *0.01  + Kt * cur_cur;

	if(speed > 642){
		anti_vel = speed - 642;
		speed = 642;
	}
	else if(speed < -642){
		anti_vel = speed + 642;
		speed = -642;
	}
}

void Current_Control(double tar_cur, volatile double c_limit)
{
	if(tar_cur > c_limit)
		tar_cur = c_limit;
	else if(tar_cur < -c_limit)
		tar_cur = -c_limit;
	
	error_cur = tar_cur- cur_cur;
	error_cur_sum += error_cur - Ka_cur * anti_cur;


	if(error_cur_sum > 24.0)
		error_cur_sum = 24.0;
	else if(error_cur_sum < -24.0)
		error_cur_sum = -24.0;


	current = P_cur*error_cur + I_cur*error_cur_sum*dt + Kt*cur_vel;

	if(current>24.0){
		anti_cur = current - 24.0;
		current = 24.0;
	}
	else if(current<-24.0){
		anti_cur = current + 24.0;
		current = -24.0;
	}
}