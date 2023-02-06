/**********************************************************************************
* File: chair_fsm_lib.cpp
*
* Purpose: Global Library containing functions and macros definitions
*          for the finite state machine (FSM) 
**********************************************************************************/
#include "chair_fsm_lib.h"

FSM::FSM(SYS_STATES_T initial_state = DEFAULT_VAL)
{
    this->stateIndex = initial_state;
}                          

SYS_STATES_T FSM::getCurrentState()
{
    return stateIndex;
}

void FSM::setCurrentState(SYS_STATES_T new_state)
{
    if(new_state >= INVALID )/* Retain current state if invalid */
    {
        return;
    } 

    this->stateIndex = new_state;
}