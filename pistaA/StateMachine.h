// Dana Elizabeth Torres Estrada ._.
#ifndef STATEMACHINE_H
#define STATEMACHINE_H

int currentMapSide = 1; // La pista se divide en 4 lados, esta variable es el lado de la pista actual
int baseSpeed = 255; // Velocidad

// Definición de estados
enum State {
    START,
    FETCH_BALL,
    GRAB_BALL,
    FETCH_EXIT,
    FINISH
};

class StateMachine {
private:
    State currentState;
    void phaseStart();
    void phaseFetchBall();
    void phaseGrabBall();
    void phaseSearchExit();
    void phaseFinish();

public:
    StateMachine() : currentState(START) {} // Constructor de la clase

    // Controlador de estados
    void loop() {
        switch (currentState) {
            case START:
                phaseStart();
                break;
            case FETCH_BALL:
                phaseFetchBall();
                break;
            case GRAB_BALL:
                phaseGrabBall();
                break;
            case FETCH_EXIT:
                phaseSearchExit();
                break;
            case FINISH:
                phaseFinish();
                break;
        }
    }
};

#endif // STATEMACHINE_H