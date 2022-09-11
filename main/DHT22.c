#use fast_io(B)
#define DHT11_PIN PIN_B5  

short Time_out ;
unsigned int8 T_byte1, T_byte2, RH_byte1, RH_byte2, CheckSum ;
unsigned int16 Temp, RH;
float Temp2, RH2;

void start_signal(){
   output_drive(DHT11_PIN); 
   output_low(DHT11_PIN);   
   delay_ms(25);
   output_high(DHT11_PIN);  
   delay_us(30);
   output_float(DHT11_PIN); 
}

short check_response(){
  delay_us(40);
  if(!input(DHT11_PIN)){ 
    delay_us(80);
    if(input(DHT11_PIN)){
      delay_us(50);
      return 1;
    }
  }
}

unsigned int8 Read_Data(){
  unsigned int8 i, k, _data = 0;               // k is used to count 1 bit reading duration
  if(Time_out)
    break;
  for(i = 0; i < 8; i++){
    k = 0;
    while(!input(DHT11_PIN)){                          // Wait until pin goes high
      k++;
      if (k > 100) {Time_out = 1; break;}
      delay_us(1);}
    delay_us(30);
    if(!input(DHT11_PIN))
      bit_clear(_data, (7 - i));               // Clear bit (7 - i)
    else{
      bit_set(_data, (7 - i));                 // Set bit (7 - i)
      while(input(DHT11_PIN)){                         // Wait until pin goes low
      k++;
      if (k > 100) {Time_out = 1; break;}
      delay_us(1);}
    }
  }
  return _data;
}

float dat_DHT22(char selector){
   delay_ms(500);
   Time_out = 0;
   start_signal();
   if(check_response()){                     // If there is response from sensor
         RH_byte1 = Read_Data();                 // read RH byte1
         RH_byte2 = Read_Data();                 // read RH byte2
         T_byte1 = Read_Data();                  // read T byte1
         T_byte2 = Read_Data();                  // read T byte2
         CheckSum = Read_Data();                 // read checksum
         if(Time_out){                           // If reading takes long time
           printf("Time out!\r\n");
         }
         else{
          if(CheckSum == ((RH_byte1 + RH_byte2 + T_byte1 + T_byte2) & 0xFF)){
           RH = RH_byte1;
           RH = (RH << 8) | RH_byte2;
           Temp = T_byte1;
           Temp = (Temp << 8) | T_byte2;
           if (Temp > 0X8000){
            Temp = Temp & 0X7FFF;
            Temp = Temp*(-1);}
           else
           Temp2 = Temp;
           Temp2 = Temp2/10;
           RH2 = RH;
           RH2 = RH2/10;
           if(selector==0){
            return Temp2;
           }
           if(selector==1){
            return RH2;
           }
           }
           else {
             printf("Checksum Error!\r\n");
           }
         }
       }
       else {
         printf("No response from the sensor\r\n");
       }
}
