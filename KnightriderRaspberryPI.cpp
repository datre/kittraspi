/* The MIT License (MIT)

Copyright (c) 2016 Daniel Treitinger

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include <stdio.h>
#include <wiringPi.h> 
#define SPEED 50 //Da Uhrsprünglich für AVR Microcontroller anstelle von ADC digtialierten Wert von Poti nun konstanter Wert

int main(void)
{
    // Starte die WiringPi-Api
    if (wiringPiSetup()==(-1))
    {
        return 1;
    }
    //Legt folgende PINS als Ausgänge fest
    pinMode(0, OUTPUT);
	pinMode(1, OUTPUT);
	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(7, OUTPUT);
	pinMode(8, OUTPUT);
    unsigned short int i, a, e, runde=0,vergleichswerte[8],richtung=0, leds, adcwert=SPEED, zaehler=0;//Deklaration der Variablen
    for(i=0; i<8; i++)
    {
        vergleichswerte[i]=0;//Vergleichwerte bekommen alle den Wert null(eigentlich überflüssig)
    }
    while(1)
    {
        if(richtung==0)//Nach links
        {
            vergleichswerte[runde]=255;//Vergleichswert der aktuellen Runde erhält den Maximalen Wert
        }
        else if(richtung==1)//Nach rechts
        {
            vergleichswerte[7-runde]=255;
        }
        for(e=0; e<5+adcwert/20; e++)//läuft so oft durch wie es der Wert vom analogen Dreknopf/SPEED will
        {
            if(zaehler==10)
            {
                for(i=0; i<8; i++)
                {
                    vergleichswerte[i]/=2;//Vergleichswerte der acht Bits werden halbiert
                }
                if(richtung==0)//Nach links
                {
                    vergleichswerte[runde]=255;//Vergleichswert der aktuellen Runde erhält den Maximalen Wert
                }
                else if(richtung==1)//Nach rechts
                {
                    vergleichswerte[7-runde]=255;
                }
                zaehler=0;
            }
            for(a=0; a<256; a++)
            {
                leds=0;//leds wird komplett null, damit man wieder sauber mit oder Verknüpfen kann(Einsen würden Stören)
                for(i=0; i<8; i++)
                {
                    if(a<vergleichswerte[i])
                    {
                        leds|=0x1<<i;//schreibt eine eins an das i-te bit mit einem ODER eine 1 in leds, wenn a noch nicht den Vergleichswert von dem i-ten bit überschritten hat
                    }
                }
            digitalWrite(0,leds&1); //PIN 0 wird auf das 0te bit von der Variable Muster gesetzt
            digitalWrite(1,(leds&2)>>1);//PIN 1 wird auf das 1te bit von der Variable Muster gesetzt
            digitalWrite(2,(leds&4)>>2);//PIN 2 wird auf das 2te bit von der Variable Muster gesetzt
            digitalWrite(3,(leds&8)>>3);//PIN 3 wird auf das 3te bit von der Variable Muster gesetzt
            digitalWrite(4,(leds&16)>>4);//PIN 4 wird auf das 4te bit von der Variable Muster gesetzt
            digitalWrite(5,(leds&32)>>5);//PIN 5 wird auf das 5te bit von der Variable Muster gesetzt
            digitalWrite(7,(leds&64)>>6);//PIN 7 wird auf das 6te bit von der Variable Muster gesetzt
            digitalWrite(8,(leds&128)>>7);//PIN 8 wird auf das 7te bit von der Variable Muster gesetzt
            }
            zaehler++;
        }
        runde++; //Eine Runde ist vorrüber, beim nächsten durchlauf wird die nächste LED die hellste sein
        if(runde==8)//wenn die achte runde erreicht ist wird die richtung umgekehrt und die variable runde zurückgesetzt auf null
        {
            richtung=richtung==0?1:0;
            runde=0;
        }
    }
    return 0;
}


