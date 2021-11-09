
// Timer 1 Output COMPARE A Interrupt

ISR(TIMER4_COMPA_vect) {
    static uint8_t pwm_cnt=0;
    uint8_t tmp=0;

    OCR4A += (uint16_t)T_PWM;
        
    if (pwm_setting[0] > pwm_cnt) tmp |= (1<<0);
    if (pwm_setting[1] > pwm_cnt) tmp |= (1<<1);
    if (pwm_setting[2] > pwm_cnt) tmp |= (1<<2);
    if (pwm_setting[3] > pwm_cnt) tmp |= (1<<3);
    if (pwm_setting[4] > pwm_cnt) tmp |= (1<<4);
    if (pwm_setting[5] > pwm_cnt) tmp |= (1<<5);
    if (pwm_setting[6] > pwm_cnt) tmp |= (1<<6);
    if (pwm_setting[7] > pwm_cnt) tmp |= (1<<7);
    PWM_PORT = tmp;                         // PWMs aktualisieren
    if (pwm_cnt==(uint8_t)(PWM_STEPS-1))
        pwm_cnt=0;
    else
        pwm_cnt++;
}

void setupPwmMotor() {

    // PWM einstellen
    
    PWM_DDR = 0xFF;         // Port als Ausgang
    
    // Timer 1 OCRA1, als variablem Timer nutzen

    TCCR4B = 1;             // Timer läuft mit vollem Systemtakt
    TIMSK4 |= (1<<OCIE4A);   // Interrupt freischalten

    //const uint8_t t1[8]={64, 0, 3, 17, 150, 99, 5, 9};
    //memcpy(pwm_setting, t1, 8);
}
