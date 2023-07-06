#include <avr/io.h>
#include <util/delay.h>
#define checkbit(x,y) ((x) & (y))
#define bitn(p) (0x01 << (p))
double distance;
int main(void)
{
    int count;
    DDRC = 0xFF;    // Set all pins of Port C as output
    DDRB = 0x01;    // Set pin 0 of Port B as output
    int pinstate;

    while (1)
    {
        trigger();  // Send trigger signal to the ultrasonic sensor

        // Wait for the ultrasonic sensor echo pin to be high, indicating detection of an object
        while (checkbit(PINB, bitn(1)))
        {
            TCCR1A = 0x00;
            TCCR1B = 0x02;
            pinstate = 1;
        }

        // Wait for the ultrasonic sensor echo pin to be low, indicating no object in range
        while (!checkbit(PINB, bitn(1)) && pinstate == 1)
        {
            TCCR1B = 0x00;
            count = TCNT1;
            distance = (count * 0.008575);

            compare();  // Compare the distance value and perform an action

            pinstate = 0;
            TCNT1 = 0;
        }
    }
}

void trigger()
{
    
    PORTB = 0x00;
    _delay_us(10);
    PORTB = 0x01;
    _delay_us(10);
    PORTB = 0x00;
}

void compare()
{
    
    if (distance < 10)
    {
        // If the distance is less than 10, set all pins of Port C to high (logic level 1)
        PORTC = 0xFF;
    }
    else
    {
        // If the distance is 10 or greater, set all pins of Port C to low (logic level 0)
        PORTC = 0x00;
    }
}
