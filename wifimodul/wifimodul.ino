#define ag_ismi "Azizg"
#define ag_sifresi "erkan1965"
#define IP "184.106.153.149"    //thingspeak.com IP adresi
 
float sicaklik;
 
void setup()
{
  Serial.begin(115200); //Seriport'u açıyoruz. Güncellediğimiz 
                        //ESP modülünün baudRate değeri 115200 olduğu için bizde Seriport'u 115200 şeklinde seçiyoruz
  
  Serial.println("AT"); //ESP modülümüz ile bağlantı kurulup kurulmadığını kontrol ediyoruz.
  
  delay(3000); //ESP ile iletişim için 3 saniye bekliyoruz.
 analogReference(INTERNAL);
  if(Serial.find("OK")){         //esp modülü ile bağlantıyı kurabilmişsek modül "AT" komutuna "OK" komutu ile geri dönüş yapıyor.
     Serial.println("AT+CWMODE=1"); //esp modülümüzün WiFi modunu STA şekline getiriyoruz. Bu mod ile modülümüz başka ağlara bağlanabilecek.
     delay(2000);
     String baglantiKomutu=String("AT+CWJAP=\"")+Azizg+"\",\""+erkan1965+"\"";
    Serial.println(baglantiKomutu);
    
     delay(5000);
 }
}
 
void loop(){
  float sicaklik = analogRead(A0) / 9.31; 
 Serial.println(sicaklik);
 sicaklik_yolla(sicaklik);
 // dakikada 1 güncellenmesi için 1 dakika bekle
 delay(60000);
}
 
 
void sicaklik_yolla(float sicaklik){
 Serial.println(String("AT+CIPSTART=\"TCP\",\"") + IP + "\",80");  //thingspeak sunucusuna bağlanmak için bu kodu kullanıyoruz. 
                                                                   //AT+CIPSTART komutu ile sunucuya bağlanmak için sunucudan izin istiyoruz. 
                                                                   //TCP burada yapacağımız bağlantı çeşidini gösteriyor. 80 ise bağlanacağımız portu gösteriyor
 delay(1000);
  if(Serial.find("Error")){     //sunucuya bağlanamazsak ESP modülü bize "Error" komutu ile dönüyor.
   Serial.println("AT+CIPSTART Error");
    return;
  }
  
 String yollanacakkomut = "GET /update?key=8WNSOH764O82FTX8&field1=";   // Burada 64T0OS3R1OEAYUML yazan kısım bizim API Key den aldığımız Key. Siz buraya kendi keyinizi yazacaksınız.
 yollanacakkomut += (int(sicaklik));                                      // Burada ise sıcaklığımızı float değişkenine atayarak yollanacakkomut değişkenine ekliyoruz.
 yollanacakkomut += "\r\n\r\n";                                             // ESP modülümüz ile seri iletişim kurarken yazdığımız komutların modüle iletilebilmesi için Enter komutu yani
  delay(3000);                                                                                // /r/n komutu kullanmamız gerekiyor.
 
 Serial.print("AT+CIPSEND=");                    //veri yollayacağımız zaman bu komutu kullanıyoruz. Bu komut ile önce kaç tane karakter yollayacağımızı söylememiz gerekiyor.
 Serial.println(yollanacakkomut.length()+2);       //yollanacakkomut değişkeninin kaç karakterden oluştuğunu .length() ile bulup yazırıyoruz.
 
 delay(1000);
 
 if(Serial.find(">")){                           //eğer sunucu ile iletişim sağlayıp komut uzunluğunu gönderebilmişsek ESP modülü bize ">" işareti ile geri dönüyor.
                                                 // arduino da ">" işaretini gördüğü anda sıcaklık verisini esp modülü ile thingspeak sunucusuna yolluyor.
 Serial.print(yollanacakkomut);
 Serial.print("\r\n\r\n");
 }
 else{
 Serial.println("AT+CIPCLOSE");
 }
}
