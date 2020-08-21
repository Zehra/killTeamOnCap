// killTeamOnCap.cpp
//

#include "bzfsAPI.h"

class killTeamOnCap : public bz_Plugin
{
public:
    virtual const char* Name (){return "KillTeamOnCap";}
    virtual void Init ( const char* config);
    virtual void Event ( bz_EventData *eventData );
    virtual void Cleanup ( void );
// Variables
int noLossOnDeath = 1;
// Utility functions
void killTeam(int noLoss, bz_eTeamType teamToKill) {
    if ((teamToKill !=eNoTeam) && (teamToKill !=eObservers))
    {
    bz_APIIntList *playerList = bz_newIntList();
    if (playerList) {
    
    bz_getPlayerIndexList(playerList);
    for ( unsigned int i = 0; i < playerList->size(); i++) {
    int targetID = (*playerList)[i];
    bz_BasePlayerRecord *playRec = bz_getPlayerByIndex ( targetID );

        if (playRec != NULL) {
            if ((playRec->spawned) && (playRec->team == teamToKill)) {
                bz_killPlayer(targetID, true, BZ_SERVER);
                if (noLoss)
                    bz_incrementPlayerLosses(targetID, -1);
            }
        }
        bz_freePlayerRecord(playRec);
    }
    bz_deleteIntList(playerList);
    }
    }
}
// 
};

BZ_PLUGIN(killTeamOnCap)

void killTeamOnCap::Init( const char* commandLine ) {
    if (strlen(commandLine) != 0)
        noLossOnDeath = 0;
    Register(bz_eCaptureEvent);
    //Register(bz_eTickEvent);
}

void killTeamOnCap::Cleanup (void) {
    Flush();
}

void killTeamOnCap::Event ( bz_EventData *eventData ) {
    if (eventData->eventType !=  bz_eCaptureEvent) 
        return;

    bz_CTFCaptureEventData_V1* captureData = (bz_CTFCaptureEventData_V1*)eventData;
        killTeam(noLossOnDeath, captureData->teamCapping);
}
  
