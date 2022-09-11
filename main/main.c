#include <main.h>
#include <SD.c>
#include <DS1307.c>
#include <DHT22.c>
#include <BMP085.c>
#include <ads1115.c>
////////////////////////////////////////////////////////////////////////////////////////
RTC_Time *mytime;
int Humss,Tempss;
unsigned int Luz;
float Temperatura,Humedad,Temps,Presions,vbat;
char txt[50],a1=0,a2=0,a3=0;
const int8 comillas = '"';
long Press=0;

void rtc_print(){
  printf("TIME: %02u:%02u:%02u", mytime->hours, mytime->minutes, mytime->seconds);
  printf(" DATE: %02u/%02u/20%02u\r\n", mytime->day, mytime->month, mytime->year);
}
////////////////////////////////////////////////////////////////////////////////////////
void main(){
   set_tris_c(0x00);
   mytime = RTC_Get();
   rtc_print();
   delay_ms(1000);  // wait a second
   while(1){
      output_low(PIN_C6);
      delay_ms(100);
      output_high(PIN_C6);
      delay_ms(20000);  // wait a second
      delay_ms(20000);  // wait a second
      a1=0;
      a3=0;
      mytime = RTC_Get();
      rtc_print();
////////////////////////////////////////////////////////////////////////////////////////  
      Humedad=dat_DHT22(1);             
      printf("Humedad: %0.1f\r\n",Humedad);
      delay_ms(500);
 ////////////////////////////////////////////////////////////////////////////////////////    
      bmp085Calibration();
      delay_ms(500);
      Temps = BMP085Temperature(); 
      Presions = BMP085Pressure();  //  calls for temperature first
      delay_ms(100);
      printf("Temperatura: %.3g\r\nPresion: %.3g\r\n",Temps,Presions);
      delay_ms(1000);
////////////////////////////////////////////////////////////////////////////////////////  
      delay_ms(10);
      datov0();
      delay_ms(10);
      datov1();
      delay_ms(10);   
      Luz=(v1*51.2);
      if(Luz<0){
         Luz=Luz*(-1);
      }  
      vbat=v2*2;
      printf("Luz: %u  Vbat: %f",Luz,vbat);
////////////////////////////////////////////////////////////////////////////////////////
      delay_ms(100);
      initialize();
      delay_ms(100);

      open_doc();
      sprintf(txt, "%02u/%02u/20%02u %02u:%02u | %0.1f | %0.1f | %0.1f ",mytime->day,mytime->month,mytime->year,mytime->hours, mytime->minutes,Humedad,Temps,Presions);
      if(fatputs(txt, &myfile) == 0){
        printf(txt);
      }
      else{
        printf("  writing error \r\n");
      }
      
      enter();
      close_doc();
      
      open_doc();
      sprintf(txt, "| %u | %f \r\n",Luz,vbat);
      if(fatputs(txt, &myfile) == 0){
        printf(txt);
      }
      else{
        printf("  writing error \r\n");
      }
      
      enter();
      close_doc();
      
      delay_ms(1000);
      Humss=(int)Humedad;
      Tempss=(int)Temps;
      Press=(long)Presions;
////////////////////////////////////////////////////////////////////////////////////////  
      delay_ms(8000);
      printf("AT+CIPSTART=%cTCP%c,%capi.thingspeak.com%c,80\r\n",comillas,comillas,comillas,comillas);
      delay_ms(8000);
      printf("AT+CIPMODE=1\r\n");
      delay_ms(8000);
      printf("AT+CIPSEND\r\n");
      delay_ms(8000);
      output_high(PIN_C6);
//      printf("GET https://api.thingspeak.com/update?api_key=S0Q18RYEFRGYNGMG&field1=%u&field2=%u&field3=%lu&field4=%d\r\n",Humss,Tempss,Press,Luz);
      printf("GET https://api.thingspeak.com/update?api_key=J41AH1ZY79SQ96Y9");
      printf("&field1=%u&field2=%u&field3=%lu&field4=%u\r\n",Humss,Tempss,Press,Luz);
      output_high(PIN_C6);
      delay_ms(3000);
      output_low(PIN_C6);
      delay_ms(3000);
      output_high(PIN_C6);
      delay_ms(3000);
      printf("AT+GSLP=1\r\n");
      delay_ms(3000);
      printf("AT+GSLP=1\r\n");
      delay_ms(3000);
////////////////////////////////////////////////////////////////////////////////////////      
      mytime = RTC_Get();
      a2=(mytime->minutes)+20;
      printf("%u %u %u %u",a2,mytime->minutes,a1,a3);
      if(a2>=60){
          a2=a2-60;
          a3=1;
       }
      while(a1==0){
         mytime = RTC_Get();
         delay_ms(10000);
         if(a3==1 && (a2>=mytime->minutes)){
            a1=1;
         }
         if(a3==0 && (a2<=mytime->minutes)){
            a1=1;
         }
         printf("%u %u %u %u",a2,mytime->minutes,a1,a3);
      }
   }
}
