void turn_right(){
    copro_setSpeed(20,-10);
    _delay_ms(1000);
    copro_stopImmediate();
}

void turn_left(){
    copro_setSpeed(-20,20);
    _delay_ms(550);
    copro_stopImmediate();
}
