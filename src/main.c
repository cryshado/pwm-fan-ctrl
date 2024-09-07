#ifndef __AVR_ATtiny85__
#define __AVR_ATtiny85__
#endif

#ifndef F_CPU
#error "F_CPU must be defined"
#endif

#include <stdint.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>

#define BTN1 PB0
#define BTN2 PB2

#define MINIDX 0
#define MAXIDX 4

uint8_t curridx = 0;

static inline void pb1_h() { PORTB |= (1 << PB1); }
static inline void pb1_l() { PORTB &= ~(1 << PB1); }
static inline void pwm_on() { TCCR0A |= (1 << COM0B1); }
static inline void pwm_off() { TCCR0A &= ~(1 << COM0B1); }
static inline void set_duty_cycle(uint8_t value) { OCR0B = value; }

void on_duty_update() {
    switch (curridx) {
    case 0:
        pb1_l();
        pwm_off();
        break;
    case 1:
        set_duty_cycle(20);
        pwm_on();
        break;
    case 2:
        set_duty_cycle(40);
        pwm_on();
        break;
    case 3:
        set_duty_cycle(59);
        pwm_on();
        break;
    case 4:
        pb1_h();
        pwm_off();
        break;
    }
}

void inc_duty_cycle() {
    curridx = curridx == MAXIDX ? MAXIDX : curridx + 1;
    on_duty_update();
}

void dec_duty_cycle() {
    curridx = curridx == MINIDX ? MINIDX : curridx - 1;
    on_duty_update();
}

void init_pwm() {
    // TOP = F_CPU / (Prescaler * F)
    // F = F_CPU / (Prescaler * (TOP + 1))
    // 16,000,000 / (8 * 25,000) - 1 = 79 (TOP value)
    // 16,000,000 / (8 * (79 + 1)) = 25,000 (i.e. 25kHz)

    DDRB |= _BV(PB1);

    // enable Fast PWM with TOP=OCRA (11.9.2; Table 11-5.)
    TCCR0A |= _BV(WGM01) | _BV(WGM00);
    TCCR0B |= _BV(WGM02);

    // non-inverting mode for OC0B will be enabled later
    // TCCR0A |= (1 << COM0B1);

    TCCR0B |= (1 << CS01); // set prescaler to /8

    OCR0A = 79; // set TOP value as `79`
    OCR0B = 0;  // set duty cycle (in range from 0 to 79)
}

void init_buttons() {
    DDRB &= ~(_BV(BTN1) | _BV(BTN2)); // set BTN1 and BTN2 as inputs
    GIMSK |= _BV(PCIE);               // enable pin change interrupts
    PCMSK |= _BV(BTN1) | _BV(BTN2);   // enable interrupts for BTN1 and BTN2
}

ISR(PCINT0_vect) {
    if (!(PINB & _BV(BTN1))) {
        inc_duty_cycle();
        return;
    }

    if (!(PINB & _BV(BTN2))) {
        dec_duty_cycle();
        return;
    }
}

int main(void) {
    init_pwm();
    init_buttons();

    sei(); // enable global interrupts

    while (1) {
    }
}
