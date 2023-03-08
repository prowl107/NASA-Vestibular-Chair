/**********************************************************************************
* File: chair_fsm_lib.h
*
* Purpose: Global Library containing functions and macros definitions
*          for the finite state machine (FSM) 
**********************************************************************************/
#ifndef _CHAIR_FSM_LIB_
#define _CHAIR_FSM_LIB_
/* Library depencies */

/* General Macros*/

/* Types */
enum SYS_STATES_T{
    DEFAULT_VAL,
    SYS_INIT,
    PROCTOR_OP_RPM,
    PROCTOR_OP_DIRECTION,
    PROCTOR_OP_TIME,
    SYS_ARM_HOLD,
    ARMED_OPERATION,
    ARMED_TARGET_REACHED, 

    /***********************************************************************
    * NOTE: Invalide state used for error checking. Placed at the end to
    * utilize type enumeration auto incrementation. ADDITIONAL SYSTEM STATES
    * MUST BE PLACED BEFORE INVALID!
    * *********************************************************************/
   INVALID 
};

/* FSM Class */
class FSM{

    public:
    /* Public Function Prototypes */
    FSM(SYS_STATES_T initial_state);     /* Constructor */
    SYS_STATES_T getCurrentState();
    void setCurrentState(SYS_STATES_T new_state);
    

    private:
    SYS_STATES_T stateIndex;                       /* Current state index */
}; 

#endif /* _CHAIR_FSM_LIB_*/
