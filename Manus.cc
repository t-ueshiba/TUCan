/*
 *  $Id: Manus.cc,v 1.2 2002-07-25 02:38:01 ueshiba Exp $
 */
#include "TU/Can++.h"

namespace TU
{
/************************************************************************
*  static data								*
************************************************************************/
static const char *messages[] =
{
    "No messages.",						//  0

    "Warning: stuck gripper.",					//  1
    "Warning: wrong area.",					//  2
    "Warning: arm folded stretched.",				//  3
    "Warning: blocked DOF.",					//  4
    "Warning: maximum M1 rotation.",				//  5

    "General: folded.",						//  6
    "General: unfolded.",					//  7
    "General: gripper ready initialising.",			//  8
    "General: absolute measuring ready.",			//  9

    "Error:   I/O 80c522 error.",				// 10
    "Error:   absolute encoder error.",				// 11
    "Error:   move without user input error.",			// 12
    "Error:   unknown error."					// 13
};

/************************************************************************
*  static functions							*
************************************************************************/
inline int
toInt(u_char hi, u_char lo)
{
    return int(short((hi << 8) | lo));
}

inline u_int
toUint(u_char hi, u_char lo)
{
    return u_int((hi << 8) | lo);
}

inline int
limit(int val, int min, int max)
{
    return (val < min ? min : val > max ? max : val);
}

inline int
sign(int val)
{
    return (val > 0 ? 1 : val < 0 ? -1 : 0);
}

inline int
abs(int val)
{
    return (val > 0 ? val : -val);
}

static bool
isZero(const Manus::Speed& speed)
{
    for (int i = 0; i < speed.dim(); ++i)
	if (speed[i] != 0)
	    return false;
    return true;
}

/************************************************************************
*  class Manus								*
************************************************************************/
//! Manus$B%^%K%T%e%l!<%?%N!<%I$r@8@.$9$k(B
/*!
  $B%^%K%T%e%l!<%?$O(B#STILL$B%b!<%I$K=i4|2=$5$l$k!%(B
  \param dev	Manus$B%^%K%T%e%l!<%?$N%G%P%$%9L>(B(ex. /dev/can0)
*/
Manus::Manus(const char* dev)
    :Can(dev), _mode(STILL), _status(OK), _pos(), _upDown(0), _speed()
{
    stillMode();
}

//! $B@^$j>v$^$l$F$$$k%^%K%T%e%l!<%?$r3H$2$k(B
/*!
  $B3H$2=*$o$k$H!$Bf:B$O@E;_$K!$$9$Y$F$N<4$NB.EY;XNaCM$O(B0$B$K$=$l$>$l%j%;%C%H(B
  $B$5$l$F<+F0E*$K(B#JOINT$B%b!<%I$K$J$k!%(B
  \return	$B$3$N(BManus$B%^%K%T%e%l!<%?%*%V%8%'%/%H!%(B
*/
Manus&
Manus::foldOut()
{
    _mode = FOLD_OUT;
    _upDown = 0;
    _speed = 0;
    do
    {
	tick();
    } while (_status != WRONG_AREA && _status != UNFOLDED);
    
    return jointMode();
}

//! $B%^%K%T%e%l!<%?$r3JG<0LCV$K@^$j>v$`(B
/*!
  $B@^$j>v$_$,40N;$9$k$H!$Bf:B$O@E;_$K!$$9$Y$F$N<4$NB.EY;XNaCM$O(B0$B$K$=$l$>$l(B
  $B%j%;%C%H$5$l$F<+F0E*$K(B#JOINT$B%b!<%I$K$J$k!%(B
  \return	$B$3$N(BManus$B%^%K%T%e%l!<%?%*%V%8%'%/%H!%(B
*/
Manus&
Manus::foldIn()
{
    Position	ref;
    ref[0] =  -590;
    ref[1] =   610;
    ref[2] =  -550;
    ref[3] = -1550;
    ref[4] =  1040;
    ref[5] = -1155;
    ref[6] = 25000;
    jointMode().moveTo(ref);
    
    _mode = FOLD_IN;
    _upDown = 0;
    _speed = 0;
    do
    {
	tick();
    } while (_status != WRONG_AREA && _status != FOLDED);
    
    return jointMode();
}

//! $B%^%K%T%e%l!<%?$r(B#STILL$B%b!<%I$K0\9T$9$k(B
/*!
  $BBf:B$O@E;_$K!$$9$Y$F$N<4$NB.EY;XNaCM$O(B0$B$K$=$l$>$l%j%;%C%H$5$l$k!%(B
  \return	$B$3$N(BManus$B%^%K%T%e%l!<%?%*%V%8%'%/%H!%(B
*/
Manus&
Manus::stillMode()
{
    _mode = STILL;
    _upDown = 0;
    _speed = 0;
    return tick();
}

//! $B%^%K%T%e%l!<%?$r(B#CARTESIAN$B%b!<%I$K0\9T$9$k(B
/*!
  $B0\9T$K<:GT$7$?>l9g$O!$(B#JOINT$B%b!<%I$K0\9T$9$k!%(B
  $BBf:B$O@E;_$K!$$9$Y$F$N<4$NB.EY;XNaCM$O(B0$B$K$=$l$>$l%j%;%C%H$5$l$k!%(B
  \return	$B$3$N(BManus$B%^%K%T%e%l!<%?%*%V%8%'%/%H!%(B
*/
Manus&
Manus::cartesianMode()
{
    _mode = CARTESIAN;
    _upDown = 0;
    _speed = 0;
    tick();
    if (_status == WRONG_AREA)
	jointMode();
    return *this;
}

//! $B%^%K%T%e%l!<%?$r(B#JOINT$B%b!<%I$K0\9T$9$k(B
/*!
  $BBf:B$O@E;_$K!$$9$Y$F$N<4$NB.EY;XNaCM$O(B0$B$K$=$l$>$l%j%;%C%H$5$l$k!%(B
  \return	$B$3$N(BManus$B%^%K%T%e%l!<%?%*%V%8%'%/%H!%(B
*/
Manus&
Manus::jointMode()
{
    _mode = JOINT;
    _upDown = 0;
    _speed = 0;
    return tick();
}

//! $BBf:B$r>e>:$5$;$k$h$&$K@_Dj$9$k(B
/*!
  $B$9$Y$F$N<4$NB.EY;XNaCM$O(B0$B$K%j%;%C%H$5$l$k!%(B
  $B<!$N(Btick()$B$N8F$S=P$7$G<B:]$KBf:B$,>e>:$9$k!%(B
  \return	$B$3$N(BManus$B%^%K%T%e%l!<%?%*%V%8%'%/%H!%(B
*/
Manus&
Manus::setBaseUp()
{
    _upDown = -1;
    _speed = 0;
    return *this;
}

//! $BBf:B$r@E;_$5$;$k$h$&$K@_Dj$9$k(B
/*!
  $B$9$Y$F$N<4$NB.EY;XNaCM$O(B0$B$K%j%;%C%H$5$l$k!%(B
  $B<!$N(Btick()$B$N8F$S=P$7$G<B:]$KBf:B$,@E;_$9$k!%(B
  \return	$B$3$N(BManus$B%^%K%T%e%l!<%?%*%V%8%'%/%H!%(B
*/
Manus&
Manus::setBaseStill()
{
    _upDown = 0;
    _speed = 0;
    return *this;
}

//! $BBf:B$r2<9_$5$;$k$h$&$K@_Dj$9$k(B
/*!
  $B$9$Y$F$N<4$NB.EY;XNaCM$O(B0$B$K%j%;%C%H$5$l$k!%(B
  $B<!$N(Btick()$B$N8F$S=P$7$G<B:]$KBf:B$,2<9_$9$k!%(B
  \return	$B$3$N(BManus$B%^%K%T%e%l!<%?%*%V%8%'%/%H!%(B
*/
Manus&
Manus::setBaseDown()
{
    _upDown = 1;
    _speed = 0;
    return *this;
}

//! $BB.EY;XNaCM$r@_Dj$9$k(B
/*!
  #CARTESIAN/#JOINT$B%b!<%I$N>l9g$O!$;XDj$5$l$?B.EYCM$,(B#SpeedLimits
  $B$NHO0O$K<}$^$i$J$1$l$P!$<}$^$k$h$&$K:GBg(B/$B:G>.CM$,@_Dj$5$l$k!%(B
  #CARTESIAN/#JOINT$B%b!<%I0J30$N>l9g$O!$B.EYCM$O(B0$B$K@_Dj$5$l$k!%$I$A$i$N>l9g$b(B
  $BBf:B$O@E;_$K%j%;%C%H$5$l$k!%<!$N(Btick()$B$N8F$S=P$7$G<B:]$KB.EY;XNa$,%^%K(B
  $B%T%e%l!<%?$KAw$i$l$k!%(B
  \param speed	$BB.EY;XNaCM!%(B#CARTESIAN$B%b!<%I$N>l9g$O3F:BI8<4$NB.EY!%$=$&$G$J$$(B
		$B>l9g$O4X@a3QB.EY!%(B
  \return	$B$3$N(BManus$B%^%K%T%e%l!<%?%*%V%8%'%/%H!%(B
*/
Manus&
Manus::setSpeed(const Speed& speed)
{
    _upDown = 0;
    switch (_mode)
    {
      case CARTESIAN:
	for (int i = 0; i < 3; ++i)
	    _speed[i] = limit(speed[i],
			      -MAX_SPEED_CART_XYZ, MAX_SPEED_CART_XYZ);
	for (int i = 3; i < 6; ++i)
	    _speed[i] = limit(speed[i],
			      -MAX_SPEED_CART_YPR, MAX_SPEED_CART_YPR);
	_speed[6] = limit(speed[6], -MAX_SPEED_CART_GRIP, MAX_SPEED_CART_GRIP);
        break;
      case JOINT:
	for (int i = 0; i < 3; ++i)
	    _speed[i] = limit(speed[i],
			      -MAX_SPEED_JOINT_012, MAX_SPEED_JOINT_012);
	for (int i = 3; i < 6; ++i)
	    _speed[i] = limit(speed[i],
			      -MAX_SPEED_JOINT_345, MAX_SPEED_JOINT_345);
	_speed[6] = limit(speed[6],
			  -MAX_SPEED_JOINT_GRIP, MAX_SPEED_JOINT_GRIP);
        break;
      default:
	_speed = 0;
	break;
    }
    return *this;
}

//! $B%^%K%T%e%l!<%?$N@)8f%k!<%W$r(B1$B2s$@$12s$9(B
/*!
  $B%^%K%T%e%l!<%?$N8=:_0LCV$rFI$_9~$`!%FI$_9~$s$@8=:_0LCV$O!$(Bposition()$B$GCN$k(B
  $B$3$H$,$G$-$k!%$5$i$K!$(B#CARTESIAN/#JOINT$B%b!<%I$N(B
  $B>l9g$O!$8=:_$NB.EY;XNaCM$r%^%K%T%e%l!<%?$KAw$k!%(B
  \return		$B$3$N(BManus$B%^%K%T%e%l!<%?%*%V%8%'%/%H!%(B
  \exception Error	$B%^%K%T%e%l!<%?$K%(%i!<$,@8$8$?!%(B
*/
Manus&
Manus::tick()
{
    for (int n = 0; n < 3; ++n)
	switch (receive())
	{
	  case 0x350:	// status, message and first 3 coordinates/angles.
	    _status = toStatus(get(0), get(1));
	    if ((_status & 0xc0) == 0xc0)
		throw Error(_status);
	    _pos[0] = toInt(get(2), get(3));
	    _pos[1] = toInt(get(4), get(5));
	    _pos[2] = toInt(get(6), get(7));
	    break;

	  case 0x360:	// last 4 coordinates/angles.
	    _pos[3] = toInt(get(0), get(1));
	    _pos[4] = toInt(get(2), get(3));
	    _pos[5] = toInt(get(4), get(5));
	    _pos[6] = toUint(get(6), get(7));
	    break;

	  case 0x37f:	// prompt for selecting control box.
	    setId(_mode);
	    if (_mode == CARTESIAN || _mode == JOINT)
	    {
		put(_upDown);
		for(int i = 0; i < _speed.dim(); ++i)
		    put(_speed[i]);
	    }
	    send();
	    break;
	}

    return *this;
}

//! $B%^%K%T%e%l!<%?$NBf:B$r:G9bE@$^$G>e$2$k(B
/*!
  #CARTESIAN$B$^$?$O(B#JOINT$B%b!<%I$G$N$_M-8z!%(B
  \return			$B$3$N(BManus$B%^%K%T%e%l!<%?%*%V%8%'%/%H!%(B
  \exception std::runtime_error	#CARTESIAN$B$^$?$O(B#JOINT$B%b!<%I$G$J$$!%(B
*/
Manus&
Manus::baseUp()
{
    if (_mode != CARTESIAN && _mode == JOINT)
	throw std::runtime_error("TU::Manus::up(): Neighter CARTESIAN nor JOINT mode!!");
    
    setBaseUp();
    for (int n = 0; n < 700; ++n)
	tick();

    return *this;
}

//! $B%^%K%T%e%l!<%?$NBf:B$r:GDcE@$^$G2<$2$k(B
/*!
  #CARTESIAN$B$^$?$O(B#JOINT$B%b!<%I$G$N$_M-8z!%(B
  \return			$B$3$N(BManus$B%^%K%T%e%l!<%?%*%V%8%'%/%H!%(B
  \exception std::runtime_error	#CARTESIAN$B$^$?$O(B#JOINT$B%b!<%I$G$J$$!%(B
*/
Manus&
Manus::baseDown()
{
    if (_mode != CARTESIAN && _mode == JOINT)
	throw std::runtime_error("Manus::down(): Neighter CARTESIAN nor JOINT mode!!");
    
    setBaseDown();
    for (int n = 0; n < 700; ++n)
	tick();

    return *this;
}

//! $B%^%K%T%e%l!<%?$rL\I80LCV$^$GF0$+$9(B
/*!
  #CARTESIAN$B$^$?$O(B#JOINT$B%b!<%I$G$N$_M-8z!%(B
  \param ref			$BL\I80LCV(B
  \return			$B$3$N(BManus$B%^%K%T%e%l!<%?%*%V%8%'%/%H!%(B
  \exception std::runtime_error	#CARTESIAN$B$^$?$O(B#JOINT$B%b!<%I$G$J$$!%(B
*/
Manus&
Manus::moveTo(const Position& ref)
{
  /*    for (int i = _pos.dim(); --i >= 0; )
    {
	Speed	speed;
	for (;;)
	{
	    setSpeed(speed).tick();
	    speed[i] = ref[i] - _pos[i];
	    cerr << "    " << i << "-th axis: "
		 << ref[i] << " - " << _pos[i] << " = " << speed[i] << endl;
	    if (abs(speed[i]) < 10)
		break;
	    speed[i] = sign(speed[i]);
	}
	}*/

    Speed	speed;
    for (;;)
    {
	setSpeed(speed).tick();

	speed = ref - _pos;

	cerr << " moveTo: position =" << _pos
	     << "         error    =" << speed;
	
	int	i;
	for (i = 0; i < 6; ++i)
	    if (abs(speed[i]) >= 20)
		break;
	if (i == 6)
	    break;
	
	for (i = 0; i < speed.dim(); ++i)
	    speed[i] = sign(speed[i]);
    } 
    
    return *this;
}

//! $B%^%K%T%e%l!<%?$N>uBV$r%a%C%;!<%8J8;zNs$KJQ49$9$k(B
/*!
  \param status	$B%^%K%T%e%l!<%?$N>uBV!%(B
  \return	$B%a%C%;!<%8J8;zNs!%(B
*/
const char*
Manus::message(Status status)
{
    switch (status)
    {
      case OK:
	return messages[0];

      // Warnings.
      case STUCK_GRIPPER:
	return messages[1];
      case WRONG_AREA:
	return messages[2];
      case ARM_FOLDED_STRETCHED:
	return messages[3];
      case BLOCKED_DOF:
	return messages[4];
      case MAXIMUM_M1_ROTATION:
	return messages[5];
	
      // General messages.
      case FOLDED:
	return messages[6];
      case UNFOLDED:
	return messages[7];
      case GRIPPER_REDAY_INITIALISING:
	return messages[8];
      case ABSOLUTE_MEASURING_READY:
	return messages[9];

      // Errors.
      case IO_80C552_ERROR:
	return messages[10];
      case ABSOLUTE_ENCODER_ERROR:
	return messages[11];
      case MOVE_WITHOUT_USER_INPUT_ERROR:
	return messages[12];
    }
    return messages[13];
}

Manus::Status
Manus::toStatus(u_char hi, u_char lo)
{
    hi &= 0xc0;

    if (hi == 0)
	return OK;
    
    switch (hi | lo)
    {
      // Warnings.
      case STUCK_GRIPPER:
	return STUCK_GRIPPER;
      case WRONG_AREA:
	return WRONG_AREA;
      case ARM_FOLDED_STRETCHED:
	return ARM_FOLDED_STRETCHED;
      case BLOCKED_DOF:
	return BLOCKED_DOF;
      case MAXIMUM_M1_ROTATION:
	return MAXIMUM_M1_ROTATION;
	
      // General messages.
      case FOLDED:
	return FOLDED;
      case UNFOLDED:
	return UNFOLDED;
      case GRIPPER_REDAY_INITIALISING:
	return GRIPPER_REDAY_INITIALISING;
      case ABSOLUTE_MEASURING_READY:
	return ABSOLUTE_MEASURING_READY;

      // Errors.
      case IO_80C552_ERROR:
	return IO_80C552_ERROR;
      case ABSOLUTE_ENCODER_ERROR:
	return ABSOLUTE_ENCODER_ERROR;
      case MOVE_WITHOUT_USER_INPUT_ERROR:
	return MOVE_WITHOUT_USER_INPUT_ERROR;
    }
    
    return UNKNOWN_ERROR;
}

}
#ifdef __GNUG__
#  include "TU/Geometry++.cc"

namespace TU
{
template
CoordBase<int, 7u>&	CoordBase<int, 7u>::operator =(double);
}

#endif
