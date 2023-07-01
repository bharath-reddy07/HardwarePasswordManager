#include <Crypto.h>
#include <AES.h>
#include <string.h>

//key[16] cotain 16 byte key(128 bit) for encryption
byte key[16]="somethingrandom";
//plaintext[16] contain the text we need to encrypt
byte plaintext[16]="123456789012345";
//cypher[16] stores the encrypted text
byte cypher[16];
//decryptedtext[16] stores decrypted text after decryption
byte decryptedtext[16];
//creating an object of AES128 class
AES128 aes128;


void setup() {
  Serial.begin(115200);
  while(!Serial);
  aes128.setKey(key,16);// Setting Key for AES 
  
  Serial.print("Before Encryption:");
  for(int i=0; i<sizeof(plaintext)-1; i++){
    Serial.write(plaintext[i]);
   }
   
  aes128.encryptBlock(cypher,plaintext);//cypher->output block and plaintext->input block
  Serial.println();
  Serial.print("After Encryption:");
  for(int j=0;j<sizeof(cypher)-1;j++){
      Serial.print(char(cypher[j]));
      Serial.print(" ");
    }
    
   aes128.decryptBlock(decryptedtext,cypher);
   
  Serial.println();
  Serial.print("After Dencryption:");
  for(int i=0; i<sizeof(decryptedtext)-1; i++){
    Serial.write(decryptedtext[i]);
   }
   Serial.println();
   Serial.println();

}

void loop() {
  // put your main code here, to run repeatedly:

}
