/*

*************************************************************************

ArmageTron -- Just another Tron Lightcycle Game in 3D.
Copyright (C) 2004  Armagetron Advanced Team (http://sourceforge.net/projects/armagetronad/) 

**************************************************************************

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
  
***************************************************************************

*/

#include "rSDL.h"

// declaration
#ifndef		ARMAGETRONAD_SRC_TRON_GCYCLEMOVEMENT_H_INCLUDED
#include	"gCycleMovement.h"
#endif

#include "tMath.h"

#include "nConfig.h"

#include "ePlayer.h"
#include "eDebugLine.h"
#include "eGrid.h"

#include "gWall.h"
#include "gSensor.h"

#include "tRecorder.h"

// #include <fstream>

#undef 	INLINE_DEF
#define INLINE_DEF

#ifndef DEDICATED
#define MAXRUBBER 1
#else
#define MAXRUBBER 3
#endif

#ifdef DEBUG
#define DEBUGOUTPUT
#endif

#ifdef DEBUGOUTPUT
#include "tSysTime.h"
static int sg_cycleDebugPrintLevel = 0;
#endif

//  *****************************************************************

static void sg_ArchiveCoord( eCoord & coord, int level )
{
    static char const * section = "_COORD";
    tRecorderSync< eCoord >::Archive( section, level, coord );
}

static void sg_ArchiveReal( REAL & real, int level )
{
    static char const * section = "_REAL";
    tRecorderSync< REAL >::Archive( section, level, real );
}

//  *****************************************************************

// version feature indicating that proper verlet integration should be used
static nVersionFeature sg_verletIntegration( 7 );

// strength of brake
static REAL sg_brakeCycle=30;
static nSettingItem<REAL> c_ab("CYCLE_BRAKE",
                               sg_brakeCycle);

REAL sg_cycleBrakeRefill  = 0.0;
REAL sg_cycleBrakeDeplete = 0.0;

// it should look this way, but a VersionFeature is used to control the application of these settings,
// so the nonwatched itmes suffice.
static nSettingItemWatched<REAL> sg_cycleBrakeRefillConf("CYCLE_BRAKE_REFILL",sg_cycleBrakeRefill, nConfItemVersionWatcher::Group_Annoying, 2 );
static nSettingItemWatched<REAL> sg_cycleBrakeDepleteConf("CYCLE_BRAKE_DEPLETE",sg_cycleBrakeDeplete, nConfItemVersionWatcher::Group_Annoying, 2 );

// static nSettingItem<REAL> sg_cycleBrakeRefillConf("CYCLE_BRAKE_REFILL",sg_cycleBrakeRefill );
// static nSettingItem<REAL> sg_cycleBrakeDepleteConf("CYCLE_BRAKE_DEPLETE",sg_cycleBrakeDeplete );

// cycle width: it won't fit into tunnels that are smaller than this
/*
REAL sg_cycleWidth = 0;
static tSettingItem<REAL> c_cw("CYCLE_WIDTH",
                               sg_cycleWidth);
*/

// base speed of cycle im m/s
static REAL sg_speedCycle=10;
static nSettingItem<REAL> c_s("CYCLE_SPEED",sg_speedCycle);

// minimal speed
static REAL sg_speedCycleMin=.25;
static nSettingItemWatched<REAL> c_sm("CYCLE_SPEED_MIN",
                                      sg_speedCycleMin,
                                      nConfItemVersionWatcher::Group_Bumpy,
                                      9);

REAL sg_speedCycleDecayBelow = 5;
static nSettingItemWatched<REAL> c_sdb("CYCLE_SPEED_DECAY_BELOW",
                                       sg_speedCycleDecayBelow,
                                       nConfItemVersionWatcher::Group_Bumpy,
                                       8);

REAL sg_speedCycleDecayAbove = .1;
static nSettingItemWatched<REAL> c_sda("CYCLE_SPEED_DECAY_ABOVE",
                                       sg_speedCycleDecayAbove,
                                       nConfItemVersionWatcher::Group_Bumpy,
                                       8);

// start speed of cycle im m/s
static REAL sg_speedCycleStart=20;
static tSettingItem<REAL> c_st("CYCLE_START_SPEED",
                               sg_speedCycleStart);

// min time between turns
REAL sg_delayCycle = .1;
static nSettingItem<REAL> c_d("CYCLE_DELAY",
                              sg_delayCycle);

// number of turns buffered exactly
int sg_cycleTurnMemory = 3;
static tSettingItem<int> c_tm("CYCLE_TURN_MEMORY",
                              sg_cycleTurnMemory);

REAL sg_delayCycleTimeBased = 1;
static nSettingItemWatched<REAL> c_dt("CYCLE_DELAY_TIMEBASED",
                                      sg_delayCycleTimeBased,
                                      nConfItemVersionWatcher::Group_Bumpy,
                                      7);

// extra factor to sg_delayCycle applied locally
#ifdef DEDICATED
REAL sg_delayCycleBonus=.95;
static tSettingItem<REAL> c_db("CYCLE_DELAY_BONUS",
                               sg_delayCycleBonus);
#else
REAL sg_delayCycleBonus=1;
#endif

REAL sg_cycleTurnSpeedFactor=.95;
static nSettingItemWatched<REAL> c_ctf("CYCLE_TURN_SPEED_FACTOR",
                                       sg_cycleTurnSpeedFactor,
                                       nConfItemVersionWatcher::Group_Bumpy,
                                       7);

// wall acceleration
static REAL sg_accelerationCycle=10;
static nSettingItem<REAL> c_a("CYCLE_ACCEL",
                              sg_accelerationCycle);

// acceleration multiplicators
REAL sg_accelerationCycleSelf = 1;
static nSettingItemWatched<REAL> c_aco("CYCLE_ACCEL_SELF",
                                       sg_accelerationCycleSelf,
                                       nConfItemVersionWatcher::Group_Bumpy,
                                       8);

REAL sg_accelerationCycleTeam = 1;
static nSettingItemWatched<REAL> c_act("CYCLE_ACCEL_TEAM",
                                       sg_accelerationCycleTeam,
                                       nConfItemVersionWatcher::Group_Bumpy,
                                       8);

REAL sg_accelerationCycleEnemy = 1;
static nSettingItemWatched<REAL> c_ace("CYCLE_ACCEL_ENEMY",
                                       sg_accelerationCycleEnemy,
                                       nConfItemVersionWatcher::Group_Bumpy,
                                       8);

REAL sg_accelerationCycleRim = 0;
static nSettingItemWatched<REAL> c_acr("CYCLE_ACCEL_RIM",
                                       sg_accelerationCycleRim,
                                       nConfItemVersionWatcher::Group_Bumpy,
                                       8);

REAL sg_accelerationCycleSlingshot = 1;
static nSettingItemWatched<REAL> c_acs("CYCLE_ACCEL_SLINGSHOT",
                                       sg_accelerationCycleSlingshot,
                                       nConfItemVersionWatcher::Group_Bumpy,
                                       8);

// acceleration offset
static REAL sg_accelerationCycleOffs=2;
static nSettingItem<REAL> c_ao("CYCLE_ACCEL_OFFSET",
                               sg_accelerationCycleOffs);


// when is a eWall near?
static REAL sg_nearCycle=6;
static nSettingItem<REAL> c_n("CYCLE_WALL_NEAR",
                              sg_nearCycle);

// tolerance for packet loss
static REAL sg_packetLossTolerance = 0;
static tSettingItem<REAL> conf_packetLossTolerance ("CYCLE_PACKETLOSS_TOLERANCE", sg_packetLossTolerance);

// tolerance for packet misses: if an intermediate packet is missing
static REAL sg_packetMissTolerance = 3;
static tSettingItem<REAL> conf_packetMissTolerance ("CYCLE_PACKETMISS_TOLERANCE", sg_packetMissTolerance);

// niceness when crashing a eWall
REAL sg_rubberCycle=MAXRUBBER;
static nSettingItem<REAL> c_r("CYCLE_RUBBER",
                              sg_rubberCycle);

REAL sg_rubberCycleTimeBased = 0;
static nSettingItemWatched<REAL> c_rtb("CYCLE_RUBBER_TIMEBASED",
                                       sg_rubberCycleTimeBased,
                                       nConfItemVersionWatcher::Group_Visual,
                                       7);

// allow legacy rubber code
bool sg_rubberCycleLegacy=true;
static nSettingItem<bool> c_rl("CYCLE_RUBBER_LEGACY",
                               sg_rubberCycleLegacy);

// niceness when crashing a eWall, influence of your ping
static REAL sg_rubberCyclePing=3;
static nSettingItem<REAL> c_rp("CYCLE_PING_RUBBER",
                               sg_rubberCyclePing);

// timescale rubber is restored on
REAL sg_rubberCycleTime=10;
static nSettingItemWatched<REAL> c_rt("CYCLE_RUBBER_TIME",
                                      sg_rubberCycleTime,
                                      nConfItemVersionWatcher::Group_Visual,
                                      7);

// max logarithmic approximation speed when rubber is in effect
static REAL sg_rubberCycleSpeed=40;
static nSettingItemWatched<REAL> c_rs("CYCLE_RUBBER_SPEED",
                                      sg_rubberCycleSpeed,
                                      nConfItemVersionWatcher::Group_Cheating,
                                      4);

#ifdef DEDICATED
#define MINDISTANCE_FACTOR 1
#else
#define MINDISTANCE_FACTOR 0
#endif

// minimal distance to a wall rubber will allow
static REAL sg_rubberCycleMinDistance=.001 * MINDISTANCE_FACTOR;
static nSettingItemWatched<REAL> c_rmd("CYCLE_RUBBER_MINDISTANCE",
                                       sg_rubberCycleMinDistance,
                                       nConfItemVersionWatcher::Group_Annoying,
                                       4);

// the length of the wall times this value is added to the previous value
static REAL sg_rubberCycleMinDistanceRatio=.0001 * MINDISTANCE_FACTOR;
static nSettingItemWatched<REAL> c_rmdr("CYCLE_RUBBER_MINDISTANCE_RATIO",
                                        sg_rubberCycleMinDistanceRatio,
                                        nConfItemVersionWatcher::Group_Annoying,
                                        4);

// on an empty rubber reservoir, this value is added as well
static REAL sg_rubberCycleMinDistanceReservoir=0.005 * MINDISTANCE_FACTOR;
static nSettingItemWatched<REAL> c_rmdres("CYCLE_RUBBER_MINDISTANCE_RESERVOIR",
        sg_rubberCycleMinDistanceReservoir,
        nConfItemVersionWatcher::Group_Annoying,
        7);

// and on a not so well prepared grind (if your last turn was not too long ago), this value.
static REAL sg_rubberCycleMinDistanceUnprepared=0.005 * MINDISTANCE_FACTOR;
static nSettingItemWatched<REAL> c_rmdup("CYCLE_RUBBER_MINDISTANCE_UNPREPARED",
        sg_rubberCycleMinDistanceUnprepared ,
        nConfItemVersionWatcher::Group_Annoying,
        7);

// this is the distance the preparation lengt is compared with
static REAL sg_rubberCycleMinDistancePreparation=0.2;
static nSettingItemWatched<REAL> c_rmdp("CYCLE_RUBBER_MINDISTANCE_PREPARATION",
                                        sg_rubberCycleMinDistancePreparation,
                                        nConfItemVersionWatcher::Group_Annoying,
                                        7);

// when connecting to older peers that may be rippable, multiply the minimum distance by this number if the
// wall in front of you is the rim
static REAL sg_rubberCycleMinDistanceLegacy=1;
static nSettingItem<REAL> c_rmdl("CYCLE_RUBBER_MINDISTANCE_LEGACY",
                                 sg_rubberCycleMinDistanceLegacy);

// this feature tracks whether the rip bug was fixed
static nVersionFeature sg_nonRippable(4);

// when adjusting to a wall, allow to get closer at least by this amount
static REAL sg_rubberCycleMinAdjust=.05;
static nSettingItemWatched<REAL> c_rma("CYCLE_RUBBER_MINADJUST",
                                       sg_rubberCycleMinAdjust,
                                       nConfItemVersionWatcher::Group_Annoying,
                                       4);

// during this fraction of the cycle delay time, rubber effectiveness will be reduced...
static REAL sg_rubberCycleDelay=0;
static nSettingItemWatched<REAL> c_rcd("CYCLE_RUBBER_DELAY",
                                       sg_rubberCycleDelay,
                                       nConfItemVersionWatcher::Group_Visual,
                                       6);

// by this factor ( meaning that rubber usage goes up by the inverse )
static REAL sg_rubberCycleDelayBonus=.5;
static nSettingItemWatched<REAL> c_rcdb("CYCLE_RUBBER_DELAY_BONUS",
                                        sg_rubberCycleDelayBonus,
                                        nConfItemVersionWatcher::Group_Visual,
                                        6);

// rubber usage gets increased by this amout after each turn...
static REAL sg_rubberCycleMalusTurn=0;
static nSettingItemWatched<REAL> c_rctm("CYCLE_RUBBER_MALUS_TURN",
                                        sg_rubberCycleMalusTurn,
                                        nConfItemVersionWatcher::Group_Visual,
                                        6);

// but the effect wears off after about this many seconds
static REAL sg_rubberCycleMalusTime=5;
static nSettingItem<REAL> c_rcmd("CYCLE_RUBBER_MALUS_TIME",
                                 sg_rubberCycleMalusTime);

// time tolerance when interpreting client commands
static REAL sg_timeTolerance=.1;
static nSettingItemWatched<REAL> c_tt( "CYCLE_TIME_TOLERANCE",
                                       sg_timeTolerance,
                                       nConfItemVersionWatcher::Group_Visual,
                                       6 );

static int sg_cycleMaxRefCount = 30000;
static tSettingItem<int> conf_sgCycleMaxRefCount ("CYCLE_MAX_REFCOUNT", sg_cycleMaxRefCount );

static inline bool clamp(REAL &c, REAL min, REAL max){
    tASSERT(min < max);

    if (!finite(c))
    {
        c = 0;
        return true;
    }

    if (c<min)
    {
        c = min;
        return true;
    }

    if (c>max)
    {
        c = max;
        return true;
    }

    return false;
}

// *******************************************************************************************
// *
// *	gCycleMovement
// *
// *******************************************************************************************
//!
//!
// *******************************************************************************************

// gCycleMovement::gCycleMovement()
// {
//	this->Init_gCycleCore();
// }

// *******************************************************************************************
// *
// *	gCycleMovement
// *
// *******************************************************************************************
//!
//!		@param	other	the source to copy from
//!
// *******************************************************************************************

// gCycleMovement::gCycleMovement( gCycleMovement const & other )
// {
// 	this->Init_gCycleCore();
//	this->CopyFrom( other );
//}

// *******************************************************************************************
// *
// *	operator =
// *
// *******************************************************************************************
//!
//!		@param	other	the source to copy from
//!		@return			a reference to this
//!
// *******************************************************************************************

gCycleMovement & gCycleMovement::operator= ( gCycleMovement const & other )
{
    this->CopyFrom( other );
    return *this;
}

// *******************************************************************************************
// *
// *	init_gCycleCore
// *
// *******************************************************************************************
//!
//!
// *******************************************************************************************

//void gCycleMovement::Init_gCycleCore()
//{
//    assert(0); // TODO: implement me
//}

// *******************************************************************************************
// *
// *	finit_gCycleCore
// *
// *******************************************************************************************
//!
//!
// *******************************************************************************************

//void gCycleMovement::Finit_gCycleCore()
//{
//    assert(0); // TODO: implement me
//}

// ----------------------------------------------------------------------------------------------------------

static float sg_speedMultiplier = 1.0f;
static nSettingItem<float> conf_mult ("REAL_CYCLE_SPEED_FACTOR", sg_speedMultiplier);

// *******************************************************************************************
// *
// *	SpeedMultiplier
// *
// *******************************************************************************************
//!
//!		@return 	the number all speed settings get multiplied by
//!
// *******************************************************************************************

float gCycleMovement::SpeedMultiplier( void )
{
    return sg_speedMultiplier;
}

// *******************************************************************************************
// *
// *	SetSpeedMultiplier
// *
// *******************************************************************************************
//!
//!		@param	mul	the number all speed settings get multiplied by
//!
// *******************************************************************************************

void gCycleMovement::SetSpeedMultiplier( REAL mul )
{
    conf_mult.Set( mul );
}

// for the given maximal acceleration, calculate the top speed
static REAL sg_MaxSpeed( REAL maxAcceleration )
{
    if ( sg_speedCycleDecayAbove > 0 )
        return sg_speedCycle + maxAcceleration / sg_speedCycleDecayAbove;
    else
        return sg_speedCycle * 100;
}

// *******************************************************************************************
// *
// *	MaximalSpeed
// *
// *******************************************************************************************
//!
//!		@return 	the maximal speed a cycle can reach
//!
// *******************************************************************************************

float gCycleMovement::MaximalSpeed( void )
{
    // determine the maximal acceleration by walls
    REAL maxWallAcceleration = 0;
    REAL wallAcceleration = sg_accelerationCycleTeam * sg_accelerationCycle;
    if ( wallAcceleration > maxWallAcceleration )
        maxWallAcceleration = wallAcceleration;
    wallAcceleration = sg_accelerationCycleEnemy * sg_accelerationCycle;
    if ( wallAcceleration > maxWallAcceleration )
        maxWallAcceleration = wallAcceleration;
    wallAcceleration = sg_accelerationCycleRim * sg_accelerationCycle;
    if ( wallAcceleration > maxWallAcceleration )
        maxWallAcceleration = wallAcceleration;
    // self acceleration is tricky: slingshot countermeasures have to be taken into account
    wallAcceleration = sg_accelerationCycleSelf * sg_accelerationCycle;
    wallAcceleration *= sg_accelerationCycleSlingshot;
    if ( wallAcceleration > maxWallAcceleration )
        maxWallAcceleration = wallAcceleration;

    // and there can always be two walls, and use wall accel formula
    maxWallAcceleration *= 2 * ( 1/sg_accelerationCycleOffs - 1/(sg_accelerationCycleOffs+sg_nearCycle ) );

    // maximal sustainable speed from that
    REAL maxSpeed = sg_MaxSpeed( maxWallAcceleration );

    // what if the brake is a booster?
    if ( sg_brakeCycle < 0 )
    {
        // what if it can be applied infinitely?
        REAL maxSpeedBoost = sg_MaxSpeed( maxWallAcceleration - sg_brakeCycle );

        // but the boost can permanently only be applied at this efficiency
        REAL efficiency = 1;
        if ( sg_cycleBrakeRefill + sg_cycleBrakeDeplete > 0 )
            efficiency = sg_cycleBrakeRefill/(sg_cycleBrakeRefill + sg_cycleBrakeDeplete);

        // maximal permanent speed
        REAL maxSpeedPermanent = sg_MaxSpeed( maxWallAcceleration - sg_brakeCycle*efficiency );

        // if the boost is limited, don't let the result be larger than what you can achieve by
        // accelerating from the max speed attainable from walls
        if ( sg_cycleBrakeDeplete > 0 )
        {
            REAL boostTime = 1/sg_cycleBrakeDeplete;
            REAL maxSpeedBoost2 = maxSpeedPermanent - boostTime * sg_brakeCycle;
            if ( maxSpeedBoost2 < maxSpeedBoost )
                maxSpeedBoost = maxSpeedBoost2;
        }

        // take over the result
        maxSpeed = maxSpeedBoost;
    }

    // start speed
    if ( sg_speedCycleStart > maxSpeed )
        maxSpeed = sg_speedCycleStart;

    // apply multiplier
    return sg_speedMultiplier * maxSpeed;
}

// ----------------------------------------------------------------------------------------------------------

// *******************************************************************************************
// *
// *	WindingNumber
// *
// *******************************************************************************************
//!
//!		@return		number of right turns taken minus the number of left turns
//!
// *******************************************************************************************

int gCycleMovement::WindingNumber( void ) const
{
    return windingNumber_;
}

// *******************************************************************************************
// *
// *	SetWindingNumberWrapped
// *
// *******************************************************************************************
//!
//!		@param newWindingNumberWrapped		new wrapped winding number taken from the current driving direction
//!
// *******************************************************************************************

void gCycleMovement::SetWindingNumberWrapped ( int newWindingNumberWrapped )
{
    // calculate the difference in the wrapped winding number
    int difference = newWindingNumberWrapped - windingNumberWrapped_;

    // wrap it into the interval [-WN/2,WN/2]
    if (2 * difference <= -Grid()->WindingNumber())
        difference += Grid()->WindingNumber();
    if (2 * difference >= Grid()->WindingNumber())
        difference -= Grid()->WindingNumber();

    // commit changes
    windingNumberWrapped_ = newWindingNumberWrapped;
    windingNumber_ += difference;
}

// *******************************************************************************************
// *
// *	Direction
// *
// *******************************************************************************************
//!
//!		@return	the currend driving direction
//!
// *******************************************************************************************

eCoord gCycleMovement::Direction( void ) const
{
    return dirDrive;
}

// *******************************************************************************************
// *
// *	LastDirection
// *
// *******************************************************************************************
//!
//!		@return	the last driving direction
//!
// *******************************************************************************************

eCoord gCycleMovement::LastDirection( void ) const
{
    return lastDirDrive;
}

// *******************************************************************************************
// *
// *	Speed
// *
// *******************************************************************************************
//!
//!		@return	the current speed
//!
// *******************************************************************************************

REAL gCycleMovement::Speed( void ) const
{
    REAL ret = verletSpeed_ + .5f * lastTimestep_ * acceleration;
    return ret > 0 ? ret : 0;
}

// *******************************************************************************************
// *
// *	Alive
// *
// *******************************************************************************************
//!
//!		@return		true if the cycle is still alive
//!
// *******************************************************************************************

bool gCycleMovement::Alive() const
{
    return alive_ > 0;
}

// *******************************************************************************************
// *
// *	Vulnerable
// *
// *******************************************************************************************
//!
//!		@return		true if the cycle is vulnerable
//!
// *******************************************************************************************

bool gCycleMovement::Vulnerable() const
{
    return true;
}

// *******************************************************************************************
// *
// *	CanMakeTurn
// *
// *******************************************************************************************
//!
//!		@return	true if a new turn is possible right now
//!
// *******************************************************************************************

bool gCycleMovement::CanMakeTurn( void ) const
{
    return pendingTurns.empty() && CanMakeTurn( lastTime );
}

// *******************************************************************************************
// *
// *	CanMakeTurn
// *
// *******************************************************************************************
//!
//!     @param  time the time to check
//!		@return	true if a new turn is possible at the given time
//!
// *******************************************************************************************

bool gCycleMovement::CanMakeTurn( REAL time ) const
{
    return time >= GetTurnDelay() + lastTurnTime_;
}

// *******************************************************************************************
// *
// *	GetTurnDelay
// *
// *******************************************************************************************
//!
//!		@return	the delay between turns in seconds
//!
// *******************************************************************************************

REAL gCycleMovement::GetTurnDelay( void ) const
{
    // the basic delay as it was before 0.2.8 looked like this:
    REAL baseDelay   = sg_delayCycle*sg_delayCycleBonus/SpeedMultiplier();

    // we're modifying it by a power law to make speed turns easier or harder:
    REAL speedFactor = verletSpeed_/(sg_speedCycle*SpeedMultiplier());

    return baseDelay * pow( speedFactor, sg_delayCycleTimeBased-1 );
}

// *******************************************************************************************
// *
// *	GetNextTurn
// *
// *******************************************************************************************
//!
//!		@return	the time of the next possible turn
//!
// *******************************************************************************************

REAL gCycleMovement::GetNextTurn( void ) const
{
    return lastTurnTime_ + GetTurnDelay();
}

// *******************************************************************************************
// *
// *	AddDestination
// *
// *******************************************************************************************
//!
//!
// *******************************************************************************************

void gCycleMovement::AddDestination( void )
{
    if ( sn_GetNetState() == nCLIENT )
    {
        gDestination* dest = tNEW(gDestination)(*this);
        //	dest->position = dest->position + dest->direction.Turn( 0, 10.0f );
        AddDestination( dest );
    }
}

// *******************************************************************************************
// *
// *	AddDestination
// *
// *******************************************************************************************
//!
//!		@param	dest		the destination to add
//!
// *******************************************************************************************

void gCycleMovement::AddDestination( gDestination * dest )
{
    //  con << "got new dest " << dest->position << "," << dest->direction
    // << "," << dest->distance << "\n";

    dest->InsertIntoList(&destinationList);

    // if the next destination already has been used, this destination will never be used
    if (dest->next && dest->next->hasBeenUsed){
        delete dest;
        return;
    }

    this->NotifyNewDestination( dest );

    // repeat insertion: position may have changed
    dest->InsertIntoList(&destinationList);
}

// *******************************************************************************************
// *
// *	GetCurrentDestination
// *
// *******************************************************************************************
//!
//!		@return		the destination this cycle is driving towards right now
//!
// *******************************************************************************************

gDestination * gCycleMovement::GetCurrentDestination( void ) const
{
    return currentDestination;
}

// *******************************************************************************************
// *
// *	AdvanceDestination
// *
// *******************************************************************************************
//!
//!
// *******************************************************************************************

void gCycleMovement::AdvanceDestination( void )
{
    // not implemented
    tASSERT(0);
}

// *******************************************************************************************
// *
// *	NotifyNewDestination
// *
// *******************************************************************************************
//!
//!		@param	dest	   the new destination the cycle is notified about
//!
// *******************************************************************************************

void gCycleMovement::NotifyNewDestination( gDestination * dest )
{
    this->OnNotifyNewDestination( dest );
}

// *******************************************************************************************
// *
// *	DoIsDestinationUsed
// *
// *******************************************************************************************
//!
//!		@param	dest	the destination to test
//!		@return			true if the destination is still in active use
//!
// *******************************************************************************************

bool gCycleMovement::DoIsDestinationUsed( const gDestination * dest ) const
{
    return ( destinationList == currentDestination || destinationList == lastDestination );
}

// *******************************************************************************************
// *
// *	DistanceToDestination
// *
// *******************************************************************************************
//!
//!		@param	dest	the destination to measure the distance to
//!		@return			the distance to the destination
//!
// *******************************************************************************************

REAL gCycleMovement::DistanceToDestination( gDestination & dest ) const
{
    // read future direction from destination
    eCoord dirTurned = dest.direction;

    REAL F = eCoord::F( dirTurned, dirDrive );
    if ( F > .99 )
    {
        // destination direction and driving direction coincide; we have to
        // make up a new turned direction

        // no need to worry if brake status changed
        if ( ( braking != 0 ) != dest.braking )
        {
            return eCoord::F( dest.position - pos, dirDrive );
        }

        // we'd have to turn in this direction to reach the destination
        int side = (dest.position - pos) * dirDrive > 0 ? -1 : 1;

        // pretend to turn in that direction and fetch driving vector
        int w = windingNumberWrapped_;
        Grid()->Turn(w, side);
        dirTurned = Grid()->GetDirection( w );
    }
    else if ( F < -.99 )
    {
        // destination direction and driving direction are opposed. This must be a grave error,
        // so we'll make something up
        return eCoord::F( dest.position - pos, dirDrive );
    }

    // calculate when a turn would need to be made that aligns
    // this cycle with the destination
    return ( ( dest.position - pos ) * dirTurned ) / ( dirDrive * dirTurned );
}

// *******************************************************************************************
// *
// *	OnNotifyNewDestination
// *
// *******************************************************************************************
//!
//!		@param	dest	   the new destination
//!
// *******************************************************************************************

void gCycleMovement::OnNotifyNewDestination( gDestination * dest )
{
    // go back one destination if the new destination appears to be older than the current one
    if ((!currentDestination || currentDestination == dest->next ) &&
            sn_GetNetState()!=nSTANDALONE && ( Owner() != ::sn_myNetID || !destinationList ) )
    {
        currentDestination=dest;
        // con << "setting new cd\n";
    }
}

// *******************************************************************************************
// *
// *	OnDropTempWall
// *
// *******************************************************************************************
//!
//!		@param	wall	   the wall the other cycle is grinding
//!
// *******************************************************************************************

void gCycleMovement::OnDropTempWall( gPlayerWall * wall )
{
}

// *******************************************************************************************
// *
// *   GetDestinationBefore
// *
// *******************************************************************************************
//!
//!        @param  sync     the sync data received from the server
//!        @param  first    the first candidate for the return value
//!        @return          the destination entry that was last processed on the server
//!
// *******************************************************************************************

gDestination * gCycleMovement::GetDestinationBefore( const SyncData & sync, gDestination * first )
{
    // message IDs smaller than 16 don't exist
    if ( sync.messageID != 1 )
    {
        gDestination * ret = first;

        // deterimine last passed destination by the message ID
        while ( ret && ret->messageID != sync.messageID )
            ret = ret->next;

        // return match
        return ret;
    }
    else
    {
        // calculate the distance of the last turn of the sync
        REAL syncLastTurnDistance = sync.distance - ( sync.pos - sync.lastTurn ).Norm();

        // message ID not available; must use heuristics
        gDestination * run = first;         // destination iterator
        gDestination * bestMatch = NULL;    // the best message that fit the sync data and that lies before the sync
        REAL bestMatchDistance = 1E+20;     // the distance of the best message to the sync
        bool braking = false;               // braking causes trouble here. Activate extra checks if brakes are involved
        while ( run )
        {
            // calculate discrepancy of last turn distance of sync to the current message's distance
            REAL distanceBefore = syncLastTurnDistance - run->distance;
            REAL distanceAfter  = run->distance - sync.distance;


            // the allowed values for run->distance are inside the interval [ syncLastTurnDistance,sync.distance ]
            // distance is a metric that is positive outside of the interval and
            // negative inside it, with minimum in the center.
            REAL distance = distanceBefore < distanceAfter ? distanceBefore : distanceAfter;

            // activate brake trouble compensation
            if ( distance < 0 && ( run->braking || sync.braking ) )
            {
                // void previous match
                if ( !braking )
                    bestMatchDistance += 1;
                braking = true;
            }

            // clamp distance to nonnegative values to give points inside the allowed interval
            // equal chances
            if ( distance < 0 )
                distance = 0;

            // prefer destinations close to the end of the allowed interval if braking is involved, else destinations close to the beginning
            if ( braking )
            {
                distance += fabs( distanceAfter + .01 * distanceBefore ) * .0001;
            }
            else
            {
                distance += fabs( distanceBefore ) * .1;
            }

            // see if brake status and driving direction match; this is a must
            if ( eCoord::F( run->direction, sync.dir ) > .9 && run->braking == ( sync.braking != 0 ) )
            {
                if ( !bestMatch || distance < bestMatchDistance )
                {
                    bestMatch = run;
                    bestMatchDistance = distance;
                }
            }
            run = run->next;
        }

        // con << bestMatchDistance << "\n";

        // return match
        return bestMatch;
    }
}

// *******************************************************************************************
// *
// *	EdgeIsDangerous
// *
// *******************************************************************************************
//!
//!		@param	wall	the wall to check for danger
//!		@param	time	the time of the possible collision
//!		@param	alpha	the local wall coordinate of the collision
//!		@return			true if the wall is dangerous
//!
// *******************************************************************************************

bool gCycleMovement::EdgeIsDangerous( const eWall * wall, REAL time, REAL alpha ) const
{
    if (!wall)
        return false;

    const gPlayerWall *w = dynamic_cast<const gPlayerWall*>(wall);
    if (w)
    {
        // have we entered a hole? Is the wall breaking down? Have we passed behind the end?
        if ( !w->IsDangerous( alpha, time ) )
            return false;

        // get time the wall was built
        REAL builtTime = w->Time(alpha);

        gCycleMovement *otherPlayer=w->CycleMovement();
        if (otherPlayer==this && time < builtTime+2.5*GetTurnDelay() )
            return false;        // impossible to make such a small circle
    }

    return true; // it is a real eWall.
}

// *******************************************************************************************
// *
// *	Turn
// *
// *******************************************************************************************
//!
//!		@param	dir	negative for left turns, positive for right turns
//!		@return		true if the turning was successful
//!
// *******************************************************************************************

bool gCycleMovement::Turn( REAL dir )
{
    if (dir>0)
        return Turn(1);
    else if (dir<0)
        return Turn(-1);
    else
        return false;
}

// *******************************************************************************************
// *
// *	Turn
// *
// *******************************************************************************************
//!
//!		@param	dir	+1 for right turns, -1 for left turns
//!		@return		true if the turning was successful
//!
// *******************************************************************************************

bool gCycleMovement::Turn( int dir )
{
    return DoTurn( dir );
}

static void DropTempWall( eCoord const & dir, gSensor const & sensor )
{
    tASSERT( sensor.ehit );

    if (sn_GetNetState() != nCLIENT )
    {
        // if the wall is temporary, let its cycle drop it so it gets copied into the grid
        // and makes less phasing problems
        // don't drop parallel walls
        eCoord vec = sensor.ehit->Vec();
        if ( fabs( dir * vec ) < vec.Norm() * .5 )
            return;

        // get the wall
        eWall * ew = sensor.ehit->GetWall();
        tASSERT( ew );
        gPlayerWall* w = dynamic_cast< gPlayerWall * >( ew );

        // check if wall exists
        if ( w )
        {
            // get the cycle
            gCycleMovement* other = w->CycleMovement();

            // let it drop wall
            if ( other )
                other->DropTempWall( w );
        }
    }
}

// *******************************************************************************************
// *
// *	MaxSpaceAhead
// *
// *******************************************************************************************
//!     determines how much a given cycle is allowed to drive ahead without getting too close to the next wall. Looks exactly lookAhead into the future.
//!
//!		@param	cycle the cycle to investigate
//!     @param  lookAhead minimum distance to look ahead
//!		@return		distance from the cycle to the next wall
//!
// *******************************************************************************************

float MaxSpaceAhead( const gCycleMovement* cycle, float lookAhead )
{
    sg_ArchiveReal( lookAhead, 9 );

    // calculate the relevant minimal distance
    REAL mindistance = sg_rubberCycleMinDistance;
    {
        // add the reservoir dependant term
        REAL rubber_granted=sg_rubberCycle;
        if ( rubber_granted > 0 )
        {
            REAL filling = cycle->GetRubber()/rubber_granted;
            if ( filling > 1 )
                filling = 1;
            mindistance += sg_rubberCycleMinDistanceReservoir * (1-filling);
        }

        // add the bad preparation dependant term
        if ( sg_rubberCycleMinDistancePreparation > 0 )
        {
            REAL badPreparation = sg_rubberCycleMinDistancePreparation/( sg_rubberCycleMinDistancePreparation + ( cycle->LastTime() - cycle->GetLastTurnTime() ) );
            mindistance += sg_rubberCycleMinDistanceUnprepared * badPreparation;
        }
    }
    sg_ArchiveReal( mindistance, 9 );

    // since we are going to subtract the rubber min distance from the found hit, we'll still have to llok a bit further:
    lookAhead += mindistance * sg_rubberCycleMinDistanceLegacy * 2;
    // be a little nice and don't drive into the eWall if turning is allowed
    gSensor fr( const_cast< gCycleMovement* >( cycle ), cycle->Position(), cycle->Direction() );
    fr.detect( lookAhead );

    if ( fr.ehit )
    {
#ifdef DEBUG
        {
            gSensor fr2( const_cast< gCycleMovement* >( cycle ), cycle->Position(), cycle->Direction() );
            fr2.detect( lookAhead );
        }
#endif

        REAL stopDistance = 0.1;
        if ( fr.ehit )
        {
            REAL norm = fr.ehit->Vec().Norm();
            stopDistance = mindistance + sg_rubberCycleMinDistanceRatio * norm;

            DropTempWall( cycle->Direction(), fr );
        }
        sg_ArchiveReal( stopDistance, 9 );

        // revert to almost old rubber logic if old clients are connected. This may cause rips, but we don't care.
        if ( sg_rubberCycleLegacy && !sg_nonRippable.Supported() && stopDistance > .001 )
            stopDistance = .001;

        // if there is a rippable peer connected and the wall is the rim wall, add extra distance
        //if ( fr.type == gSENSOR_RIM && !sg_nonRippable.Supported() )
        //    stopDistance *= sg_rubberCycleMinDistanceLegacy;

        REAL space = fr.hit;
        sg_ArchiveReal( space, 9 );

        // see if we just did a turn
        REAL distSinceLastTurn = cycle->GetDistanceSinceLastTurn();

        // we want to get closer to the wall by at least some percentage
        REAL maxStop = ( distSinceLastTurn + space ) * ( 1 - sg_rubberCycleMinAdjust );
        if ( maxStop < stopDistance )
        {
            stopDistance = maxStop;
        }

        sg_ArchiveReal( stopDistance, 9 );

        space -= stopDistance;

        // add safety
        REAL safety = cycle->Position().Norm() * 2 * EPS;
        space -= safety;

        sg_ArchiveReal( space, 9 );

        return space;
    }
    else
    {
        if (sn_GetNetState() != nCLIENT )
        {
            // send out scanners to the left and right (only for wall gridding)
            for (int dir = 1; dir >= -1; dir -= 2)
            {
                gSensor side( const_cast< gCycleMovement* >( cycle ), cycle->Position(), cycle->Direction().Turn(1,.05 * dir) );
                side.detect( lookAhead);
                if ( side.ehit )
                {
                    DropTempWall( cycle->Direction(), side );
                }
            }
        }
    }

    REAL space = 1E+30;
    sg_ArchiveReal( space, 9 );
    return space;
}

// *******************************************************************************************
// *
// *	MaxSpaceAhead
// *
// *******************************************************************************************
//!     determines how much a given cycle is allowed to drive ahead without getting too close to the next wall. Looks at least lookAhead into the future, but never reports more than maxReport as result. The next timestep is assumed to be ts seconds long.

//!		@param	cycle the cycle to investigate
//!     @param  ts the next timestep
//!     @param  lookAhead minimum distance to look ahead
//!     @param  maxReport maximum return value
//!		@return		distance from the cycle to the next wall
//!
// *******************************************************************************************

float MaxSpaceAhead( const gCycleMovement* cycle, float ts, float lookAhead, float maxReport )
{
    // lookahead should be at least the next expected timestep ( times two for safety )
    REAL step=cycle->Speed() * ts * 2;
    if ( lookAhead < step )
        lookAhead = step;

    // lookahead should be at least maxReport
    if ( lookAhead < maxReport )
        lookAhead = maxReport;

    REAL space = MaxSpaceAhead( cycle, lookAhead );

    if ( space < maxReport )
        return space;
    else
        return maxReport;
}

// feature indicating that a client sends the time of turn commands
static nVersionFeature sg_CommandTime( 4 );

// server side feature of lag sliding correction code
static nVersionFeature sg_AntiLag( 5 );

// flag indicating whether to use the logic to prevent lag sliding only when every connected client can benefit from it
static bool sg_fairAntiLagSliding=true;
static nSettingItem<bool> c_fals("CYCLE_FAIR_ANTILAG",sg_fairAntiLagSliding);

// check if we should apply anti-lag-sliding code
static bool sg_UseAntiLagSliding( const eNetGameObject* obj )
{
    tASSERT( obj );

    // cant do anyting for old clients that don't send the time of commands
    if ( !sg_CommandTime.Supported( obj->Owner() ) )
        return false;

    // likewise if the server does not support anti lag code and this is the client
    if ( sn_GetNetState() == nCLIENT && !sg_AntiLag.Supported( obj->Owner() ) )
        return false;

    // check whether the command time is sent by everyone or at least the object owner ( depending on fairness )
    if ( sg_fairAntiLagSliding )
    {
        return sg_CommandTime.Supported();
    }
    else
    {
        // we already checked whether the command time was sent
        return true;
    }
}

// *******************************************************************************************
// *
// *	Timestep
// *
// *******************************************************************************************
//!
//!		@param	currentTime	the time to simulate up to
//!		@return	true if the cycle is to be deleted
//!
// *******************************************************************************************

bool gCycleMovement::Timestep( REAL currentTime )
{
    // clamp stuff to finite values
    clamp( rubber, 0, sg_rubberCycle );

    // keep this cycle alive
    tJUST_CONTROLLED_PTR< gCycleMovement > keep( this->GetRefcount()>0 ? this : 0 );

    // remove old destinations
    //REAL lag = 1;
    //if ( player )
    //    lag = player->ping;
    // REAL lagDistance = lag * Speed() * 10;

    while(destinationList && destinationList->hasBeenUsed && !IsDestinationUsed( destinationList ) )
        delete destinationList;

    // calculate timestep
    REAL dt = currentTime - lastTime;

    sg_ArchiveReal( dt, 9 );

    // calculate acceleration
    this->CalculateAcceleration( dt );

    // if (currentTime > lastTime)
    {
        int timeout=10;
        bool forceTurn = false; // force turning at the next iteration

        // only simulate forward in time
        while (pendingTurns.empty() && currentDestination && timeout > 0 && currentTime > lastTime )
        {
            timeout --;

            // the distance to the next destination
            REAL dist_to_dest = DistanceToDestination( *currentDestination );
            sg_ArchiveReal( dist_to_dest, 9 );

            REAL ts=currentTime - lastTime;
            sg_ArchiveReal( ts, 9 );

            sg_ArchiveReal( verletSpeed_, 9 );
            sg_ArchiveReal( acceleration, 9 );

            // our speed
            REAL avgspeed=verletSpeed_;
            if (acceleration > 0)
                avgspeed += acceleration * SpeedMultiplier() * ts * .5;

            // will rubber slow the cycle down to a crawl, so that the command time will be
            // a better indication when to turn than the position?
            bool rubberActive = false;

            // check if the path to the destination is clear for the next timesteps
            sg_ArchiveReal( rubberSpeedFactor, 9 );
            if ( rubberSpeedFactor < .999 )
            {
                // take rubber activity into account
                rubberActive = true;
                avgspeed *= rubberSpeedFactor;
                if ( avgspeed < EPS )
                    avgspeed = EPS;

                sg_ArchiveReal( avgspeed, 9 );

                // don't drive into a wall, turn before getting too close
                REAL lookahead = ts * avgspeed * 2;
                REAL distToWall = MaxSpaceAhead( this, ts, lookahead, lookahead );
                if ( dist_to_dest > distToWall )
                    dist_to_dest = distToWall;
            }

            static bool breakp = false;

            // the time left until the turn happened on the client
            // REAL timeLeft = currentDestination->GetGameTime() - lastTime;

            // the earliest and latest allowed time to turn
            REAL turnTime = currentDestination->GetGameTime();
            REAL earliestTurnTime = turnTime - sg_timeTolerance - 100;
            REAL latestTurnTime   = turnTime + sg_timeTolerance;

            // if rubber is active and anti lag sliding code should be enabled,
            // then allow no early or late turns
            if ( sg_UseAntiLagSliding( this ) )
            {
                earliestTurnTime = turnTime - sg_timeTolerance;
                if ( rubberActive )
                {
                    earliestTurnTime = latestTurnTime = turnTime;

                }
            }

            sg_ArchiveReal( dist_to_dest, 9 );

            if ( dist_to_dest > ts * avgspeed && currentTime < latestTurnTime )
                break; // no need to worry; we won't reach the next destination

            if ( currentTime < earliestTurnTime && sg_CommandTime.Supported( Owner() ) )
                break; // the turn is too far in the future

            if ( lastTime >= earliestTurnTime && ( forceTurn || dist_to_dest < 0.01 || timeout <= 0 || lastTime >= latestTurnTime ) ){
                forceTurn = false;
                // con << timeLeft << ", " << earlyTurnTolerance << ", " << rubberActive << ", " << dist_to_dest << "\n";

                // the destination is very close or we gave up. Now is the time to turn towards
                // it or turn to the direction it gives us

                // bring us to the exact location to avoid lag sliding due to
                // disagreement between client and server.
                // but only if we are reasonably close ( don't want cheating )
                // and use a correct move that kills us if we cross a wall.

                /*
                         if ( dist_to_dest < .1 && dist_to_dest > -.1 )
                         {
                             Move( pos + dirDrive * dist_to_dest, currentTime, currentTime );
                         }
                #ifdef DEBUG
                         else
                         {
                             if ( breakp )
                             {
                                 int x;
                                 x = 0;
                             }
                             con << "gCycle::Timestep: Could not completely reach destination.\n";
                             breakp = true;
                         }
                #endif			
                */

                // con << "Turn: " << lastTime << ", " << dist_to_dest << ", " << currentDestination->position << ", " << pos << "\n";

                //eDebugLine::SetTimeout(2);
                //eDebugLine::SetColor( 0,1,1 );
                //eDebugLine::Draw( currentDestination->position, 0, currentDestination->position, 8 );
                //eDebugLine::SetColor( 0,0,1 );
                //eDebugLine::Draw( pos, 0, pos, 8 );

                REAL t = currentDestination->direction * dirDrive;

                // if we can't turn now, simulate a bit further
                if ( fabs(t) > .01 && !CanMakeTurn() )
                {
                    REAL nextTurn = GetNextTurn() + .005;
                    if ( nextTurn < currentTime )
                    {
                        TimestepCore( nextTurn );
                        lastTurnTime_ = -100;
                    }
                    else
                    {
                        // not enough time to simulate to turn possibility; skip out of loop
                        break;
                    }
                }

                bool used = false; // flag indicating whether the current destination has been used
                bool missed=(fabs(t)<.01);
                if (int(braking) != int(currentDestination->braking))
                    missed=!missed;

                // detect false misses: if the last destination's message ID is just
                // one below the current destination's message ID, it's a fake
                if ( missed && lastDestination && lastDestination->messageID == currentDestination->messageID-1 )
                    missed = false;

                if (!missed){ // then we did not miss a destination
                    used = true;

                    if (fabs(t)>.01)
                        Turn(-t);
                    else{
                        braking = currentDestination->braking;
                        if (sn_GetNetState()!=nCLIENT)
                            RequestSync();
                    }

                    /*
                      con << "turning alon " << currentDestination->position << "," 
                      << currentDestination->direction << "," 
                      << currentDestination->distance << "\n";
                    */
                }
                else
                {
                    // Uh oh. One command is missing. We should wait as long as possible, perhaps
                    // it already is on its way.
                    if ( lastTime > currentTime - Lag()*sg_packetMissTolerance )
                        return !Alive();

                    // OK, we missed a turn. Don't panic. Just turn
                    // towards the destination:
                    REAL side = (currentDestination->position - pos) * dirDrive;
                    if (fabs(side)>1)
                    {
                        Turn(-side);
                    }
                    else
                        used = true;

                    /*
                      con << "turning to   " << currentDestination->position << "," 
                      << currentDestination->direction << "," 
                      << currentDestination->distance << "\n";
                    */

                }

                if ( used )
                {
                    // only the server marks destinations as used; the client has to reuse them sometimes.
                    currentDestination->hasBeenUsed = (sn_GetNetState()!=nCLIENT);
                    lastDestination = currentDestination;

                    // advance
                    currentDestination = currentDestination->next;
                }


                while (currentDestination && currentDestination->hasBeenUsed)
                {
                    breakp = false;
                    currentDestination = currentDestination->next;
                }
            }
            else
            {
                // ok, the dest is right ahead, but not close enough.
                // how long does it take at least
                // (therefore the strange average speed) to get there?
                sg_ArchiveReal( avgspeed, 9 );
                REAL tsTodo = dist_to_dest/avgspeed;
                /*
                         if ( tsTodo > timeLeft )
                         {
                             tsTodo = timeLeft;
                         }
                */
                sg_ArchiveReal( tsTodo, 9 );
                // try to turn at the exactly right moment
                REAL timeLeft = turnTime - lastTime;
                sg_ArchiveReal( timeLeft, 9 );
                if ( timeLeft >=0 && tsTodo > timeLeft )
                {
                    // force turn on next iteration, we'll be there
                    forceTurn = true;
                    tsTodo = timeLeft;
                }

                // but simulate at least until the moment we CAN make a turn
                REAL mints = earliestTurnTime - lastTime;
                // sg_ArchiveReal( mints, 9 );
                if ( tsTodo < mints )
                {
                    tsTodo = mints;
                }


                if ( tsTodo < 0 )
                {
                    // should never happen
                    st_Breakpoint();
                    return !Alive();
                }
                if ( tsTodo > ts )
                {
                    tsTodo = ts;
                }
	#ifdef DEBUG
                if ( tsTodo < 0 )
                    con << "Negative timestep!\n";
	#endif
                sg_ArchiveReal( tsTodo, 9 );

                // core simulation
                if ( tsTodo > EPS )
                {
                    TimestepCore( lastTime + tsTodo );
                }
                else
                {
                    // already nothing to do, turn on next iteration
                    forceTurn = true;
                }
            }
        }
    }

    // simulate exactly to the time of the next turn if it is in reach
    REAL nextTurn = GetNextTurn();
    if ( !pendingTurns.empty() && currentTime > nextTurn )
    {
        if ( nextTurn > lastTime )
            TimestepCore( nextTurn );
        lastTurnTime_ = -100;

        //con << "Executing delayed turn at time " << lastTime << "\n";
        Turn(pendingTurns.front());
        pendingTurns.pop_front();
    }

    // do the rest of the timestep
    bool ret = TimestepCore( currentTime );

    return ret;
}

// *******************************************************************************************
// *
// *	AddRef
// *
// *******************************************************************************************
//!
//!
// *******************************************************************************************

void gCycleMovement::AddRef( void )
{
    eNetGameObject::AddRef();
    if ( GetRefcount() > sg_cycleMaxRefCount && Alive() )
    {
        // during the kill, further refcounts will be added, so we need to pump
        // up the limit
        int backup = sg_cycleMaxRefCount;
        sg_cycleMaxRefCount += 100;
        Kill();
        sg_cycleMaxRefCount = backup;
    }
}

// *******************************************************************************************
// *
// *	gCycleMovement
// *
// *******************************************************************************************
//!
//!		@param	grid			the grid the cycle will live on
//!		@param	pos				start position
//!		@param	dir				start direction
//!		@param	player			player owning this cycle
//!		@param	autodelete		should the cycle get deleted when it gets killed?
//!
// *******************************************************************************************

gCycleMovement::gCycleMovement( eGrid * grid, const eCoord & pos, const eCoord & dir, ePlayerNetID * player, bool autodelete )
        :eNetGameObject(grid, pos,dir,player,autodelete),
        destinationList(NULL),currentDestination(NULL),lastDestination(NULL),
        dirDrive(dir),
        acceleration(0),
        lastTimestep_(0),
        verletSpeed_(sg_speedCycleStart * SpeedMultiplier()),
        pendingTurns()
{
    windingNumberWrapped_ = windingNumber_ = Grid()->DirectionWinding(dir);

    MyInitAfterCreation();
}

// *******************************************************************************************
// *
// *	gCycleMovement
// *
// *******************************************************************************************
//!
//!		@param	message		network message to read everything from
//!
// *******************************************************************************************

gCycleMovement::gCycleMovement( nMessage & message )
        :eNetGameObject(message),
        destinationList(NULL),currentDestination(NULL),lastDestination(NULL),
        dirDrive(1,0),
        acceleration(0),
        lastTimestep_(0),
        verletSpeed_(5)
{
    windingNumberWrapped_ = windingNumber_ = 2;

    MyInitAfterCreation();
}

// *******************************************************************************************
// *
// *	~gCycleMovement
// *
// *******************************************************************************************
//!
//!
// *******************************************************************************************

gCycleMovement::~gCycleMovement( void )
{
    lastDestination = NULL;
    currentDestination = NULL;

    while(destinationList)
    {
        gDestination* dest = destinationList;
        delete dest;
    }

    verletSpeed_=distance=0;
}

// *******************************************************************************************
// *
// *	CopyFrom
// *
// *******************************************************************************************
//!
//!		@param	other	the cycle to copy everything from
//!
// *******************************************************************************************

void gCycleMovement::CopyFrom( const gCycleMovement & other )
{
    // calculate position update
    eCoord posUpdate = other.Position() - this->Position();

#ifdef DEBUG_X
    // only update direction if the positions are out of sync
    REAL lag = 1;
    if ( player )
        lag = player->ping;

    REAL tol = this->speed * lag;
    if ( posUpdate.NormSquared() > tol*tol )// && eCoord::F( dirDrive, other.dirDrive ) < .5 )
    {
        con << "Out of sync!\n";
        //		dir 			= other.Direction();

        // get second opinion
        tJUST_CONTROLLED_PTR<gCycleExtrapolator> extrapolator = tNEW( gCycleExtrapolator )(grid, pos, dir );
        gCycleExtrapolator& secondOpinion = *extrapolator;
        secondOpinion.CopyFrom( sg_usedMessage, *this );
        eGameObject::TimestepThis( other.lastTime, &secondOpinion );
    }
#endif

    dirDrive 		= other.dirDrive;

    // transfer position and time
    currentFace = other.currentFace;
    pos = other.Position();
    lastTime = other.LastTime();
    // Move( other.Position(), LastTime(), other.LastTime() );
    // Move( other.Position() + other.Direction() * ( ( lastTime - other.LastTime() ) * other.Speed() ), LastTime(), other.LastTime() );

    // std::cout << "copy: " << brakingReservoir << ":" << braking << "\n";

    // transfer additional data
    distance        = other.distance;
    lastTimestep_   = other.lastTimestep_;
    verletSpeed_    = other.verletSpeed_;
    acceleration    = other.acceleration;
    rubber	        = other.rubber;
    rubberMalus     = other.rubberMalus;
    brakingReservoir= other.brakingReservoir;
    windingNumber_          = other.windingNumber_;
    windingNumberWrapped_   = other.windingNumberWrapped_;

    // std::cout << "copy: " << brakingReservoir << ":" << braking << "\n";

#ifdef DEBUG_X
    if ( turns != other.turns )
    {
        con << "Client/Server turn mismatch:" << turns << " != " << other.turns << "\n";
    }
#endif

    // update number of turns if the player is not turning wildly
    if ( lastTime > GetNextTurn() + 2 * GetTurnDelay() )
        turns			= other.turns;
}

static nVersionFeature sg_sendCorrectLastTurn(8);

// *******************************************************************************************
// *
// *	CopyFrom
// *
// *******************************************************************************************
//!
//!		@param	sync	   	the network sync data to copy the most important data from
//!		@param	other		the cycle to copy the rest of the information from
//!
// *******************************************************************************************

void gCycleMovement::CopyFrom( const SyncData & sync, const gCycleMovement & other )
{
    // fetch values from sync
    dir = dirDrive 	= sync.dir;
    lastTimestep_   = 0;
    verletSpeed_ 	= sync.speed;
    rubber			= sync.rubber;
    rubberMalus     = sync.rubberMalus;
    braking			= sync.braking;
    distance		= sync.distance;
    turns			= sync.turns;
    brakingReservoir= sync.brakingReservoir;
    // std::cout << "fromsync: " << brakingReservoir << ":" << braking << "\n";

    // reset winding number and acceleration
    this->SetWindingNumberWrapped( Grid()->DirectionWinding(dirDrive) );
    acceleration	= 0;

    // fetch values from other
    // rubber = other.rubber;
    SetPlayer( other.Player() );
    currentFace         = other.currentFace;

    {
        //this->currentDestination = other.destinationList;
        //while ( currentDestination && currentDestination->messageID != sync.messageID )
        //    currentDestination = currentDestination->next;

        // deterimine last passed destination by the message ID
        this->currentDestination = GetDestinationBefore( sync, other.destinationList );

        bool trustDestination = true;
        if ( currentDestination && sn_GetNetState() == nCLIENT && !sg_sendCorrectLastTurn.Supported(0) )
        {
            // the server may send wrong information in the rare case that
            // our last turn command was not promptly executed. Sanity check:
            // if the relevant information from the alleged last destination
            // differs from the current state, it is already the next destination.
            if ( ( currentDestination->braking != (bool)braking ) || fabs( currentDestination->direction * dirDrive ) > .01 )
                trustDestination = false;
        }

        // we only need the next one
        if ( trustDestination && currentDestination )
            currentDestination = currentDestination->next;
    }

    // let extrapolator find its face ( and set position and time )
    MoveSafely( sync.pos, sync.time, sync.time );

    // set last turn
    lastTurnTime_ = -100;
}

// *******************************************************************************************
// *
// *	InitAfterCreation
// *
// *******************************************************************************************
//!
//!
// *******************************************************************************************

void gCycleMovement::InitAfterCreation( void )
{
#ifdef DEBUG
    if (!finite(verletSpeed_))
        st_Breakpoint();
#endif
    eNetGameObject::InitAfterCreation();
#ifdef DEBUG
    if (!finite(verletSpeed_))
        st_Breakpoint();
#endif
    MyInitAfterCreation();
}

// version feature indicating that proper scaling of the base acceleration with the speed multiplier is used
static nVersionFeature sg_correctAccelerationScaling( 8 );

// *******************************************************************************************
// *
// *	CalculateAcceleration
// *
// *******************************************************************************************
//!
//!		@param	dt	length of timestep
//!
// *******************************************************************************************

void gCycleMovement::CalculateAcceleration( REAL dt )
{
    // calculate acceleration
    acceleration=0;

    // brake: it's only available since this version...
    static nVersionFeature brakeDepletion(2);

    // and servers starting from this version disable it my modifying config items
    static nVersionFeature brakeDepletionHandledWithConfig(10);

    // simply use the configured brake always on the server
    // and on the client if the server should have disabled it, but does not.
    if ( sn_GetNetState() != nCLIENT || brakeDepletion.Supported() || brakeDepletionHandledWithConfig.Supported(0) )
    {
        if(braking)
        {
            if ( brakingReservoir > 0.0 )
            {
                brakingReservoir -= dt * sg_cycleBrakeDeplete;
                acceleration-=sg_brakeCycle * SpeedMultiplier();
            }
            else
                brakingReservoir = 0.0f;
        }
        else
        {
            if ( brakingReservoir < 1.0 )
            {
                brakingReservoir += dt * sg_cycleBrakeRefill;
            }
            else
                brakingReservoir = 1.0f;
        }
    }
    else
    {
        if(braking)
        {
            acceleration-=sg_brakeCycle * SpeedMultiplier();
        }
    }

    sg_ArchiveReal( acceleration, 9 );

    REAL baseSpeed = sg_speedCycle * SpeedMultiplier();
    if ( verletSpeed_ < ( sg_correctAccelerationScaling.Supported() ? baseSpeed : sg_speedCycle ) )
        acceleration+=( baseSpeed - verletSpeed_) * sg_speedCycleDecayBelow;
    else
        acceleration+=( baseSpeed - verletSpeed_) * sg_speedCycleDecayAbove;

    sg_ArchiveReal( acceleration, 9 );

    // sense near wall behind us, accelerate more
    REAL totalWallAcceleration = 0; // total acceleration by walls
    // REAL tunnelWidth           = 0; // with of the tunnel the cycle is in
    bool slingshot  = true;         // flag indicating whether the cycle is between two walls
    bool oneOwnWall = false;        // flag indicating whether one of the walls is your own
    for(int d=1;d>=-1;d-=2){
        gSensor rear(this,pos,dirDrive.Turn(-1,d));
        rear.detect(sg_nearCycle);

        if ( rear.ehit )
        {
            // see if the wall is parallel to the driving direction, only then should it add speed
            eCoord wallVec = rear.ehit->Vec();
            if ( fabs( eCoord::F( wallVec, dirDrive  ) ) > .9 )
            {
                // enemyInfluence.AddSensor( rear, 1 );
                REAL wallAcceleration=SpeedMultiplier() * sg_accelerationCycle * ((1/(rear.hit+sg_accelerationCycleOffs))
                                      -(1/(sg_nearCycle+sg_accelerationCycleOffs)));

                // tunnelWidth += rear.hit;

                // apply modificators
                switch (rear.type)
                {
                case gSENSOR_SELF:
                    wallAcceleration *= sg_accelerationCycleSelf;
                    oneOwnWall = true;
                    break;
                case gSENSOR_TEAMMATE:
                    wallAcceleration *= sg_accelerationCycleTeam;
                    break;
                case gSENSOR_ENEMY:
                    wallAcceleration *= sg_accelerationCycleEnemy;
                    break;
                case gSENSOR_RIM:
                    wallAcceleration *= sg_accelerationCycleRim;
                    break;
                case gSENSOR_NONE:
                    wallAcceleration = 0;
                    slingshot = false;
                    break;

                }
                totalWallAcceleration += wallAcceleration;
            }
            else
            {
                slingshot = false;
            }
        }
        else
        {
            slingshot = false;
        }

        sg_ArchiveReal( totalWallAcceleration, 9 );
    }

    /*
    // kill cycle if it is inside a too narrow channel
    if ( sn_GetNetState() != nCLIENT && slingshot && tunnelWidth < sg_cycleWidth )
    {
        st_Breakpoint();
        throw gCycleDeath( NULL, pos );
    }
    */

    // apply slingshot multiplier
    if ( slingshot && oneOwnWall )
        totalWallAcceleration *= sg_accelerationCycleSlingshot;

    // apply wall acceleration
    acceleration += totalWallAcceleration;

    sg_ArchiveReal( acceleration, 9 );
}

// *******************************************************************************************
// *
// *	ApplyAcceleration
// *
// *******************************************************************************************
//!
//!		@param	dt		length of timestep
//!
// *******************************************************************************************

void gCycleMovement::ApplyAcceleration( REAL dt )
{
    sg_ArchiveReal( verletSpeed_, 9 );
    sg_ArchiveReal( dt, 9 );
    sg_ArchiveReal( acceleration, 9 );

    // the speed needs to be simulated for this half frame and half of the last frame
    REAL verletTimestep = sg_verletIntegration.Supported() ? .5 * ( dt + lastTimestep_ ) : dt;
    lastTimestep_ = dt;

    sg_ArchiveReal( verletTimestep, 9 );

    verletSpeed_+=acceleration*verletTimestep;

    // clamp speed
    REAL minSpeed = sg_speedCycle*SpeedMultiplier()*sg_speedCycleMin;
    REAL maxSpeed = ( 100 + sg_speedCycle*SpeedMultiplier() )* 100000;

    sg_ArchiveReal( minSpeed, 9 );
    sg_ArchiveReal( maxSpeed, 9 );
    sg_ArchiveReal( acceleration, 9 );

    if ( clamp( verletSpeed_, minSpeed, maxSpeed ) )
        acceleration = 0;

    sg_ArchiveReal( acceleration, 9 );

    sg_ArchiveReal( verletSpeed_, 9 );
}

// *******************************************************************************************
// *
// *	DoTurn
// *
// *******************************************************************************************
//!
//!		@param	dir +1 for right turns, -1 for left turns
//!		@return		true of the turn could be executed right now, false if it was queued
//!
// *******************************************************************************************

bool gCycleMovement::DoTurn( int dir )
{
    if ( turns == 0 )
        turns = 1;

    if (dir >  1) dir =  1;
    if (dir < -1) dir = -1;

    if ( CanMakeTurn( lastTime ) )
    {
        // store last postion
        lastTurnPos_ = pos;

        turns++;

        verletSpeed_ *= sg_cycleTurnSpeedFactor;
        rubberMalus  += sg_rubberCycleMalusTurn;

        // turn winding numbers
        int wn = windingNumberWrapped_;
        Grid()->Turn(wn, dir);
        this->SetWindingNumberWrapped( wn );

        eCoord nextDirDrive = Grid()->GetDirection(windingNumberWrapped_);

        // send out a sensor a bit backwards and forwards into the turn direction to
        // copy all temporary walls into the grid
        {
            eCoord range = nextDirDrive * .1 * Speed();
            gSensor gridder1( this, Position(), range );
            gridder1.detect( 1 );
            if ( gridder1.ehit )
                ::DropTempWall( nextDirDrive, gridder1 );

            gSensor gridder2( this, Position(), -range );
            gridder2.detect( 1 );
            if ( gridder2.ehit )
                ::DropTempWall( nextDirDrive, gridder2 );

            gSensor gridder3( this, Position()-range*.5, range );
            gridder3.detect( 1 );
            if ( gridder3.ehit )
                ::DropTempWall( nextDirDrive, gridder3 );

            // if edges have been inserted into the grid, find a new current face.
            FindCurrentFace();
        }

        // update driving directions
        lastDirDrive = dirDrive;

        lastTurnTime_ = lastTime;

        dirDrive = nextDirDrive;

#ifdef DEBUGOUTPUT
        if ( sg_cycleDebugPrintLevel > 0 )
            con << Player()->GetName() << " turned " << pos << "," << dirDrive << " " << tSysTimeFloat() << "\n";
#endif

        return true;
    }
    else {
        int maxPendingTurns=sg_cycleTurnMemory;
        int size = pendingTurns.size();
        // std::cerr << "size of " << &pendingTurns << ": " << size << std::endl;
        if (size <= maxPendingTurns)
            pendingTurns.push_back(dir);
        else {
            if(pendingTurns.empty()) return false; //just to be sure
            if(pendingTurns.back() != dir) {
                pendingTurns.pop_back(); //opposite turns cancel so the cycle still moves into the expected direction
            }
            else {
                pendingTurns.push_back(dir); //add it anyways...
            }
        }
    }

    return false;
}

// *******************************************************************************************
// *
// *	RightBeforeDeath
// *
// *******************************************************************************************
//!
//!		@param	numTries	number of times this function will be called approximately before the cycle will be killed
//!
// *******************************************************************************************

void gCycleMovement::RightBeforeDeath( int numTries )
{
}

// *******************************************************************************************
// *
// *	Die
// *
// *******************************************************************************************
//!
//!		@param	time	the time of death
//!
// *******************************************************************************************

void gCycleMovement::Die( REAL time )
{
    // only do something if you are alive
    if ( alive_ == 1 )
    {
        alive_ = -1;
        deathTime = time;
    }

    // or complete death if you died only recently
    if ( alive_ == -1 )
    {
        alive_ = 0;
    }
}

class gRecursionGuard
{
public:
    gRecursionGuard( bool & guard )
            : guard_( guard )
    {
        guard_ = false;
    }
    ~gRecursionGuard()
    {
        guard_ = true;
    }
private:
    bool & guard_;
};

// *******************************************************************************************
// *
// *	TimestepCore
// *
// *******************************************************************************************
//!
//!		@param	currentTime		time to simulate up to
//!		@return					true if the cycle is to be killed
//!
// *******************************************************************************************

bool gCycleMovement::TimestepCore( REAL currentTime )
{
    eCoord oldpos=pos;
    REAL lastSpeed=verletSpeed_;

    REAL ts=(currentTime-lastTime);

    // apply acceleration
    if ( sg_verletIntegration.Supported() )
        this->ApplyAcceleration( ts );

    //eDebugLine::SetTimeout( 2 );
    //eDebugLine::SetColor(1,1,0);
    //eDebugLine::Draw(pos, 4, pos, 4 + 20 * ts);

    sg_ArchiveCoord( pos, 9 );
    sg_ArchiveReal( ts, 9 );
    sg_ArchiveReal( verletSpeed_, 9 );

#ifdef DEBUG
    if ( ts > 2.0f )
    {
        int x;
        x = 0;
    }

    if ( verletSpeed_ > 30.0f )
    {
        int x;
        x = 0;
    }

    if ( acceleration > 100.0f )
    {
        int x;
        x = 0;
    }
#endif

    clamp(ts, -10, 10);

    REAL step=verletSpeed_*ts;

    int numTries = 0;
    bool emergency = false;

    rubberSpeedFactor = 1;

    // be a little nice and don't drive into the wall
    REAL rubber_granted=sg_rubberCycle;

    // phasing test debug code
    //if ( tSysTimeFloat() < 16 || ( Player() && Player()->IsHuman() ) )
    //    rubber_granted = 0;

    // rubber effectiveness right now
    REAL rubberEffectiveness = 1/(1 + rubberMalus );

    // make rubber more effective for high ping players
    if ( player )
    {
        if ( rubber_granted > 0 )
            // either by increasing the effectiveness...
            rubberEffectiveness *= ( rubber_granted + player->ping * sg_rubberCyclePing )/rubber_granted;
        else
            // or the reservoir.
            rubber_granted += player->ping * sg_rubberCyclePing;
    }

    // reduce it further if cycle turned recently
    {
        REAL delayTime = lastTurnTime_ + GetTurnDelay() * sg_rubberCycleDelay;
        if ( lastTime < delayTime )
        {
            rubberEffectiveness *= sg_rubberCycleDelayBonus;

            // if the target time is after the rubber delay ends...
            if( currentTime > delayTime )
            {
                static bool recurse = true;
                if (recurse)
                {
                    gRecursionGuard guard( recurse );

                    verletSpeed_=lastSpeed;
                    // do two small timesteps
                    return TimestepCore( delayTime ) || TimestepCore( currentTime );
                }
            }
        }
    }

    sg_ArchiveReal( rubberEffectiveness, 9 );

    // TODO: solve smooth position correction trouble with rubber
    if ( player && ( rubber_granted > rubber || sn_GetNetState() == nCLIENT ) && sg_rubberCycleSpeed > 0 && step > 0 && ( sn_GetNetState() == nCLIENT || rubberEffectiveness > 0 ) )
    {
        //clamp effectiveness of rubber
        if ( rubberEffectiveness <= 0 )
            rubberEffectiveness = .0001;
        {
            // modify rubber effectiveness by a speed dependant power law
            REAL speedFactor = verletSpeed_/(sg_speedCycle*SpeedMultiplier());

            rubberEffectiveness *= pow( speedFactor, sg_rubberCycleTimeBased );
        }

        // formerly: rubberFactor = .5
        REAL beta = ts * sg_rubberCycleSpeed;
        REAL rubberFactor;
        if ( beta > .001 )
            rubberFactor = 1 - exp( -beta );
        else
            rubberFactor = beta;        // better accuracy than the full formula

        // rubberFactor must not be too close to 1, otherwise we get precision trouble
        if ( rubberFactor > .999 )
            rubberFactor = .999;

        // revert to old rubber logic if old clients are connected
        if ( sg_rubberCycleLegacy && !sg_nonRippable.Supported() && rubberFactor < .5f )
            rubberFactor = .5f;

        // space we need to look ahead
        REAL neededSpace = step/rubberFactor;
        if ( neededSpace < step*3 || ts <= 0 )
            neededSpace = step*3;

        // determine how long we can drive on
        REAL space = MaxSpaceAhead( this, neededSpace );

        // if the available space in front is less than the space needed to slow down via
        // the rubber brake, activate rubber and slow down
        if ( space < neededSpace )
        {
            /*
            // debug output for sensitive space/time diagrams
            static REAL lastTimePrinted = 0;
            if ( currentTime > lastTimePrinted && Player() )
            {
                lastTimePrinted = currentTime;
                std::ofstream f( Player()->GetUserName() + "_space", std::ios::app );
                f << lastTime << " " << log(space) << "\n";
            }
            */

            // notify AIs of it
            emergency = true;

            // calculate the step the rubber code should do based on the decay factor
            // calculated earler
            REAL rubberStep = space * rubberFactor;
            if ( rubberStep > step )
                rubberStep = step;

            // clamp the step
            if (step<0)
                step=0;

            // calculate the amount of rubber needed for the desired brake effect
            REAL rubberneeded = step - rubberStep;
            if (rubberneeded < 0)
                rubberneeded = 0;

            // clamp rubberneeded to the amout of rubber available
            REAL rubberAvailable = ( rubber_granted - rubber ) * rubberEffectiveness;
            if ( sn_GetNetState() != nCLIENT && rubberneeded > rubberAvailable && Vulnerable() )
                rubberneeded = rubberAvailable;

            // update rubber usage
            rubber += rubberneeded / rubberEffectiveness;

            numTries = int((sg_rubberCycleTime * ( rubber_granted - rubber ) - 1 )/(sg_rubberCycleTime * step*1.5 + 1));
            int numTriesSpace = int(space*10/verletSpeed_);
            if ( numTriesSpace < numTries )
                numTriesSpace = 0;

            rubberSpeedFactor = 1 - rubberneeded/step;

            // correct the step to take, don't go backwards.
            step -= rubberneeded;
            if (step<0)
                step=0;

            //{
            //    rubber+=step;
            //    step=0;
            //}
        }
    }

    sg_ArchiveReal( step, 9 );

    // move forward
    eCoord nextpos;
    if ( verletSpeed_ >0 )
        nextpos=pos+dirDrive*step;
    else
        nextpos=pos;

    // call base timestep
    REAL lastTimeStorage = this->lastTime;
    if (eNetGameObject::Timestep(currentTime))
        return true;

    eCoord lastPos = pos;
    tJUST_CONTROLLED_PTR< eFace > lastFace = currentFace;
    try
    {
#ifdef DEBUG
        static int run = 0;
        run++;
        if ( run == -1 )
        {
            st_Breakpoint();
        }
#endif
        Move(nextpos,currentTime,currentTime);
#ifdef DEBUG
        {
            if ( step > 0 && ( nextpos - pos ).NormSquared() > 1 )
            {
                con << "Wrong move! run = " << run << ", nextpos = " << nextpos << ", pos = " << pos << "\n";
            }
        }
#endif
        distance += step;
        lastTimeAlive_ = currentTime;
    }
    catch ( gCycleDeath const & )
    {
        rubberSpeedFactor = 0;

        // the cycle should die in this movement. Prevent it if there is rubber left.
        // if RUBBER_MINDISTANCE is negative and the player is not an AI, the cycle dies anyway.
        if ( rubberEffectiveness <= 0 || step >= (rubber_granted-rubber)*rubberEffectiveness || ( sg_rubberCycleMinDistance < 0 && Player() && Player()->IsHuman() ) )
        {
            // last survival chance: packet loss protection. Determine whether it should be in effect..
            bool toleratePacketLoss = false;
            if (!currentDestination)
            {
                // calculate time tolerance to capture packet loss...
                REAL tolerance = Lag() * sg_packetLossTolerance;

                // add lag fluctuation to the mix
                if ( sn_GetNetState() == nSERVER && player && player->Owner() != 0 )
                {
                    REAL varianceTolerance = 2 * sqrtf( sn_Connections[ player->Owner() ].ping.GetSnailAverager().GetDataVariance() );
                    // clamp it, high fluctuations are the player's own problem
                    if ( varianceTolerance > tolerance )
                        varianceTolerance = tolerance;
                    tolerance = varianceTolerance;
                }

                // if time has not progressed beyond tolerance, protection may be in effect
                toleratePacketLoss = ( currentTime - lastTimeAlive_ < tolerance );
            }

            // ... and apply it.
            if ( toleratePacketLoss )
            {
                this->lastTime = lastTimeStorage;
                pos = lastPos;
                verletSpeed_ = lastSpeed;
                currentFace = lastFace;
                numTries = 0;
                emergency = true;

                // don't simulate further
                return false;
            }
            else
            {
                // no, no straw left. Rethrow and get killed.
                rubber = rubber_granted;

                // update distance to include the really covered space
                distance += eCoord::F( dirDrive, pos - lastPos );

                throw;
            }
        }
        else
        {
            pos = lastPos;
            currentFace = lastFace;
            rubber += step/rubberEffectiveness;

            numTries = 0;
            emergency = true;
        }
    }

    // let rubber decay
    if ( sg_rubberCycleTime > 0 )
        rubber /= (1+ts/sg_rubberCycleTime);
    else
        rubber = 0;

    // let rubber decay
    if ( sg_rubberCycleMalusTime > 0 )
        rubberMalus /= (1+ts/sg_rubberCycleMalusTime);
    else
        rubberMalus = 0;


    // clamp rubber ( mostly for client side HUD display )
    if ( rubber > rubber_granted )
        rubber = rubber_granted;


    lastTime=currentTime;

    // give the AI a chance to evade just in time
    if (emergency)
    {
        RightBeforeDeath(numTries);
    }

#ifdef DEBUGOUTPUT
    if ( sg_cycleDebugPrintLevel > 1 )
        con << Player()->GetName() << " moved " << pos << "," << dirDrive << " " << tSysTimeFloat() << "\n";
#endif

    /*
    // debug output for sensitive rubber/time diagrams
    static REAL lastTimePrinted = 0;
    if ( currentTime > lastTimePrinted && Player() )
    {
        lastTimePrinted = currentTime;
        std::ofstream f( Player()->GetUserName() + "_rubber", std::ios::app );
        f << currentTime << " " << rubber << "\n";
    }
    */

    // apply acceleration
    if ( !sg_verletIntegration.Supported() )
        this->ApplyAcceleration( ts );

    return false;
}

// *******************************************************************************************
// *
// *	MyInitAfterCreation
// *
// *******************************************************************************************
//!
//!
// *******************************************************************************************

void gCycleMovement::MyInitAfterCreation( void )
{
#ifdef DEBUG
    // con << "creating cycle.\n";
#endif
    brakingReservoir = 1.0f;

    braking = false;

    acceleration = 0;

    dir=dirDrive;
    lastDirDrive=dirDrive;
    lastTurnPos_=pos;

    distance=0;
    // wallContDistance = 5;
    rubber=0.0f;
    rubberMalus=0.0f;
    rubberSpeedFactor=1.0f;

    alive_ = 1;

    z=.75;

    turns=1;

    pendingTurns.clear();
    lastTurnTime_=lastTime-10;

    lastTimeAlive_ = lastTime;

    if (!finite(verletSpeed_)){
        st_Breakpoint();
        verletSpeed_ = 1;
    }

    if (verletSpeed_ < .1)
        verletSpeed_=.1;

#ifdef DEBUGOUTPUT
    if ( sg_cycleDebugPrintLevel > 0 )
        con << Player()->GetName() << " created " << pos << "," << dirDrive << " " << tSysTimeFloat() << "\n";
#endif
}

// *******************************************************************************************
// *
// *	Init_gCycleCore
// *
// *******************************************************************************************
//!
//!
// *******************************************************************************************

//void gCycleMovement::Init_gCycleCore( void )
//{
//    assert(0); // implement me
//}

// *******************************************************************************************
// *
// *	Finit_gCycleCore
// *
// *******************************************************************************************
//!
//!
// *******************************************************************************************

//void gCycleMovement::Finit_gCycleCore( void )
//{
//    assert(0); // implement me
//}

// *******************************************************************************************
// *
// *	gCycleMovement
// *
// *******************************************************************************************
//!
//!
// *******************************************************************************************

//gCycleMovement::gCycleMovement( void )
//{
//    assert(0); // implement me
//}

// *******************************************************************************************
// *
// *	gCycleMovement
// *
// *******************************************************************************************
//!
//!		@param	other
//!
// *******************************************************************************************

//gCycleMovement::gCycleMovement( gCycleMovement const & other )
//{
//    assert(0); // implement me
//}

// *******************************************************************************************
// *
// *	operator =
// *
// *******************************************************************************************
//!
//!		@param	other
//!		@return
//!
// *******************************************************************************************

//gCycleMovement & gCycleMovement::operator =( gCycleMovement const & other )
//{
//    assert(0); // implement me
//    return gCycleMovement();
//}

// *******************************************************************************************
// *
// *	CopyFrom
// *
// *******************************************************************************************
//!
//!		@param	other
//!
// *******************************************************************************************

//void gCycleMovement::CopyFrom( const gCycleMovement & other )
//{
//   assert(0); // implement me
//}

// *******************************************************************************************
// *
// *	DoGetDistanceSinceLastTurn
// *
// *******************************************************************************************
//!
//!		@return		the distance driven since the last turn
//!
// *******************************************************************************************

REAL gCycleMovement::DoGetDistanceSinceLastTurn( void ) const
{
    return eCoord::F( dirDrive, pos - lastTurnPos_ );
}

// *******************************************************************************************
// *
// *	RubberMalusActive
// *
// *******************************************************************************************
//!
//!		@return
//!
// *******************************************************************************************

bool gCycleMovement::RubberMalusActive( void )
{
    return sg_rubberCycleMalusTurn > 0;
}

// *******************************************************************************************
// *
// *	MoveSafely
// *
// *******************************************************************************************
//!
//!		@param	dest	   the destination position
//!		@param	startTime  the start time of the movement
//!		@param	endTime	   the end time of the movement
//!
// *******************************************************************************************

void gCycleMovement::MoveSafely( const eCoord & dest, REAL startTime, REAL endTime )
{
    short lastAlive = alive_;
    alive_ = 0;
    Move( dest, startTime, endTime );
    alive_ = lastAlive;
}

REAL GetTurnSpeedFactor(void) {
    return sg_cycleTurnSpeedFactor;
}

