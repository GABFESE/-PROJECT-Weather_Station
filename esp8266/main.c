#include <main.h>
const int8 comillas = '"';
int a=0,b=0;

void main(){  
   printf("AT\r\n");
   delay_ms(10000);
   printf("AT+CIPSTART=%cTCP%c,%capi.thingspeak.com%c,80\r\n",comillas,comillas,comillas,comillas);
   delay_ms(1000);
   printf("AT+CIPMODE=1\r\n");
   delay_ms(1000);
   printf("AT+CIPSEND\r\n");
   delay_ms(1000);
   while(1){
      printf("GET https://api.thingspeak.com/update?api_key=S0Q18RYEFRGYNGMG&field1=%u&field2=%u\r\n",a,b);
      a=a+20;
      b=b+20;
      delay_ms(20000);
   }
}
