#include<htc.h>
__CONFIG(0x1f7a);
#define _XTAL_FREQ 20000000
#define rs RC0
#define rw RC1
#define en RC2
#define Trigger RC3
#define Echo RC4

void delay();
void lcd_command(unsigned char data);
void lcd_data(unsigned char data);

unsigned long time_taken;
unsigned int distance;
void delay_ms(unsigned int milliseconds);
void delay();

void main() 
	{
    // Initialize TRIS registers for sensor pins
    TRISC3 = 0; // Trig pin (output)
   TRISC4 = 1; // Echo pin (input)

    TRISD = 0; // 8-bit data line for sending data to the LCD
    TRISC0 = 0; // Register select
    TRISC1 = 0; // Read/write operation
    TRISC2 = 0; // Enable bit operation

    PORTD = 0x00; // Port D is declared as an 8-bit data port

    lcd_command(0x38); // 5x8 Display line, 16x2 two lines, 8-bit data
    lcd_command(0x0C); // Display ON, Cursor OFF
    lcd_command(0x01); // Clear display
    lcd_command(0x80); // DDRAM address set to the first line


    while (1)
	     {
        // Clear Timer1 and trigger the sensor
       TMR1H = 0;
       TMR1L = 0;
        Trigger = 1;
        __delay_ms(5);
        Trigger = 0;

        // Wait for Echo pin to go high
       while (Echo == 0);

        // Start Timer1
       TMR1ON = 1;

        // Wait for Echo pin to go low
        while (Echo == 1);

        // Stop Timer1
       TMR1ON = 0;

        // Calculate distance using the formula
        time_taken = (TMR1L | (TMR1H << 8));
        distance = (0.0272* time_taken) /2;
        time_taken = time_taken / 58; // Calculate distance in centimeters
        lcd_command(0x80); // Set DDRAM address to the first line
        lcd_data('D');
        lcd_data('I');
        lcd_data('S');
        lcd_data('T');
        lcd_data('A');
        lcd_data('N');
        lcd_data('C');
        lcd_data('E');
        lcd_data(':');
        lcd_data(' ');
          // Display the distance value
        lcd_data((distance / 100) + '0');
        lcd_data(((distance / 10) % 10) + '0');
        lcd_data((distance % 10) + '0');
        lcd_data('c');
        lcd_data('m');
     

        // Delay before taking the next distance measurement (you may need to adjust this)
        delay_ms(10); // Delay for 1 second
    }
}
      
void delay_ms(unsigned int milliseconds) 
	{
    while (milliseconds > 0) 
	    {
        __delay_ms(1);
        milliseconds--;
    }
}

void delay() 
	{
    TMR1CS = 0; //  Timer1 clock select assigned
    T1CKPS0 = 1; //  Timer1 clock prescalar0
    T1CKPS1 = 1; //  Timer1 clock prescalar1
    TMR1H = 0X0B; //  Timer1 High
    TMR1L = 0XDC; //  Timer1 Low
    TMR1ON = 1; //  Timer1 is ON
    while (!TMR1IF); // Timer1 Interrupt flag is 1
    TMR1IF = 0; // Timer1 Interrupt flag is 0
    TMR1ON = 0; // Timer1 is OFF
}

void lcd_data(unsigned char data) 
	{
    rs = 1;
    rw = 0;
    en = 1;
    PORTD = data;
    __delay_ms(1);
    en = 0;
}

void lcd_command(unsigned char data) 
	{
    rs = 0;
    rw = 0;
    en = 1;
    PORTD = data;
    __delay_ms(1);
    en = 0;
}


