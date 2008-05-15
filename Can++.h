/*
 *  $Id: Can++.h,v 1.3 2008-05-15 08:53:48 ueshiba Exp $
 */
#ifndef __TUCanPP_h
#define __TUCanPP_h

#include <sys/types.h>
#include <can4linux.h>
#include "TU/Vector++.h"
#include <stdexcept>

namespace TU
{
/************************************************************************
*  class Can								*
************************************************************************/
/*!
  CAN(Control Area Network)$B$N%N!<%I$rI=$9%/%i%9!%(B
*/
class Can
{
  public:
  //! $BDL?.B.EY(B
    enum Baud
    {
	B10k	=   10,		//!< 10k baud
	B20k	=   20,		//!< 20k baud
	B40k	=   40,		//!< 40k baud
	B50k	=   50,		//!< 50k baud
	B100k	=  100,		//!< 100k baud
	B125k	=  125,		//!< 125k baud
	B250k	=  250,		//!< 250k baud
	B500k	=  500,		//!< 500k baud
	B800k	=  800,		//!< 800k baud
	B1000k	= 1000,		//!< 1000k baud
    };
    
  public:
    Can(const char* dev)				;
    ~Can()						;

    Can&		setBaud(Baud baud)		;
    
  protected:
    u_long		nreceive()			;
    u_long		receive()			;
    
  //! $B%a%C%;!<%8$N(BID$B$rJV$9(B
  /*!
    \return $B8=:_FI$_9~$^$l$F$$$k%a%C%;!<%8$N(BID$B!%(B*/
    u_long		id()			const	{return _msg.id;}
  //! $B%a%C%;!<%8$K4^$^$l$k%G!<%?$N%P%$%H?t$rJV$9(B
  /*!
    \return $B8=:_FI$_9~$^$l$F$$$k%a%C%;!<%8$K4^$^$l$k%G!<%?$N%P%$%H?t!%(B*/
    u_int		nbytes()		const	{return _msg.length;}
    u_char		get(u_int i)		const	;
    
    Can&		setId(u_long id)		;
    Can&		put(u_char c)			;
    const Can&	send()			const	;
    
  private:
    Can(const Can&)					;
    Can&		operator =(const Can&)	;
    
    const int	_fd;
    canmsg_t	_msg;
};

/************************************************************************
*  class Manus								*
************************************************************************/
/*!
  Manus$B%^%K%T%e%l!<%?(B(Exact Dynamics$B<R(B)$B$rI=$9%/%i%9!%(B
*/
class Manus : public Can
{
  public:
  //! Manus$B$N8=:_0LCV(B
    typedef Vector<int, FixedSizedBuf<int, 7> >	Position;
  //! Manus$B$X$NB.EY;XNaCM(B
    typedef Vector<int, FixedSizedBuf<int, 7> >	Speed;

  //! Manus$B$NF0:n%b!<%I(B
    enum Mode
    {
	STILL	  = 0x370,	//!< control box 0: startup/initialization.
	CARTESIAN = 0x371,	//!< control box 1: cartesian control.
	JOINT	  = 0x374,	//!< control box 4: joint control.
	FOLD_OUT  = 0x375,	//!< control box 5: folding out.
	FOLD_IN	  = 0x376	//!< control box 6: folding in.
    };

  //! Manus$B$N>uBV(B
    enum Status
    {
	OK			= 0x00,	//!< $B@5>o(B

      // Warnings.
	STUCK_GRIPPER		= 0x40,	//!< $B%0%j%C%Q$,>c32J*$K>WFM(B
	WRONG_AREA		= 0x41,	//!< $BJQ$J;Q@*$+$i(Bcartesian/fold$B$K0\9T(B
	ARM_FOLDED_STRETCHED	= 0x42,	//!< $B%"!<%`$,1d$S$-$C$?(B
	BLOCKED_DOF		= 0x43,	//!< $B2aIi2Y(B/$B>WFM(B
	MAXIMUM_M1_ROTATION	= 0x44,	//!< $B2sE>3Q$N8BEY$r1[$($?(B

      // General messages.
	FOLDED				= 0x80,	//!< fold$B>uBV(B
	UNFOLDED			= 0x81,	//!< unfold$B>uBV(B
	GRIPPER_REDAY_INITIALISING	= 0x82,	//!< gripper ready
	ABSOLUTE_MEASURING_READY	= 0x83,	//!< cartesian mode ready

      // Errors.
	IO_80C552_ERROR			= 0xc1,	//!< user I/O$B$N%(%i!<(B
	ABSOLUTE_ENCODER_ERROR		= 0xc4,	//!< $B%(%s%3!<%@$N%(%i!<(B
	MOVE_WITHOUT_USER_INPUT_ERROR	= 0xcf,	//!< $BF~NO$,$J$$$N$KF0$$$?(B
	UNKNOWN_ERROR			= 0xc5	//!< $B$=$NB>$N%(%i!<(B
    };

  //! Manus$B$X$NB.EY;XNa$N:GBgCM(B
    enum SpeedLimits
    {
	MAX_SPEED_CART_XYZ	= 127,	//!< xyz$B<4$N:GBgB.EY(B(cartesian mode)
	MAX_SPEED_CART_YPR	=  10,	//!< ypr$B<4$N:GBgB.EY(B(cartesian mode)
	MAX_SPEED_CART_GRIP	=  15,	//!< $B%0%j%C%Q$N:GBgB.EY(B(cartesian mode)
	MAX_SPEED_JOINT_012	=  10,	//!< 012$B<4$N:GBgB.EY(B(joint mode)
	MAX_SPEED_JOINT_345	=  10,	//!< 345$B<4$N:GBgB.EY(B(joint mode)
	MAX_SPEED_JOINT_GRIP	=  15	//!< $B%0%j%C%Q$N:GBgB.EY(B(joint mode)
    };

  //! cartesian$B%b!<%I$G$N3F<4$N:BI8CM$N:GBg(B/$B:G>.CM(B
    enum CartesianLimits
    {
	MIN_CART_XYZ	=   -720,	//!< xyz$B<4$N:G>.CM(B
	MAX_CART_XYZ	=    720,	//!< xyz$B<4$N:GBgCM(B
	MIN_CART_YAW	=  -1800,	//!< yaw$B<4$N:G>.CM(B
	MAX_CART_YAW	=   1800,	//!< yaw$B<4$N:GBgCM(B
	MIN_CART_PITCH	=   -900,	//!< pitch$B<4$N:G>.CM(B
	MAX_CART_PITCH	=    900,	//!< pitch$B<4$N:GBgCM(B
	MIN_CART_ROLL	=  -1800,	//!< roll$B<4$N:G>.CM(B
	MAX_CART_ROLL	=   1800,	//!< roll$B<4$N:GBgCM(B
	MIN_CART_GRIP	=  28100,	//!< grip$B<4$N:GBgCM(B
	MAX_CART_GRIP	=  54000	//!< grip$B<4$N:GBgCM(B
    };
    
  //! joint$B%b!<%I$G$N3F<4$N:BI8CM$N:GBg(B/$B:G>.CM(B
    enum JointLimits
    {
	MIN_JOINT_012	=  -1800,	//!< $BBh(B012$B<4$N:G>.CM(B
	MAX_JOINT_012	=   1800,	//!< $BBh(B012$B<4$N:GBgCM(B
	MIN_JOINT_3	=  -1800,	//!< $BBh(B3$B<4(B(yaw)$B$N:G>.CM(B
	MAX_JOINT_3	=   1800,	//!< $BBh(B3$B<4(B(yaw)$B$N:GBgCM(B
	MIN_JOINT_4	=      0,	//!< $BBh(B4$B<4$N:G>.CM(B
	MAX_JOINT_4	=   1266,	//!< $BBh(B4$B<4$N:GBgCM(B
	MIN_JOINT_5	=  -1800,	//!< $BBh(B5$B<4(B(roll)$B$N:G>.CM(B
	MAX_JOINT_5	=   1800,	//!< $BBh(B5$B<4(B(roll)$B$N:GBgCM(B
	MIN_JOINT_GRIP	=  28100,	//!< grip$B<4$N:G>.CM(B
	MAX_JOINT_GRIP	=  54000	//!< grip$B<4$N:GBgCM(B
    };

  //! Manus$B$N%(%i!<(B
    class Error : public std::runtime_error
    {
      public:
      //! $B%(%i!<%*%V%8%'%/%H$r@8@.$9$k(B
      /*!
	\param stat $B%(%i!<$rI=$9(BManus$B$N>uBVJQ?t(B. */
	Error(Status stat)
	    :std::runtime_error(Manus::message(stat)), status(stat)	{}
	
      //! $B%(%i!<$rI=$9(BManus$B$N>uBVJQ?t(B
	const Status	status;
    };
    
  public:
    Manus(const char* dev)				;

  //! $B8=:_$N%^%K%T%e%l!<%?$NF0:n%b!<%I$rJV$9(B
  /*!
    \return $B8=:_$N%b!<%I!%(B*/
    Mode		mode()			const	{return _mode;}
  //! $B8=:_$N%^%K%T%e%l!<%?$N>uBV$rJV$9(B
  /*!
    \return $B8=:_$N>uBV!%(B*/
    Status		status()		const	{return _status;}
			operator bool()		const	;
  //! $B8=:_$N%^%K%T%e%l!<%?$N0LCV$rJV$9(B
  /*!
    \return $B8=:_$N0LCV!%(B#CARTESIAN$B%b!<%I$N>l9g$O%O%s%I$N(Bcatesian$B:BI8!%(B
    $B$=$&$G$J$$>l9g$O4X@a:BI8!%(B*/
    const Position&	position()		const	{return _pos;}
  //! $B8=:_$N%^%K%T%e%l!<%?$X$NB.EY;XNaCM$rJV$9(B
  /*!
    \return $B8=:_$NB.EY;XNaCM!%(B#CARTESIAN$B%b!<%I$N>l9g$O3F:BI8<4$NB.EY!%(B
    $B$=$&$G$J$$>l9g$O4X@a3QB.EY!%(B*/
    const Speed&	speed()			const	{return _speed;}

    Manus&		foldOut()			;
    Manus&		foldIn()			;
    Manus&		stillMode()			;
    Manus&		cartesianMode()			;
    Manus&		jointMode()			;
    Manus&		setBaseUp()			;
    Manus&		setBaseStill()			;
    Manus&		setBaseDown()			;
    Manus&		setSpeed(const Speed& speed)	;
    Manus&		tick()				;
    Manus&		baseUp()			;
    Manus&		baseDown()			;
    Manus&		moveTo(const Position& ref)	;

    static const char*	message(Status status)		;
    
  private:
    static Status	toStatus(u_char hi, u_char low)	;
    
    Mode	_mode;
    Status	_status;
    Position	_pos;		// current cartesian coordinates/joint angles.
    int		_upDown;	// up: -1, down: 1, still: 0.
    Speed	_speed;		// speed given to the manipulator.
};

//! $B8=:_$N%^%K%T%e%l!<%?$,(Bwarning$B$b$7$/$O(Berror$B>uBV$K$J$$$+D4$Y$k(B
/*!
  \return	warning$B>uBV$K$b(Berror$B>uBV$K$b$J$1$l$P(Btrue$B$rJV$9!%(B
		$B$=$&$G$J$1$l$P(Bfalse$B$rJV$9!%(B
*/
inline
Manus::operator bool() const
{
    return (_status & 0x40 ? true : false);
}
 
}
#endif	/* !__TUCanPP_h	*/
