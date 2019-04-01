
#include <Fuzzy.h>
#include <FuzzyComposition.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzyOutput.h>
#include <FuzzyRule.h>
#include <FuzzyRuleAntecedent.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzySet.h>

Fuzzy* fuzzy = new Fuzzy();

const int trigPin = 8;
const int echoPin = 12;

long duration;
int distance;
//--------------------------------//

#include <SoftwareSerial.h>
#define RX 9 // 10
#define TX 10 //11
String AP = "Pikaa";       // CHANGE ME
String PASS = "pavsakul1234"; // CHANGE ME
String API = "FMNO9C4J1Z7J95V3";   // CHANGE ME
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field1";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;

SoftwareSerial esp8266(RX,TX); 
SoftwareSerial mySerial(0,1);//tx rx // changed from 3,4 to 0,1 

//-------------------------//

void setup() {
  // put your setup code here, to run once:
    pinMode(3,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(11,OUTPUT);

  fuzzy_rules();
     //--------------------ULTRASONIC SENSOR---------
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  
  Serial.begin(9600);
  mySerial.begin(9600);
  /*----- wifi stuff again */
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
  countTrueCommand = 0;

}
void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }


void loop() {
  // put your main code here, to run repeatedly:
  motorspeed(255);
  Serial.println("running");

   digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    
    // Calculating the distance
    distance= duration*0.034/2;
    
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance);


   valSensor = distance;
// valSensor = getSensorData();
 String getData = "GET /update?api_key="+ API +"&"+ field +"="+String(valSensor);
sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
 Serial.println("cloud");


}

void motorspeed(int n)
{
  analogWrite(3, n);
  analogWrite(5, 0);
  analogWrite(6, n);
  analogWrite(11, 0);
}

void distance_calc(){

   digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    
    // Calculating the distance
    distance= duration*0.034/2;
    
    // Prints the distance on the Serial Monitor
    Serial.println("Distance: ");
    Serial.println(distance);

  
  }

void fuzzy_rules(){
  
   FuzzyInput* distance = new FuzzyInput(1);// With its ID in param
 
  // Creating the FuzzySet to compond FuzzyInput distance
 FuzzySet* small = new FuzzySet(0, 0, 20, 40); // Small distance
 distance->addFuzzySet(small); // Add FuzzySet small to distance
 FuzzySet* moderate = new FuzzySet(30, 50, 50, 70); // Safe distance
 distance->addFuzzySet(moderate); // Add FuzzySet safe to distance
 FuzzySet* big = new FuzzySet(60, 80, 80, 80); // Big distance
 distance->addFuzzySet(big); // Add FuzzySet big to distance

 fuzzy->addFuzzyInput(distance); // Add FuzzyInput to Fuzzy object

// SPEED
// Step 3 - Creating a FuzzyInput speed
 FuzzyInput* carspeed = new FuzzyInput(2);// With its ID in param
 
  // Creating the FuzzySet to compond FuzzyInput carspeed
 FuzzySet* slow = new FuzzySet(0, 0, 60, 120); // Small carspeed
 carspeed->addFuzzySet(slow); // Add FuzzySet small to carspeed
 FuzzySet* average = new FuzzySet(90, 130, 130, 180); // Safe carspeed
 carspeed->addFuzzySet(average); // Add FuzzySet safe to carspeed
 FuzzySet* fast = new FuzzySet(160, 200, 200, 255); // Big carspeed
 carspeed->addFuzzySet(fast); // Add FuzzySet big to carspeed

 fuzzy->addFuzzyInput(carspeed); // Add FuzzyInput to Fuzzy object
 // OUTPUT

 // Passo 3 - Creating FuzzyOutput situation
 FuzzyOutput* situation = new FuzzyOutput(1);// With its ID in param

 // Creating FuzzySet to compond FuzzyOutput situation
 FuzzySet* safe = new FuzzySet(0, 0, 2, 3); // Slow situation
 situation->addFuzzySet(safe); // Add FuzzySet slow to situation
 FuzzySet* caution = new FuzzySet(2, 3, 4, 5); // Average situation
 situation->addFuzzySet(caution); // Add FuzzySet average to situation
 FuzzySet* threat = new FuzzySet(4, 6, 6, 8); // Fast situation
 situation->addFuzzySet(threat); // Add FuzzySet fast to situation
 FuzzySet* critical = new FuzzySet(7, 9, 10, 10); // very Fast situation
 situation->addFuzzySet(critical); // Add FuzzySet fast to situation

 fuzzy->addFuzzyOutput(situation); // Add FuzzyOutput to Fuzzy object

 //Passo 4 - Assembly the Fuzzy rules
 // FuzzyRule "IF distance = samll and carspeed = slow THEN situation = caution"
 FuzzyRuleAntecedent* ifDistanceSmallAndCarspeedSlow = new FuzzyRuleAntecedent();
ifDistanceSmallAndCarspeedSlow->joinWithAND(small, slow); 
 FuzzyRuleConsequent* thenSitCaution = new FuzzyRuleConsequent(); // Instantiating a Consequent to expression
 thenSitCaution->addOutput(caution);// Adding corresponding FuzzySet to Consequent object
 // Instantiating a FuzzyRule object
 FuzzyRule* fuzzyRule01 = new FuzzyRule(1, ifDistanceSmallAndCarspeedSlow, thenSitCaution); // Passing the Antecedent and the Consequent of expression
 
 fuzzy->addFuzzyRule(fuzzyRule01); // Adding FuzzyRule to Fuzzy object
 
 // FuzzyRule "IF distance = small and carspeed = average THEN situation = threat"
 FuzzyRuleAntecedent* ifDistanceSmallAndCarspeedAverage = new FuzzyRuleAntecedent(); // Instantiating an Antecedent to expression
 ifDistanceSmallAndCarspeedAverage->joinWithAND(small,average); // Adding corresponding FuzzySet to Antecedent object
 FuzzyRuleConsequent* thenSitThreat = new FuzzyRuleConsequent(); // Instantiating a Consequent to expression
 thenSitThreat->addOutput(threat); // Adding corresponding FuzzySet to Consequent object
 // Instantiating a FuzzyRule object
 FuzzyRule* fuzzyRule02 = new FuzzyRule(2, ifDistanceSmallAndCarspeedAverage, thenSitThreat); // Passing the Antecedent and the Consequent of expression
  
 fuzzy->addFuzzyRule(fuzzyRule02); // Adding FuzzyRule to Fuzzy object
 
 // FuzzyRule "IF distance = small and carspeed = fast THEN sitaution = critical"
 FuzzyRuleAntecedent* ifDistanceSmallAndCarspeedFast = new FuzzyRuleAntecedent(); // Instantiating an Antecedent to expression
 ifDistanceSmallAndCarspeedFast->joinWithAND(small,fast); // Adding corresponding FuzzySet to Antecedent object
 FuzzyRuleConsequent* thenSitCritical = new FuzzyRuleConsequent(); // Instantiating a Consequent to expression
 thenSitCritical->addOutput(critical);// Adding corresponding FuzzySet to Consequent object
 // Instantiating a FuzzyRule object
 FuzzyRule* fuzzyRule03 = new FuzzyRule(3, ifDistanceSmallAndCarspeedFast, thenSitCritical); // Passing the Antecedent and the Consequent of expression
 
 fuzzy->addFuzzyRule(fuzzyRule03); // Adding FuzzyRule to Fuzzy object

  // FuzzyRule "IF distance = moderate and carspeed = slow THEN sitaution = safe"
 FuzzyRuleAntecedent* ifDistanceModerateAndCarspeedSlow = new FuzzyRuleAntecedent(); // Instantiating an Antecedent to expression
 ifDistanceModerateAndCarspeedSlow->joinWithAND(moderate,slow); // Adding corresponding FuzzySet to Antecedent object
 FuzzyRuleConsequent* thenSitSafe = new FuzzyRuleConsequent(); // Instantiating a Consequent to expression
 thenSitSafe->addOutput(safe);// Adding corresponding FuzzySet to Consequent object
 // Instantiating a FuzzyRule object
 FuzzyRule* fuzzyRule04 = new FuzzyRule(4, ifDistanceModerateAndCarspeedSlow, thenSitSafe); // Passing the Antecedent and the Consequent of expression
 
 fuzzy->addFuzzyRule(fuzzyRule04); // Adding FuzzyRule to Fuzzy object

  // FuzzyRule "IF distance = moderate and carspeed = average THEN sitaution = caution"
 FuzzyRuleAntecedent* ifDistanceModerateAndCarspeedAverage = new FuzzyRuleAntecedent(); // Instantiating an Antecedent to expression
 ifDistanceModerateAndCarspeedAverage->joinWithAND(moderate,average); // Adding corresponding FuzzySet to Antecedent object
 //FuzzyRuleConsequent* thenSitCaution = new FuzzyRuleConsequent(); // Instantiating a Consequent to expression
 thenSitCaution->addOutput(caution);// Adding corresponding FuzzySet to Consequent object
 // Instantiating a FuzzyRule object
 FuzzyRule* fuzzyRule05 = new FuzzyRule(5, ifDistanceModerateAndCarspeedAverage, thenSitCaution); // Passing the Antecedent and the Consequent of expression
 
 fuzzy->addFuzzyRule(fuzzyRule05); // Adding FuzzyRule to Fuzzy object

  // FuzzyRule "IF distance = moderate and carspeed = fast THEN sitaution = caution"
 FuzzyRuleAntecedent* ifDistanceModerateAndCarspeedFast = new FuzzyRuleAntecedent(); // Instantiating an Antecedent to expression
 ifDistanceModerateAndCarspeedFast->joinWithAND(moderate,fast); // Adding corresponding FuzzySet to Antecedent object
// FuzzyRuleConsequent* thenSitThreat = new FuzzyRuleConsequent(); // Instantiating a Consequent to expression
 thenSitThreat->addOutput(threat);// Adding corresponding FuzzySet to Consequent object
 // Instantiating a FuzzyRule object
 FuzzyRule* fuzzyRule06 = new FuzzyRule(6, ifDistanceModerateAndCarspeedFast, thenSitThreat); // Passing the Antecedent and the Consequent of expression
 
 fuzzy->addFuzzyRule(fuzzyRule06); // Adding FuzzyRule to Fuzzy object

   // FuzzyRule "IF distance = big and carspeed = slow THEN sitaution = safe"
 FuzzyRuleAntecedent* ifDistanceBigAndCarspeedSlow = new FuzzyRuleAntecedent(); // Instantiating an Antecedent to expression
 ifDistanceBigAndCarspeedSlow->joinWithAND(big,slow); // Adding corresponding FuzzySet to Antecedent object
// FuzzyRuleConsequent* thenSitSafe = new FuzzyRuleConsequent(); // Instantiating a Consequent to expression
 thenSitSafe->addOutput(safe);// Adding corresponding FuzzySet to Consequent object
 // Instantiating a FuzzyRule object
 FuzzyRule* fuzzyRule07 = new FuzzyRule(7, ifDistanceBigAndCarspeedSlow, thenSitSafe); // Passing the Antecedent and the Consequent of expression
 
 fuzzy->addFuzzyRule(fuzzyRule07); // Adding FuzzyRule to Fuzzy object

    // FuzzyRule "IF distance = big and carspeed = average THEN sitaution = safe"
 FuzzyRuleAntecedent* ifDistanceBigAndCarspeedAverage = new FuzzyRuleAntecedent(); // Instantiating an Antecedent to expression
 ifDistanceBigAndCarspeedSlow->joinWithAND(big,average); // Adding corresponding FuzzySet to Antecedent object
// FuzzyRuleConsequent* thenSitSafe = new FuzzyRuleConsequent(); // Instantiating a Consequent to expression
 thenSitSafe->addOutput(safe);// Adding corresponding FuzzySet to Consequent object
 // Instantiating a FuzzyRule object
 FuzzyRule* fuzzyRule08 = new FuzzyRule(8, ifDistanceBigAndCarspeedAverage, thenSitSafe); // Passing the Antecedent and the Consequent of expression
 
 fuzzy->addFuzzyRule(fuzzyRule08); // Adding FuzzyRule to Fuzzy object

   // FuzzyRule "IF distance = big and carspeed = fast THEN sitaution = caution"
 FuzzyRuleAntecedent* ifDistanceBigAndCarspeedFast = new FuzzyRuleAntecedent(); // Instantiating an Antecedent to expression
 ifDistanceBigAndCarspeedFast->joinWithAND(big,fast); // Adding corresponding FuzzySet to Antecedent object
 //FuzzyRuleConsequent* thenSitCaution = new FuzzyRuleConsequent(); // Instantiating a Consequent to expression
 thenSitCaution->addOutput(caution);// Adding corresponding FuzzySet to Consequent object
 // Instantiating a FuzzyRule object
 FuzzyRule* fuzzyRule09 = new FuzzyRule(9, ifDistanceBigAndCarspeedFast, thenSitCaution); // Passing the Antecedent and the Consequent of expression
 
 fuzzy->addFuzzyRule(fuzzyRule09); // Adding FuzzyRule to Fuzzy object

  
  }
