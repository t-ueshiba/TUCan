/*
 *  $Id: Can.cc,v 1.2 2002-07-25 02:38:01 ueshiba Exp $
 */
/*!
  \mainpage	libTUCan++ - CAN$B$*$h$S(BManus$B%^%K%T%e%l!<%?%3%s%H%m!<%i(B
  \anchor	libTUCan

  libTUCan++$B$O!$(BCAN(Control Area Network)$B%G%P%$%9$*$h$S(B
  <a href="http://www.exactdynamics.nl/">Exact Dynamics$B<R(B</a>$B@=$N(BManus
  $B%^%K%T%e%l!<%?$N$?$a$N%3%s%H%m!<%i%i%$%V%i%j$G$"$k!%(B

  libTUCan++$B$O!$%U%j!<$N(BCAN$B%\!<%IMQ%G%P%$%9%I%i%$%P$G$"$k(B
  <a href="http://www.port.de/engl/canprod/content/sw_linux.html">can4linux
  </a>$B$rMxMQ$7$F$$$k$N$G!$F0:n4D6-$b$3$l$K=`$8$k!%(BLINUX$B$N(Bkernel-2.2.18,
  kernel-2.4.18$B$G$NF0:n<B@S$,$"$k!%$*$=$i$/(Bkernel-2.2, kernel-2.4$B6&$K(B
  minor version$B$X$N0MB8@-$O$J$$$b$N$H?dB,$5$l$k!%$?$@$7!$(Bkernel-2.4.10
  $B0J9_$G$O!$(Bcan4linux$B$K<c43<j$r2C$($kI,MW$,$"$k(B(\ref can4linux)$B!%(B

  \section can4linux can4linux$B$N%$%s%9%H!<%k(B
  
  can4linux$B$r(B
  <a href="http://www.port.de/engl/canprod/content/sw_linux.html>
  $B%@%&%s%m!<%I(B</a>$B$7$FE83+$7$?8e!$0J2<$N:n6H$r9T$&!%(B

  $B$^$:!$0J2<$N$h$&$K$7$F(Bcan4linux$B%I%i%$%P$r%3%s%Q%$%k$9$k!%(B

  \verbatim
  % cd can4linux
  % make \endverbatim
  $B$J$*!$(Bsrc/can_82c200funcs.c, src/can_core.c, src/can_sysctl.c
  $B$K<j$r2C$($F$"$k!%(B

  $B<!$K!$%G%P%$%9%N!<%I$r:n@.$9$k!%(B

  \verbatim
  # mknod -m666 /dev/can0 c 91 0 \endverbatim

  $B$5$i$K!$(Bcan4linux$B%I%i%$%PMQ$N(Bconfiguration file$B$H$7$F!$(B
  /etc/can.conf$B$K0J2<$NFbMF(B(CAN$B%+!<%I$K$h$C$F0[$J$k!%0J2<$O(BManus
  $B%^%K%T%e%l!<%?IUB0$N(BAT$B%+!<%I$NNc(B)$B$N%U%!%$%k$r:n$k!%(B

  \verbatim
  # Channel 0

  Chipset_0=82c200	# Phillips 82c200 can controller.
  Base_0=0x0300		# Base I/O port address of this board is 0x300.
  Irq_0=5		# Must be equal to the jumper pin setting on the board.
  Baud_0=250		# Transmission speed of Manus manipulator is 250 baud.
  AccMask_0=0xffff
  Timeout_0=100
  Outc_0=0

  IOModel_0=p		# Must be "p"(port I/O) but "m"(memory-mapped).
  TxSpeed_0=s		# "s"(slow) is safer than "f"(fast).\endverbatim

  $B$9$k$H!$0J2<$N<jB3$-$G%I%i%$%P$N%m!<%I!?%"%s%m!<%I$,$G$-$k$h$&$K$J$k!%(B
  \verbatim
  # make load			// $B%I%i%$%P$N%m!<%I(B. 
  # make unload			// $B%I%i%$%P$N%"%s%m!<%I(B. 
  % grep . /proc/sys/Can/*	// $B@5$7$/%I%i%$%P$,%m!<%I$5$l$F$$$k$+3NG'(B.\endverbatim

  PC$B$N5/F0;~$K%I%i%$%P$,<+F0E*$K%m!<%I$5$l$k$?$a$K$O!$$^$:(B
  \verbatim
  # mkdir /lib/modules/`uname -r`/kernel/drivers/can
  # cp Can.o /lib/modules/`uname -r`/kernel/drivers/can
  # cp utils/cansetup /etc\endverbatim
  $B$r9T$$!$(B/etc/rc.d/rc.modules$B$K0J2<$NFbMF$rDI2C$9$k!%(B

  \verbatim
  #!/bin/sh

  canpath=/lib/modules/`uname -r`/kernel/drivers/can

  if [ -f "$canpath"/Can.o -a -x /etc/cansetup ]; then
    /sbin/insmod $canpath/Can.o
    /etc/cansetup
  fi\endverbatim

  $B$5$i$K!$(B
  \verbatim
  # cd /etc
  # ln -s rc.d/rc.modules \endverbatim
  $B$H$7$F!$(B/etc$B$+$i$3$N%U%!%$%k$,8+$($k$h$&$K$7$F$*$/(B(RedHat7.x$B7O$N$?$a$NA<CV(B)$B!%(B

  \section example $B%W%m%0%i%`Nc(B - manustest

  $B40A4$J%W%m%0%i%`Nc$H$7$F!$%-!<%\!<%I%$%s%?!<%U%'!<%9$K$h$k(BManus
  $B%^%K%T%e%l!<%?$N%3%s%H%m!<%k%W%m%0%i%`(B: \ref manustest$B$,$"$k!%(B
*/
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include "TU/Can++.h"

namespace TU
{
/************************************************************************
*  class CanPort							*
************************************************************************/
//! CAN$B%N!<%I$r@8@.$9$k(B
/*!
  \param dev			$B$3$N%N!<%I$N%G%P%$%9L>(B(ex. /dev/can0)$B!%(B
  \exception std::runtime_error	$B%G%P%$%9$N(Bopen$B$K<:GT!%(B
*/
Can::Can(const char* dev)
    :_fd(open(dev, O_RDWR))
{
    using namespace	std;
    if (_fd < 0)
	throw runtime_error(string("TU::Can::Can: cannot open device!! ") + strerror(errno));
    _msg.flags	= 0;
    _msg.cob	= 0;
    _msg.id	= 0;
    _msg.length	= 0;
}

//! CAN$B%N!<%I$rGK2u$9$k(B
Can::~Can()
{
    if (_fd >= 0)
	close(_fd);
}

//! $B%a%C%;!<%8$,E~Ce$7$F$$$k>l9g$OFI$_9~$_!$$=$&$G$J$1$l$PD>$A$KLa$k(B
/*!
  \return			$BFI$_9~$s$@%a%C%;!<%8$N(BID$B!%E~Ce$7$F$$$J$$(B
				$B>l9g$O(B0xffffffff$B!%(B
  \exception std::runtime_error	$BFI$_9~$_$K<:GT!%(B
*/
u_long
Can::nreceive()
{
    using namespace	std;
    Receive_par_t	rx;
    rx.Rx     = &_msg;
    rx.error  = 0;
    rx.retval = 0;
    if (::ioctl(_fd, RECEIVE, &rx) < 0)
	throw runtime_error(string("ioctl (TU::Can::nreceive) ")
			    + strerror(errno));
    return (rx.retval != 0 ? id() : ~0);
}

//! $B%a%C%;!<%8$,E~Ce$9$k$^$GBT$C$FFI$_9~$`(B
/*!
  \return			$BFI$_9~$s$@%a%C%;!<%8$N(BID$B!%(B
  \exception std::runtime_error	$BFI$_9~$_$K<:GT!%(B
*/
u_long
Can::receive()
{
    using namespace	std;
    Receive_par_t	rx;
    rx.Rx     = &_msg;
    rx.error  = 0;
    rx.retval = 0;
    do
    {
	if (::ioctl(_fd, RECEIVE, &rx) < 0)
	    throw runtime_error(string("ioctl (TU::Can::receive) ")
				+ strerror(errno));
    } while (rx.retval == 0);
    return id();
}

//! $B%a%C%;!<%8$K4^$^$l$k%G!<%?$rJV$9(B
/*!
  \param i	$B%G!<%?$X$N(Bindex$B!%(B\f$0 \leq i < \f$nbytes()$B$G$J$1$l$P$J$i$J$$!%(B
  \return	$B8=:_FI$_9~$^$l$F$$$k%a%C%;!<%8$N(Bi$BHVL\$N%G!<%?!%(B
  \exception std::out_of_range	i$B$,HO0O30!%(B
*/
u_char
Can::get(u_int i) const
{
    if (i < 0 || i >= _msg.length)
	throw std::out_of_range("TU::Can::get: invalid index!!");
    return _msg.data[i];
}

//! $BDL?.B.EY$r@_Dj$9$k(B
/*!
  \param baud			$B@_Dj$7$?$$(Bbaud rate$B!%(B
  \return			$B$3$N(BCAN$B%N!<%I%*%V%8%'%/%H!%(B
  \exception std::runtime_error	ioctl$B$K<:GT!%(B
*/
Can&
Can::setBaud(Baud baud)
{
    using namespace		std;
    volatile Command_par_t	cmd;
    cmd.cmd = CMD_STOP;
    if (::ioctl(_fd, COMMAND, &cmd) == -1)
	throw runtime_error(string("ioctl (TU::Can::setBaud) ")
			    + strerror(errno));
    
    Config_par_t		cfg;
    cfg.target = CONF_TIMING;
    cfg.val1   = baud;
    if (::ioctl(_fd, CONFIG, &cfg) == -1)
	throw runtime_error(string("ioctl (TU::Can::setBaud) ")
			    + strerror(errno));
    
    cmd.cmd = CMD_START;
    if (::ioctl(_fd, COMMAND, &cmd) == -1)
	throw runtime_error(string("ioctl (TU::Can::setBaud) ")
			    + strerror(errno));

    return *this;
}

//! $BAw?.%a%C%;!<%8$N(BID$B$r@_Dj$9$k(B
/*!
  $B$3$l$^$G%a%C%;!<%8Cf$K$"$C$?%G!<%?$OGK4~$5$l$k!%(B
  \param id	$B@_Dj$7$?$$%a%C%;!<%8(BID$B!%(B
  \return	$B$3$N(BCAN$B%N!<%I%*%V%8%'%/%H!%(B
*/
Can&
Can::setId(u_long id)
{
    _msg.id	= id;
    _msg.length	= 0;
    return *this;
}

//! $BAw?.%a%C%;!<%8$K%G!<%?$r3JG<$9$k(B
/*!
  put()$B$rJ#?t2s9T$&$H!$%G!<%?$O$=$N=gHV$K3JG<$5$l$k!%$?$@$7!$(B1$B$D$N(B
  $B%a%C%;!<%8$K%G!<%?$O(B8$B$D$^$G$7$+3JG<$G$-$J$$!%(B
  \param c			$B3JG<$7$?$$%G!<%?!%(B
  \return			$B$3$N(BCAN$B%N!<%I%*%V%8%'%/%H!%(B
  \exception std::out_of_range	$B4{$K%a%C%;!<%8%P%C%U%!$,0lGU!%(B
*/
Can&
Can::put(u_char c)
{
    if (_msg.length >= CAN_MSG_LENGTH)
	throw std::out_of_range("TU::Can::put: message buffer is full!!");
    _msg.data[_msg.length++] = c;
    return *this;
}

//! $B%a%C%;!<%8$rAw?.$9$k(B
/*!
  \return			$B$3$N(BCAN$B%N!<%I%*%V%8%'%/%H!%(B
  \exception std::runtime_error	$BAw?.$K<:GT!%(B
*/
const Can&
Can::send() const
{
    using namespace	std;
  /*    if (::write(_fd, &_msg, 1) < 0)
	throw runtime_error(string("write (TU::Can::send) ") + strerror(errno)); */
    Send_par_t	tx;
    tx.Tx     = (canmsg_t*)&_msg;
    tx.error  = 0;
    tx.retval = 0;
    if (::ioctl(_fd, SEND, &tx) < 0)
	throw runtime_error(string("ioctl (TU::Can::send) ")
			    + strerror(errno));
    return *this;
}
 
}
